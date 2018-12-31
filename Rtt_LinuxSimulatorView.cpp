//////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2018 Corona Labs Inc.
// Contact: support@coronalabs.com
//
// This file is part of the Corona game engine.
//
// Commercial License Usage
// Licensees holding valid commercial Corona licenses may use this file in
// accordance with the commercial license agreement between you and 
// Corona Labs Inc. For licensing terms and conditions please contact
// support@coronalabs.com or visit https://coronalabs.com/com-license
//
// GNU General Public License Usage
// Alternatively, this file may be used under the terms of the GNU General
// Public license version 3. The license is as published by the Free Software
// Foundation and appearing in the file LICENSE.GPL3 included in the packaging
// of this file. Please review the following information to ensure the GNU 
// General Public License requirements will
// be met: https://www.gnu.org/licenses/gpl-3.0.html
//
// For overview and more information on licensing please refer to README.md
//
//////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include <wx/string.h>
#include <wx/menu.h>
#include <wx/textctrl.h>

//#include "Core\Rtt_Build.h"
#include "Rtt_AuthorizationTicket.h"
#include "Rtt_LuaContext.h"
#include "Rtt_LuaFile.h"
#include "Rtt_MPlatform.h"
#include "Rtt_PlatformAppPackager.h"
#include "Rtt_PlatformPlayer.h"
#include "Rtt_PlatformSimulator.h"
#include "Rtt_RenderingStream.h"
#include "Rtt_Runtime.h"
#include "Rtt_SimulatorAnalytics.h"
#include "Rtt_Authorization.h"
#include "Rtt_LinuxPlatform.h"
#include "Rtt_LinuxSimulatorServices.h"
#include "Rtt_LinuxSimulatorView.h"
#include "Rtt_SimulatorRecents.h"
#include "Rtt_WebAppPackager.h"
#include "Rtt_LinuxAppPackager.h"
#include "Rtt_AndroidAppPackager.h"
#include "Rtt_WebServicesSession.h"
#include "Core/Rtt_FileSystem.h"

namespace Rtt
{

	LinuxPlatformServices::LinuxPlatformServices(MPlatform* platform)
		: fPlatform(platform)
	{
	}

	void LinuxSimulatorView::onAndroidBuild(wxCommandEvent& e)
	{
		int rc;
		
		androidBuildParams* params = (androidBuildParams*)e.GetEventUserData();
		CoronaAppContext* ctx = params->fCtx;
		const char* srcDir = ctx->getAppPath();
		const char* dstDir = ctx->getSaveFolder().c_str();
		const char* identity = "";
		const char* applicationName = ctx->getAppName().c_str();

		// Create the app packager.
		LinuxPlatform* platform = wxGetApp().getPlatform();
		MPlatformServices* service = new LinuxPlatformServices(platform);

		std::string resourcesDir = platform->getInstallDir();
		resourcesDir += LUA_DIRSEP;
		resourcesDir += "Resources";
		AndroidAppPackager packager(*service, resourcesDir.c_str());

		// Read the application's "build.settings" file.
		bool wasSuccessful = packager.ReadBuildSettings(srcDir);
		if (!wasSuccessful)
		{
			return; 
		}

		// Check if a custom build ID has been assigned.
		// This is typically assigned to daily build versions of Corona.
		const char * customBuildId = packager.GetCustomBuildId();
		if (!Rtt_StringIsEmpty(customBuildId))
		{
			Rtt_Log("\nUsing custom Build Id %s\n", customBuildId);
		}

		// these are currently unused
		const char *bundleId = "bundleId"; //TODO
		const TargetDevice::Platform targetPlatform(TargetDevice::Platform::kAndroidPlatform);
		const char *versionName = "1.0.0";
		bool isDistribution = true;
		
		// get path to android template
		std::string templateDir = platform->getInstallDir();
		templateDir += LUA_DIRSEP;
		templateDir += "Resources";
		
		// these are currently unused
		const char *provisionFile = "";
		const char *sdkRoot = templateDir.c_str();
		const char *targetAppStoreName = "google";
		std::string package(params->fPackage->GetValue().c_str());
		std::string keystore(params->fKeystore->GetValue().c_str());
		const char *keystore_pwd = "android";
		const char *alias = "androiddebugkey";
		const char *alias_pwd = "android";
		int versionCode = 1;

		// Package build settings parameters.
		AndroidAppPackagerParams androidBuilderParams(
		applicationName, versionName, identity, provisionFile,	
		srcDir,	dstDir,	sdkRoot,  
		targetPlatform, targetAppStoreName,
		(S32)Rtt::TargetDevice::VersionForPlatform(Rtt::TargetDevice::kAndroidPlatform),
		customBuildId, NULL,
		package.c_str(), isDistribution, keystore.c_str(), keystore_pwd, alias, alias_pwd, versionCode );

		// Select build template
		Rtt::Runtime* runtimePointer = ctx->GetRuntime();
//		U32 luaModules = runtimePointer->VMContext().GetModules();
//		androidBuilderParams.InitializeProductId(luaModules);

		const char kBuildSettings[] = "build.settings";
		Rtt::String buildSettingsPath;
		ctx->getPlatform()->PathForFile(kBuildSettings, Rtt::MPlatform::kResourceDir, Rtt::MPlatform::kTestFileExists, buildSettingsPath);
		androidBuilderParams.SetBuildSettingsPath(buildSettingsPath.GetString());

		std::string tmp = Rtt_GetSystemTempDirectory();
		tmp += LUA_DIRSEP;
		tmp += "CoronaLabs";

		WebServicesSession* session = NULL;

		// Have the server build the app. (Warning! This is a long blocking call.)
		platform->SetActivityIndicator(true);
		rc = packager.Build(&androidBuilderParams, *session, tmp.c_str());
		platform->SetActivityIndicator(false);

		params->fDlg->Close();

		wxMessageDialog* dial;
		if (rc == 0)
		{
			dial = new wxMessageDialog(NULL, wxT("Your application was built successfully."), wxT("Corona Simulator"), wxOK | wxICON_INFORMATION);
		}
		else
		{
			dial = new wxMessageDialog(NULL, wxT("Failed to build application."), wxT("Corona Simulator"), wxOK | wxICON_ERROR);
		}
		dial->ShowModal();
	}

	void LinuxSimulatorView::onLinuxBuild(wxCommandEvent& e)
	{
		linuxBuildParams* params = (linuxBuildParams*)e.GetEventUserData();
		CoronaAppContext* ctx = params->fCtx;
		bool useStandartResources = params->fUseStandardResources->GetValue();
		const char* srcDir = ctx->getAppPath();
		const char* dstDir = ctx->getSaveFolder().c_str();
		const char* identity = "no-identity";
		const char* applicationName = ctx->getAppName().c_str();

		// Create the app packager.
		LinuxPlatform* platform = wxGetApp().getPlatform();
		MPlatformServices* service = new LinuxPlatformServices(platform);
		LinuxAppPackager packager(*service);

		// Read the application's "build.settings" file.
		bool wasSuccessful = packager.ReadBuildSettings(srcDir);
		if (!wasSuccessful)
		{
			return; 
		}

		// Check if a custom build ID has been assigned.
		// This is typically assigned to daily build versions of Corona.
		const char * customBuildId = packager.GetCustomBuildId();
		if (!Rtt_StringIsEmpty(customBuildId))
		{
			Rtt_Log("\nUsing custom Build Id %s\n", customBuildId);
		}

		// these are currently unused
		const char *bundleId = "bundleId"; //TODO
		const char *sdkRoot = "";
		int targetVersion = Rtt::TargetDevice::kLinux;
		const TargetDevice::Platform targetPlatform(TargetDevice::Platform::kLinuxPlatform);
		const char *versionName = "1.0.0";
		bool isDistribution = true;
		
		std::string linuxtemplate = platform->getInstallDir();
		linuxtemplate += LUA_DIRSEP;
		linuxtemplate += "Resources";
		linuxtemplate += LUA_DIRSEP;
		linuxtemplate += "linuxtemplate.zip";

		// Package build settings parameters.
		LinuxAppPackagerParams linuxBuilderParams(
		applicationName, versionName, identity, NULL,	
		srcDir,	dstDir,	NULL,  
		targetPlatform, targetVersion,
		Rtt::TargetDevice::kLinux, customBuildId,
		NULL, bundleId, isDistribution, NULL, useStandartResources);

		// Select build template
		Rtt::Runtime* runtimePointer = ctx->GetRuntime();
	//	U32 luaModules = runtimePointer->VMContext().GetModules();
	//	webBuilderParams.InitializeProductId(luaModules);

		const char kBuildSettings[] = "build.settings";
		Rtt::String buildSettingsPath;
		ctx->getPlatform()->PathForFile(kBuildSettings, Rtt::MPlatform::kResourceDir, Rtt::MPlatform::kTestFileExists, buildSettingsPath);
		linuxBuilderParams.SetBuildSettingsPath(buildSettingsPath.GetString());

		std::string tmp = Rtt_GetSystemTempDirectory();
		tmp += LUA_DIRSEP;
		tmp += "CoronaLabs";

		WebServicesSession* session = NULL;

		// Have the server build the app. (Warning! This is a long blocking call.)
		platform->SetActivityIndicator(true);
		int rc = packager.Build(&linuxBuilderParams, *session, tmp.c_str());
		platform->SetActivityIndicator(false);

		params->fDlg->Close();

		wxMessageDialog* dial;
		if (rc == 0)
		{
			dial = new wxMessageDialog(NULL, wxT("Your application was built successfully."), wxT("Corona Simulator"), wxOK | wxICON_INFORMATION);
		}
		else
		{
			dial = new wxMessageDialog(NULL, wxT("Failed to build application."), wxT("Corona Simulator"), wxOK | wxICON_ERROR);
		}
		dial->ShowModal();
	}

	void LinuxSimulatorView::onWebBuild(wxCommandEvent& e)
	{
		webBuildParams* params = (webBuildParams*)e.GetEventUserData();
		CoronaAppContext* ctx = params->fCtx;
		bool useStandartResources = params->fUseStandardResources->GetValue();
		bool createFBInstantArchive = params->fCreateFBInstance->GetValue();
		const char* srcDir = ctx->getAppPath();
		const char* dstDir = ctx->getSaveFolder().c_str();
		const char* identity = "no-identity";
		const char* applicationName = ctx->getAppName().c_str();

		// Create the app packager.
		LinuxPlatform* platform = wxGetApp().getPlatform();
		MPlatformServices* service = new LinuxPlatformServices(platform);
		WebAppPackager packager(*service);

		// Read the application's "build.settings" file.
		bool wasSuccessful = packager.ReadBuildSettings(srcDir);
		if (!wasSuccessful)
		{
			return; // CBuildResult(Rtt::WebServicesSession::kBuildError, message);
		}

		// Check if a custom build ID has been assigned.
		// This is typically assigned to daily build versions of Corona.
		const char * customBuildId = packager.GetCustomBuildId();
		if (!Rtt_StringIsEmpty(customBuildId))
		{
			Rtt_Log("\nUsing custom Build Id %s\n", customBuildId);
		}

		// these are currently unused
		const char *bundleId = "bundleId"; //TODO
		const char *sdkRoot = "";
		int targetVersion = Rtt::TargetDevice::kWeb1_0;
		const TargetDevice::Platform targetPlatform(TargetDevice::Platform::kWebPlatform);
		const char *versionName = "1.0.0";
		bool isDistribution = true;
		
		std::string webtemplate = platform->getInstallDir();
		webtemplate += LUA_DIRSEP;
		webtemplate += "Resources";		
		webtemplate += LUA_DIRSEP;
		webtemplate += "webtemplate.zip";

		// Package build settings parameters.
		WebAppPackagerParams webBuilderParams(
			applicationName, versionName, identity, NULL,
			srcDir, dstDir, NULL,
			targetPlatform, targetVersion,
			Rtt::TargetDevice::kWebGenericBrowser, customBuildId,
			NULL, bundleId, isDistribution, useStandartResources, webtemplate.c_str(), createFBInstantArchive);

		// Select build template
		Rtt::Runtime* runtimePointer = ctx->GetRuntime();
	//	U32 luaModules = runtimePointer->VMContext().GetModules();
	//	webBuilderParams.InitializeProductId(luaModules);

		const char kBuildSettings[] = "build.settings";
		Rtt::String buildSettingsPath;
		ctx->getPlatform()->PathForFile(kBuildSettings, Rtt::MPlatform::kResourceDir, Rtt::MPlatform::kTestFileExists, buildSettingsPath);
		webBuilderParams.SetBuildSettingsPath(buildSettingsPath.GetString());

		std::string tmp = Rtt_GetSystemTempDirectory();
		tmp += LUA_DIRSEP;
		tmp += "CoronaLabs";

		WebServicesSession* session = NULL;

		// Have the server build the app. (Warning! This is a long blocking call.)
		platform->SetActivityIndicator(true);
		int rc = packager.Build(&webBuilderParams, *session, tmp.c_str());
		platform->SetActivityIndicator(false);

		params->fDlg->Close();

		wxMessageDialog* dial;
		if (rc == 0)
		{
			dial = new wxMessageDialog(NULL, wxT("Your application was built successfully."), wxT("Corona Simulator"), wxOK | wxICON_INFORMATION);
		}
		else
		{
			dial = new wxMessageDialog(NULL, wxT("Failed to build application."), wxT("Corona Simulator"), wxOK | wxICON_ERROR);
		}
		dial->ShowModal();
	}

	void LinuxSimulatorView::onCancel(wxCommandEvent& e)
	{
		cancelBuild* eb = (cancelBuild*)e.GetEventUserData();
		eb->fDlg->Close();
	}

	/// <summary>Opens a dialog to build the currently selected project as an app.</summary>
	void LinuxSimulatorView::OnBuildForWeb(CoronaAppContext* ctx)
	{
		wxDialog* OpenDialog = new wxDialog(NULL, -1, "HTML5 Build Setup (beta)", wxDefaultPosition, wxSize(550, 280));
		wxPanel *panel = new wxPanel(OpenDialog, -1);
		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

		int y = 15;
		int h = 30;
		int x2 = 170;
		wxSize sz = wxSize(350, 30);
		new wxStaticText(panel, -1, wxT("Application Name:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, ctx->getAppName(), wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Version Code:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, "1", wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Project Path:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, ctx->getAppPath(), wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Save to Folder:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, ctx->getSaveFolder(), wxPoint(x2, y - 2), sz);
		
		y += h + 5;
		wxCheckBox* useStandardResources = new wxCheckBox(panel, -1, "Include Standard Resources", wxPoint(x2, y));
		y += h;
		wxCheckBox* createFBInstance = new wxCheckBox(panel, -1, "Create FB Instant archive", wxPoint(x2, y));

		wxButton *okButton = new wxButton(OpenDialog, -1, wxT("Build"), wxDefaultPosition, wxSize(70, 30));
		okButton->Bind(wxEVT_BUTTON, &LinuxSimulatorView::onWebBuild, wxID_ANY, wxID_ANY, new webBuildParams(OpenDialog, ctx, useStandardResources, createFBInstance));		
		okButton->SetDefault(); 

		wxButton *closeButton = new wxButton(OpenDialog, -1, wxT("Cancel"), wxDefaultPosition, wxSize(70, 30));
		closeButton->Bind(wxEVT_BUTTON, &LinuxSimulatorView::onCancel, wxID_ANY, wxID_ANY, new cancelBuild(OpenDialog));		// cancel

		hbox->Add(okButton, 1);
		hbox->Add(closeButton, 1, wxLEFT, 5);

		vbox->Add(panel, 1);
		vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

		OpenDialog->SetSizer(vbox);
		OpenDialog->Centre();
		int rc = OpenDialog->ShowModal();
		OpenDialog->Destroy();
	}

	void LinuxSimulatorView::OnBuildForLinux(CoronaAppContext* ctx)
	{
		wxDialog* OpenDialog = new wxDialog(NULL, -1, "Linux Build Setup (beta)", wxDefaultPosition, wxSize(550, 280));
		wxPanel *panel = new wxPanel(OpenDialog, -1);
		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

		int y = 15;
		int h = 30;
		int x2 = 170;
		wxSize sz = wxSize(350, 30);
		new wxStaticText(panel, -1, wxT("Application Name:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, ctx->getAppName(), wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Version Code:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, "1", wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Project Path:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, ctx->getAppPath(), wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Save to Folder:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, ctx->getSaveFolder(), wxPoint(x2, y - 2), sz);
		
		y += h + 5;
		wxCheckBox* useStandardResources = new wxCheckBox(panel, -1, "Include Standard Resources", wxPoint(x2, y));

		wxButton *okButton = new wxButton(OpenDialog, -1, wxT("Build"), wxDefaultPosition, wxSize(70, 30));
		okButton->Bind(wxEVT_BUTTON, &LinuxSimulatorView::onLinuxBuild, wxID_ANY, wxID_ANY, new linuxBuildParams(OpenDialog, ctx, useStandardResources));		
		okButton->SetDefault(); 

		wxButton *closeButton = new wxButton(OpenDialog, -1, wxT("Cancel"), wxDefaultPosition, wxSize(70, 30));
		closeButton->Bind(wxEVT_BUTTON, &LinuxSimulatorView::onCancel, wxID_ANY, wxID_ANY, new cancelBuild(OpenDialog));		// cancel

		hbox->Add(okButton, 1);
		hbox->Add(closeButton, 1, wxLEFT, 5);

		vbox->Add(panel, 1);
		vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

		OpenDialog->SetSizer(vbox);
		OpenDialog->Centre();
		int rc = OpenDialog->ShowModal();
		OpenDialog->Destroy();
	}

	void LinuxSimulatorView::OnBuildForAndroid(CoronaAppContext* ctx)
	{
		wxDialog* OpenDialog = new wxDialog(NULL, -1, "Android Build Setup", wxDefaultPosition, wxSize(550, 450));
		wxPanel *panel = new wxPanel(OpenDialog, -1);
		wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
		wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

		int y = 15;
		int h = 30;
		int x2 = 170;
		wxSize sz = wxSize(350, h);
		
		std::string package;
		char uname[256] = {0};
		int rc = getlogin_r(uname, sizeof(uname));
		package = "com.coronalabs.";
		package += uname;
		package += ".";
		package += ctx->getAppName();
		std::replace(package.begin(), package.end(), ' ', '_'); // replace all ' ' to '_'
		
		new wxStaticText(panel, -1, wxT("Application Name:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, ctx->getAppName(), wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Version Code:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, "1", wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Version Name:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, "1.0.0", wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Package:"), wxPoint(20, y));
		wxTextCtrl* wxPackage = new wxTextCtrl(panel, -1, package.c_str(), wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, "A unique Java-style package identifier for your app\n(e.g. com.acme.games.myfarmgame)", wxPoint(x2, y));
		y += h + 15;
		new wxStaticText(panel, -1, wxT("Project Path:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, ctx->getAppPath(), wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Target App Store:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, "Google Play", wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Keystore:"), wxPoint(20, y));
		Rtt::String keystore;
		ctx->getPlatform()->PathForFile("debug.keystore", Rtt::MPlatform::kResourceDir, Rtt::MPlatform::kTestFileExists, keystore);
		wxTextCtrl* wxKeystore = new wxTextCtrl(panel, -1, keystore.GetString(), wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Key Alias:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, "androiddebugkey", wxPoint(x2, y - 2), sz);
		y += h;
		new wxStaticText(panel, -1, wxT("Save to Folder:"), wxPoint(20, y));
		new wxTextCtrl(panel, -1, ctx->getSaveFolder(), wxPoint(x2, y - 2), sz);
		y += h;
		new wxCheckBox(panel, -1, "Create Live Build", wxPoint(x2, y));

		wxButton *okButton = new wxButton(OpenDialog, -1, wxT("Build"), wxDefaultPosition, wxSize(70, 30));
		okButton->Bind(wxEVT_BUTTON, &LinuxSimulatorView::onAndroidBuild, wxID_ANY, wxID_ANY, 
		new androidBuildParams(OpenDialog, ctx, wxKeystore, wxPackage));	
		okButton->SetDefault(); 

		wxButton *closeButton = new wxButton(OpenDialog, -1, wxT("Cancel"), wxDefaultPosition, wxSize(70, 30));
		closeButton->Bind(wxEVT_BUTTON, &LinuxSimulatorView::onCancel, wxID_ANY, wxID_ANY, new cancelBuild(OpenDialog));		// cancel

		hbox->Add(okButton, 1);
		hbox->Add(closeButton, 1, wxLEFT, 5);

		vbox->Add(panel, 1);
		vbox->Add(hbox, 0, wxALIGN_CENTER | wxTOP | wxBOTTOM, 10);

		OpenDialog->SetSizer(vbox);
		OpenDialog->Centre();
		
		rc = OpenDialog->ShowModal();
		OpenDialog->Destroy();
	}

}

