//////////////////////////////////////////////////////////////////////////////
//
// This file is part of the Corona game engine.
// For overview and more information on licensing please refer to README.md 
// Home page: https://github.com/coronalabs/corona
// Contact: support@coronalabs.com
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Core/Rtt_Build.h"
#include "Core/Rtt_Array.h"
#include "Rtt_PlatformSimulator.h"
#include "Rtt_TargetDevice.h"
#include "Rtt_LinuxSimulatorServices.h"
#include "Rtt_LinuxContext.h"
#include "Rtt_MPlatformServices.h"
#include "Rtt_MPlatform.h"

namespace Rtt
{

	class LinuxPlatformServices : public MPlatformServices
	{
	public:
		LinuxPlatformServices(MPlatform* platform);
		//		~WinPlatformServices();

	protected:
		bool RequestAdminAuthorization(const char *name) const;

	public:
		// MPlatformServices
		virtual const MPlatform& Platform() const override { return *fPlatform; }
		virtual PlatformConnection* CreateConnection(const char* url) const override { Rtt_ASSERT(0); return NULL; };

		virtual void GetPreference(const char *key, String * value) const override
		{
			Preference::ReadValueResult val = fPlatform->GetPreference(key, "");
			const char* str = val.GetValue().ToString().GetValue()->c_str();
			value->Set(str);
		}

		// Pass NULL for value to remove pref
		virtual void SetPreference(const char *key, const char *value) const override
		{
//			fPlatform->SetPreference(key, value);
			int k=1;
		}

		virtual void GetSecurePreference(const char *key, String * value) const override { return GetPreference(key, value); }
		virtual bool SetSecurePreference(const char *key, const char *value) const { SetPreference(key, value); return true; }

		virtual void GetLibraryPreference(const char *key, String * value) const override  { return GetPreference(key, value); }
		virtual void SetLibraryPreference(const char *key, const char *value) const override { SetPreference(key, value); }

		virtual bool IsInternetAvailable() const override { return false; };
		virtual bool IsLocalWifiAvailable() const override { return false; };
		virtual void Terminate() const override {}

		virtual void Sleep(int milliseconds) const override {};

	private:
		MPlatform* fPlatform;
	};

	class LinuxSimulatorView
	{
	public:
		LinuxSimulatorView();
		virtual ~LinuxSimulatorView();

		static void OnBuildForLinux(CoronaAppContext* ctx);
		static void OnBuildForWeb(CoronaAppContext* ctx);
		static void OnBuildForAndroid(CoronaAppContext* ctx);

		static void onWebBuild(wxCommandEvent&);
		static void onLinuxBuild(wxCommandEvent&);
		static void onAndroidBuild(wxCommandEvent&);

		static void onCancel(wxCommandEvent&);
		static void appWebBuild(CoronaAppContext* ctx);

		struct androidBuildParams : public wxObject
		{
			androidBuildParams(wxDialog* dlg, CoronaAppContext* ctx, wxTextCtrl* keystore, wxTextCtrl* package)
				: fDlg(dlg)
				, fCtx(ctx)
				, fKeystore(keystore)
				, fPackage(package)
			{};

			wxDialog* fDlg;
			CoronaAppContext* fCtx;
			wxTextCtrl* fKeystore;
			wxTextCtrl* fPackage;
		};

		struct webBuildParams : public wxObject
		{
			webBuildParams(wxDialog* dlg, CoronaAppContext* ctx, wxCheckBox* useStandardResources, wxCheckBox* createFBInstance)
				: fDlg(dlg)
				, fCtx(ctx)
				, fUseStandardResources(useStandardResources)
				, fCreateFBInstance(createFBInstance)
			{};

			wxDialog* fDlg;
			CoronaAppContext* fCtx;
			wxCheckBox* fUseStandardResources;
			wxCheckBox* fCreateFBInstance;
			std::string fErrMsg;
		};

		struct linuxBuildParams : public wxObject
		{
			linuxBuildParams(wxDialog* dlg, CoronaAppContext* ctx, wxCheckBox* useStandardResources)
				: fDlg(dlg)
				, fCtx(ctx)
				, fUseStandardResources(useStandardResources)
			{};

			wxDialog* fDlg;
			CoronaAppContext* fCtx;
			wxCheckBox* fUseStandardResources;
			std::string fErrMsg;
		};

		struct cancelBuild : public wxObject
		{
			cancelBuild(wxDialog* dlg) : fDlg(dlg) {};
			wxDialog* fDlg;
		};

	};
}
