//////////////////////////////////////////////////////////////////////////////
//
// This file is part of the Corona game engine.
// For overview and more information on licensing please refer to README.md
// Home page: https://github.com/coronalabs/corona
// Contact: support@coronalabs.com
//
//////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <math.h>
#include <wx/string.h>
#include <wx/menu.h>
#include <wx/textctrl.h>
#include <sstream>
#include <fstream>
#include "Rtt_LuaContext.h"
#include "Rtt_LuaFile.h"
#include "Rtt_MPlatform.h"
#include "Rtt_PlatformAppPackager.h"
#include "Rtt_PlatformPlayer.h"
#include "Rtt_PlatformSimulator.h"
#include "Rtt_RenderingStream.h"
#include "Rtt_Runtime.h"
#include "Rtt_SimulatorAnalytics.h"
#include "Rtt_LinuxPlatform.h"
#include "Rtt_LinuxSimulatorServices.h"
#include "Rtt_LinuxSimulatorView.h"
#include "Rtt_LinuxFileUtils.h"
#include "Rtt_SimulatorRecents.h"
#include "Rtt_WebAppPackager.h"
#include "Rtt_LinuxAppPackager.h"
#include "Rtt_AndroidAppPackager.h"
#include "Core/Rtt_FileSystem.h"

#define SIMULATOR_CONFIG_SHOW_RUNTIME_ERRORS "/showRuntimeErrors"
#define SIMULATOR_CONFIG_RELAUNCH_ON_FILE_CHANGE "/relaunchOnFileChange"
#define SIMULATOR_CONFIG_OPEN_LAST_PROJECT "/openLastProject"
#define SIMULATOR_CONFIG_LAST_PROJECT_DIRECTORY "/lastProjectDirectory"
#define SIMULATOR_CONFIG_WINDOW_X_POSITION "/xPos"
#define SIMULATOR_CONFIG_WINDOW_Y_POSITION "/yPos"
#define SIMULATOR_CONFIG_SKIN_ID "/skinID"
#define SIMULATOR_CONFIG_SKIN_WIDTH "/skinWidth"
#define SIMULATOR_CONFIG_SKIN_HEIGHT "/skinHeight"
#define SIMULATOR_CONFIG_SKIN_ZOOMED_WIDTH "/zoomedWidth"
#define SIMULATOR_CONFIG_SKIN_ZOOMED_HEIGHT "/zoomedHeight"
//#define SIMULATOR_CONFIG_WELCOME_SCREEN_ZOOMED_WIDTH  "/welcomeScreenZoomedWidth"
//#define SIMULATOR_CONFIG_WELCOME_SCREEN_ZOOMED_HEIGHT "/welcomeScreenZoomedHeight"

using namespace std;

string& ltrim(string& str, const string& chars)
{
	str.erase(0, str.find_first_not_of(chars));
	return str;
}

string& rtrim(string& str, const string& chars)
{
	str.erase(str.find_last_not_of(chars) + 1);
	return str;
}

string& trim(string& str, const std::string& chars = "\t\n\v\f\r ")
{
	return ltrim(rtrim(str, chars), chars);
}

void split(vector<string>& cont, const string& str, const string& delims)
{
	const char* s = str.c_str();
	const char* delim = strstr(s, delims.c_str());
	while (delim)
	{
		string item(s, delim - s);
		cont.push_back(trim(item));
		s = delim + delims.size();
		delim = strstr(s, delims.c_str());
	}
	if (*s != 0)
		cont.push_back(s);
}

bool ReadRecentDocs(vector<string>& Names, vector<string>& Paths)
{
	std::string recent_path = Rtt::LinuxFileUtils::GetHomePath();
	recent_path += "/.Solar2D/recent_projects.conf";
	ifstream f(recent_path);
	if (f.is_open())
	{
		string line;
		while (getline(f, line))
		{
			std::vector<std::string> items;
			split(items, line, "=");
			if (items.size() == 2 && items[0].size() > 0 && items[1].size() > 0)
			{
				Names.push_back(items[0]);
				Paths.push_back(items[1]);
			}
		}
		f.close();
		return Names.size() > 0;
	}
	return false;
}

namespace Rtt
{
	LinuxPlatformServices::LinuxPlatformServices(MPlatform* platform)
		: fPlatform(platform)
	{
	}

	// static initialise vars
	const float  LinuxSimulatorView::skinScaleFactor = 1.5;
	const int  LinuxSimulatorView::skinMinWidth = 320;
	wxString LinuxSimulatorView::Config::settingsFilePath = wxEmptyString;
	wxString LinuxSimulatorView::Config::lastProjectDirectory = wxEmptyString;
	bool LinuxSimulatorView::Config::showRuntimeErrors = true;
	bool LinuxSimulatorView::Config::openLastProject = false;
	LinuxPreferencesDialog::RelaunchType LinuxSimulatorView::Config::relaunchOnFileChange = LinuxPreferencesDialog::RelaunchType::Always;
	int LinuxSimulatorView::Config::windowXPos = 10;
	int LinuxSimulatorView::Config::windowYPos = 10;
	int LinuxSimulatorView::Config::skinID = 6223;
	int LinuxSimulatorView::Config::skinWidth = 320;
	int LinuxSimulatorView::Config::skinHeight = 480;
	int LinuxSimulatorView::Config::zoomedWidth = LinuxSimulatorView::Config::skinWidth;
	int LinuxSimulatorView::Config::zoomedHeight = LinuxSimulatorView::Config::skinHeight;
//	int LinuxSimulatorView::Config::welcomeScreenZoomedWidth = 960;
//	int LinuxSimulatorView::Config::welcomeScreenZoomedHeight = 720;
	wxConfig* LinuxSimulatorView::Config::configFile;
	std::map<int, LinuxSimulatorView::SkinProperties> LinuxSimulatorView::fSkins;

	void LinuxSimulatorView::Config::Load()
	{
		if (LinuxSimulatorView::Config::settingsFilePath.IsEmpty())
		{
			LinuxSimulatorView::Config::settingsFilePath = LinuxFileUtils::GetHomePath();
			LinuxSimulatorView::Config::settingsFilePath.append("/.Solar2D/simulator.conf");
			LinuxSimulatorView::Config::configFile = new wxFileConfig(wxEmptyString, wxEmptyString, LinuxSimulatorView::Config::settingsFilePath);
		}

		// read from the simulator config file or create it, if it doesn't exist
		if (wxFileExists(LinuxSimulatorView::Config::settingsFilePath))
		{
			int relaunchOnFileChange = 0;
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_LAST_PROJECT_DIRECTORY), &LinuxSimulatorView::Config::lastProjectDirectory);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_SHOW_RUNTIME_ERRORS), &LinuxSimulatorView::Config::showRuntimeErrors);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_OPEN_LAST_PROJECT), &LinuxSimulatorView::Config::openLastProject);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_RELAUNCH_ON_FILE_CHANGE), &relaunchOnFileChange);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_WINDOW_X_POSITION), &LinuxSimulatorView::Config::windowXPos);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_WINDOW_Y_POSITION), &LinuxSimulatorView::Config::windowYPos);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_SKIN_ID), &LinuxSimulatorView::Config::skinID);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_SKIN_WIDTH), &LinuxSimulatorView::Config::skinWidth);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_SKIN_HEIGHT), &LinuxSimulatorView::Config::skinHeight);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_SKIN_ZOOMED_WIDTH), &LinuxSimulatorView::Config::zoomedWidth);
			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_SKIN_ZOOMED_HEIGHT), &LinuxSimulatorView::Config::zoomedHeight);
//			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_WELCOME_SCREEN_ZOOMED_WIDTH), &LinuxSimulatorView::Config::welcomeScreenZoomedWidth);
//			LinuxSimulatorView::Config::configFile->Read(wxT(SIMULATOR_CONFIG_WELCOME_SCREEN_ZOOMED_HEIGHT), &LinuxSimulatorView::Config::welcomeScreenZoomedHeight);
			LinuxSimulatorView::Config::relaunchOnFileChange = static_cast<LinuxPreferencesDialog::RelaunchType>(relaunchOnFileChange);
		}
		else
		{
			LinuxSimulatorView::Config::Save();
		}
	}

	void LinuxSimulatorView::Config::Save()
	{
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_LAST_PROJECT_DIRECTORY), LinuxSimulatorView::Config::lastProjectDirectory);
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_SHOW_RUNTIME_ERRORS), LinuxSimulatorView::Config::showRuntimeErrors);
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_OPEN_LAST_PROJECT), LinuxSimulatorView::Config::openLastProject);
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_RELAUNCH_ON_FILE_CHANGE), static_cast<int>(LinuxSimulatorView::Config::relaunchOnFileChange));
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_WINDOW_X_POSITION), LinuxSimulatorView::Config::windowXPos);
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_WINDOW_Y_POSITION), LinuxSimulatorView::Config::windowYPos);
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_SKIN_ID), LinuxSimulatorView::Config::skinID);
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_SKIN_WIDTH), LinuxSimulatorView::Config::skinWidth);
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_SKIN_HEIGHT), LinuxSimulatorView::Config::skinHeight);
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_SKIN_ZOOMED_WIDTH), LinuxSimulatorView::Config::zoomedWidth);
		LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_SKIN_ZOOMED_HEIGHT), LinuxSimulatorView::Config::zoomedHeight);
		//LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_WELCOME_SCREEN_ZOOMED_WIDTH), LinuxSimulatorView::Config::welcomeScreenZoomedWidth);
		//LinuxSimulatorView::Config::configFile->Write(wxT(SIMULATOR_CONFIG_WELCOME_SCREEN_ZOOMED_HEIGHT), LinuxSimulatorView::Config::welcomeScreenZoomedHeight);
		LinuxSimulatorView::Config::configFile->Flush();
	}

	void LinuxSimulatorView::Config::Cleanup()
	{
		delete LinuxSimulatorView::Config::configFile;
	}

	bool LinuxSimulatorView::LoadSkin(lua_State* L, int skinID, std::string filePath)
	{
		int top = lua_gettop(L);
		int status = Lua::DoFile(L, filePath.c_str(), 0, true);
		lua_pop(L, 1); // remove DoFile result
		lua_getglobal(L, "simulator");

		SkinProperties skin;

		if (lua_type(L, -1) == LUA_TTABLE)
		{
			lua_getfield(L, -1, "device");
			if (lua_type(L, -1) == LUA_TSTRING)
			{
				skin.device = lua_tostring(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "screenOriginX");
			if (lua_type(L, -1) == LUA_TNUMBER)
			{
				skin.screenOriginX = lua_tointeger(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "screenOriginY");
			if (lua_type(L, -1) == LUA_TNUMBER)
			{
				skin.screenOriginY = lua_tointeger(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "screenWidth");
			if (lua_type(L, -1) == LUA_TNUMBER)
			{
				skin.screenWidth = lua_tointeger(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "screenHeight");
			if (lua_type(L, -1) == LUA_TNUMBER)
			{
				skin.screenHeight = lua_tointeger(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "androidDisplayApproximateDpi");
			if (lua_type(L, -1) == LUA_TNUMBER)
			{
				skin.androidDisplayApproximateDpi = lua_tointeger(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "displayManufacturer");
			if (lua_type(L, -1) == LUA_TSTRING)
			{
				skin.displayManufacturer = lua_tostring(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "displayName");
			if (lua_type(L, -1) == LUA_TSTRING)
			{
				skin.displayName = lua_tostring(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "isUprightOrientationPortrait");
			if (lua_type(L, -1) == LUA_TBOOLEAN)
			{
				skin.isUprightOrientationPortrait = lua_toboolean(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "supportsScreenRotation");
			if (lua_type(L, -1) == LUA_TBOOLEAN)
			{
				skin.supportsScreenRotation = lua_toboolean(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "windowTitleBarName");
			if (lua_type(L, -1) == LUA_TSTRING)
			{
				skin.windowTitleBarName = lua_tostring(L, -1);
			}
			lua_pop(L, 1);

			// pop simulator
			lua_pop(L, 1);
			lua_settop(L, top);

			wxString skinTitle(skin.windowTitleBarName);
			skinTitle.append(wxString::Format(wxT(" (%ix%i)"), skin.screenWidth, skin.screenHeight));
			skin.skinTitle = skinTitle;
			skin.id = skinID;
			skin.selected = false;

			fSkins[skinID] = skin;
		}
		else
		{
			printf("Couldn't find 'simulator' table\n");
		}

		return (status == 0);
	}

	LinuxSimulatorView::SkinProperties LinuxSimulatorView::GetSkinProperties(int skinID)
	{
		if (fSkins.count(skinID) > 0)
		{
			return fSkins[skinID];
		}

		return fSkins.begin()->second;
	}

	LinuxSimulatorView::SkinProperties LinuxSimulatorView::GetSkinProperties(wxString skinTitle)
	{
		SkinProperties foundSkin;

		for (int i = 0; i < fSkins.size(); i++)
		{
			if (fSkins[i].skinTitle.IsSameAs(skinTitle))
			{
				foundSkin = fSkins[i];
				break;
			}
		}

		return foundSkin;
	}

	void LinuxSimulatorView::DeselectSkins()
	{
		for (int i = 0; i < fSkins.size(); i++)
		{
			fSkins[i].selected = false;
		}
	}

	void LinuxSimulatorView::SelectSkin(int skinID)
	{
		if (fSkins.count(skinID) > 0)
		{
			DeselectSkins();
			fSkins[skinID].selected = true;
		}
	}

	bool LinuxSimulatorView::IsRunningOnSimulator()
	{
#ifdef Rtt_SIMULATOR
		return true;
#endif

		return false;
	}

	void LinuxSimulatorView::OnLinuxPluginGet(const char* appPath, const char* appName, LinuxPlatform* platform)
	{
		const char* identity = "no-identity";

		// Create the app packager.
		MPlatformServices* service = new LinuxPlatformServices(platform);
		LinuxAppPackager packager(*service);

		// Read the application's "build.settings" file.
		bool wasSuccessful = packager.ReadBuildSettings(appPath);

		if (!wasSuccessful)
		{
			return;
		}

		int targetVersion = Rtt::TargetDevice::kLinux;
		const TargetDevice::Platform targetPlatform(TargetDevice::Platform::kLinuxPlatform);

		std::string linuxtemplate(platform->getInstallDir());
		linuxtemplate.append("/Resources/template_x64.tgz");

		// Package build settings parameters.
		LinuxAppPackagerParams linuxBuilderParams(
			appName, NULL, identity, NULL,
			appPath, NULL, NULL,
			targetPlatform, targetVersion,
			Rtt::TargetDevice::kLinux, NULL,
			NULL, NULL, false, NULL, false, false, true);

		const char kBuildSettings[] = "build.settings";
		Rtt::String buildSettingsPath;
		platform->PathForFile(kBuildSettings, Rtt::MPlatform::kResourceDir, Rtt::MPlatform::kTestFileExists, buildSettingsPath);
		linuxBuilderParams.SetBuildSettingsPath(buildSettingsPath.GetString());
		int rc = packager.Build(&linuxBuilderParams, NULL);
	}

	/// Gets a list of recent projects.
	void LinuxSimulatorView::GetRecentDocs(Rtt::LightPtrArray<Rtt::RecentProjectInfo>* listPointer)
	{
		if (listPointer)
		{
			listPointer->Clear();

			vector<string> Names;
			vector<string> Paths;
			if (ReadRecentDocs(Names, Paths))
			{
				for (int i = 0; i < Names.size(); i++)
				{
					// Create a recent project info object.
					RecentProjectInfo* infoPointer = new RecentProjectInfo();
					if (!infoPointer)
					{
						continue;
					}

					// Copy the project's folder name to the info object.
					String sTitle;
					sTitle.Append(Names[i].c_str());
					infoPointer->formattedString = sTitle;

					// Copy the project's "main.lua" file path to the info object.
					String sPath;
					sPath.Append(Paths[i].c_str());
					infoPointer->fullURLString = sPath;

					// Add the info object to the given collection.
					listPointer->Append(infoPointer);
				}
			}
		}
	}

} // namespace Rtt
