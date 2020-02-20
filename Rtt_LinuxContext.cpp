//////////////////////////////////////////////////////////////////////////////
//
// This file is part of the Corona game engine.
// For overview and more information on licensing please refer to README.md 
// Home page: https://github.com/coronalabs/corona
// Contact: support@coronalabs.com
//
//////////////////////////////////////////////////////////////////////////////

#include <limits.h>

#ifdef _WIN32
	#include <GL/glew.h>
#else
	#include <unistd.h>
	#include <dirent.h>
	#define _chdir chdir
	#include <pwd.h>
	#include <sys/types.h>
#endif

#include <string.h>
#include "Core/Rtt_Build.h"
#include "Core/Rtt_Time.h"
#include "Rtt_Runtime.h"
#include "Rtt_LuaContext.h"
#include "Core/Rtt_Types.h"
#include "Rtt_LinuxContext.h"
#include "Rtt_LinuxPlatform.h"
#include "Rtt_LinuxRuntimeDelegate.h"
#include "Rtt_LuaFile.h"
#include "Core/Rtt_FileSystem.h"
#include "Rtt_Archive.h"
#include "Display/Rtt_Display.h"
#include "Display/Rtt_DisplayDefaults.h"
#include "Rtt_KeyName.h"
#include "Rtt_Freetype.h"
#include "Rtt_LuaLibSimulator.h"
#include "Rtt_LinuxSimulatorView.h"

//#define Rtt_DEBUG_TOUCH 1

#define TIMER_ID wxID_HIGHEST + 11
#define ID_MENU_WELCOME wxID_HIGHEST + 12
#define ID_MENU_BUILD_ANDROID wxID_HIGHEST + 13
#define ID_MENU_RELAUNCH wxID_HIGHEST + 14
#define ID_MENU_CLOSE wxID_HIGHEST + 15
#define ID_MENU_BUILD_WEB wxID_HIGHEST + 16
#define ID_MENU_BUILD_LINUX wxID_HIGHEST + 17

using namespace Rtt;

//#include "wx/msgdlg.h"
#include "wx/menu.h"
#include "wx/dcclient.h"
#include "wx/app.h"
#include "wx/display.h"

static const char* getStartupPath(std::string* exeFileName)
{
#ifdef _WIN32
	return "Z:\\CoronaSimulator";
#else
	static char buf[ PATH_MAX + 1];
	ssize_t count = readlink( "/proc/self/exe", buf, PATH_MAX );
	buf[count] = 0;

	// remove file name
	char* filename = strrchr(buf, '/');
	Rtt_ASSERT(filename);
	if (exeFileName)
	{
		*exeFileName = filename + 1;
	}
	
	*filename = 0;
	return buf;
#endif
}

wxDEFINE_EVENT(eventOpenProject, wxCommandEvent);
wxDEFINE_EVENT(eventRelaunchProject, wxCommandEvent);
wxDEFINE_EVENT(eventWelcomeProject, wxCommandEvent);
wxDEFINE_EVENT(eventNewProject, wxCommandEvent);

namespace Rtt
{

	MouseListener::MouseListener(Runtime &runtime)
		: fRuntime(runtime)
		, fScaleX(1)
		, fScaleY(1)
	{
	}

	//
	//  touch
	//

	void MouseListener::TouchDown(int x, int y, int fid)
	{
		bool notifyMultitouch = fRuntime.Platform().GetDevice().DoesNotify(MPlatformDevice::kMultitouchEvent);
		x = (int)(x / fScaleX);
		y = (int)(y / fScaleY);

		// sanity check
		if (fStartPoint.find(fid) != fStartPoint.end() || (notifyMultitouch == false && fStartPoint.size() > 0))
		{
			return;
		}

		fStartPoint[fid] = pt(x, y);

		TouchEvent t((float)x, (float)y, (float)x, (float)y, TouchEvent::kBegan);

		// it must not be ZERO!
		t.SetId((void*)(fid + 1));

#if Rtt_DEBUG_TOUCH
		printf("TouchDown %d(%d, %d)\n", fid, x, y);
#endif

		if (notifyMultitouch)
		{
			MultitouchEvent t2(&t, 1);
			DispatchEvent(t2);
		}
		else
		{
			DispatchEvent(t);
		}
	}

	void		MouseListener::TouchMoved(int x, int y, int fid)
	{
		bool notifyMultitouch = fRuntime.Platform().GetDevice().DoesNotify(MPlatformDevice::kMultitouchEvent);

		x = (int)(x / fScaleX);
		y = (int)(y / fScaleY);

#if Rtt_DEBUG_TOUCH
		// Commented out b/c it's very noisy
		//printf("TouchMoved %d(%d, %d)\n", id, x, y);
#endif

		// sanity check
		if (fStartPoint.find(fid) == fStartPoint.end())
		{
			return;
		}

		TouchEvent t((float)x, (float)y, (float)fStartPoint[fid].x, (float)fStartPoint[fid].y, TouchEvent::kMoved);

		// it must not be ZERO!
		t.SetId((void*)(fid + 1));

		if (notifyMultitouch)
		{
			MultitouchEvent t2(&t, 1);
			DispatchEvent(t2);
		}
		else
		{
			DispatchEvent(t);
		}
	}

	void		MouseListener::TouchUp(int x, int y, int fid)
	{
		bool notifyMultitouch = fRuntime.Platform().GetDevice().DoesNotify(MPlatformDevice::kMultitouchEvent);

		x = (int)(x / fScaleX);
		y = (int)(y / fScaleY);

		// sanity check
		if (fStartPoint.find(fid) == fStartPoint.end())
		{
			return;
		}

		TouchEvent t((float)x, (float)y, (float)fStartPoint[fid].x, (float)fStartPoint[fid].y, TouchEvent::kEnded);

		// it must not be ZERO!
		t.SetId((void*)(fid + 1));

#if Rtt_DEBUG_TOUCH
		printf( "TouchUp %d(%d, %d)\n", fid, x, y );
#endif

		if (notifyMultitouch)
		{
			MultitouchEvent t2(&t, 1);
			DispatchEvent(t2);
		}
		else
		{
			DispatchEvent(t);
		}

		// Dispatch a "tap" event if mouse button was released in the same position it was pressed in.
		if (fStartPoint[fid].x == x && fStartPoint[fid].y == y)
		{
			Rtt::TapEvent event(x, y, fid + 1);
			DispatchEvent(event);
		}

		fStartPoint.erase(fid);
	}

	void MouseListener::DispatchEvent(const MEvent& e) const
	{
		fRuntime.DispatchEvent(e);
	}

	KeyListener::KeyListener(Runtime &runtime)
		: fRuntime(runtime)
	{
		fKeyName['A'] = KeyName::kA;
		fKeyName['B'] = KeyName::kB;
		fKeyName['C'] = KeyName::kC;
		fKeyName['D'] = KeyName::kD;
		fKeyName['E'] = KeyName::kE;
		fKeyName['F'] = KeyName::kF;
		fKeyName['G'] = KeyName::kG;
		fKeyName['H'] = KeyName::kH;
		fKeyName['I'] = KeyName::kI;
		fKeyName['J'] = KeyName::kJ;
		fKeyName['K'] = KeyName::kK;
		fKeyName['L'] = KeyName::kL;
		fKeyName['M'] = KeyName::kM;
		fKeyName['N'] = KeyName::kN;
		fKeyName['O'] = KeyName::kO;
		fKeyName['P'] = KeyName::kP;
		fKeyName['Q'] = KeyName::kQ;
		fKeyName['R'] = KeyName::kR;
		fKeyName['S'] = KeyName::kS;
		fKeyName['T'] = KeyName::kT;
		fKeyName['U'] = KeyName::kU;
		fKeyName['V'] = KeyName::kV;
		fKeyName['W'] = KeyName::kW;
		fKeyName['X'] = KeyName::kX;
		fKeyName['Y'] = KeyName::kY;
		fKeyName['Z'] = KeyName::kZ;
		fKeyName['0'] = KeyName::k0;
		fKeyName['1'] = KeyName::k1;
		fKeyName['2'] = KeyName::k2;
		fKeyName['3'] = KeyName::k3;
		fKeyName['4'] = KeyName::k4;
		fKeyName['5'] = KeyName::k5;
		fKeyName['6'] = KeyName::k6;
		fKeyName['7'] = KeyName::k7;
		fKeyName['8'] = KeyName::k8;
		fKeyName['9'] = KeyName::k9;
		fKeyName[340] = KeyName::kF1;
		fKeyName[341] = KeyName::kF2;
		fKeyName[342] = KeyName::kF3;
		fKeyName[343] = KeyName::kF4;
		fKeyName[344] = KeyName::kF5;
		fKeyName[345] = KeyName::kF6;
		fKeyName[346] = KeyName::kF7;
		fKeyName[347] = KeyName::kF8;
		fKeyName[348] = KeyName::kF9;
		fKeyName[349] = KeyName::kF10;
		fKeyName[350] = KeyName::kF11;
		fKeyName[351] = KeyName::kF12;
		fKeyName[32] = KeyName::kSpace;
		fKeyName[307] = KeyName::kLeftAlt;
		//		fKeyName["Right Alt"] = KeyName::kRightAlt;
		fKeyName[308] = KeyName::kLeftControl;
		//		fKeyName["Right Ctrl"] = KeyName::kRightControl;
		fKeyName[306] = KeyName::kLeftShift;
		//		fKeyName["Right Shift"] = KeyName::kRightShift;
		fKeyName[393] = KeyName::kLeftCommand;
		//		fKeyName["Right Windows"] = KeyName::kRightCommand;
		fKeyName[315] = KeyName::kUp;
		fKeyName[317] = KeyName::kDown;
		fKeyName[314] = KeyName::kLeft;
		fKeyName[316] = KeyName::kRight;
		fKeyName[313] = KeyName::kHome;
		fKeyName[312] = KeyName::kEnd;
		fKeyName[366] = KeyName::kPageUp;
		fKeyName[367] = KeyName::kPageDown;
		fKeyName[322] = KeyName::kInsert;
		fKeyName[127] = KeyName::kDeleteForward;
		fKeyName[8] = KeyName::kDeleteBack; //kBack;
		fKeyName[47] = KeyName::kForwardSlash;
		fKeyName[92] = KeyName::kBackSlash;
		fKeyName['='] = KeyName::kPlus;
		fKeyName['-'] = KeyName::kMinus;
		fKeyName[','] = ",";
		fKeyName['.'] = KeyName::kPeriod;
		fKeyName['`'] = "`";
		fKeyName[';'] = ";";
		fKeyName['['] = KeyName::kLeftBracket;
		fKeyName[']'] = KeyName::kRightBracket;
		fKeyName['\''] = KeyName::kApostrophe;
		fKeyName[27] = KeyName::kEscape;
		fKeyName[13] = KeyName::kEnter;
		fKeyName[321] = KeyName::kPrintScreen;
		fKeyName[310] = KeyName::kMediaPause;
		fKeyName[365] = KeyName::kScrollLock;
		fKeyName[384] = KeyName::kNumPad0;
		fKeyName[382] = KeyName::kNumPad1;
		fKeyName[379] = KeyName::kNumPad2;
		fKeyName[381] = KeyName::kNumPad3;
		fKeyName[376] = KeyName::kNumPad4;
		fKeyName[383] = KeyName::kNumPad5;
		fKeyName[378] = KeyName::kNumPad6;
		fKeyName[375] = KeyName::kNumPad7;
		fKeyName[377] = KeyName::kNumPad8;
		fKeyName[380] = KeyName::kNumPad9;
		fKeyName[392] = KeyName::kNumPadDivide;
		fKeyName[387] = KeyName::kNumPadMultiply;
		fKeyName[390] = KeyName::kNumPadSubtract;
		fKeyName[388] = KeyName::kNumPadAdd;
		fKeyName[370] = KeyName::kNumPadEnter;
		fKeyName[385] = KeyName::kNumPadComma;
		//		fKeyName["Keypad ."] = KeyName::kNumPadPeriod;
		//		fKeyName["Keypad ("] = KeyName::kNumPadLeftParentheses;
		//		fKeyName["Keypad )"] = KeyName::kNumPadRightParentheses;
		fKeyName[364] = KeyName::kNumLock;
		fKeyName[9] = KeyName::kTab;
		fKeyName[426] = KeyName::kVolumeUp;
		fKeyName[425] = KeyName::kVolumeDown;
		fKeyName[424] = KeyName::kVolumeMute;
		//		fKeyName["Left GUI"] = KeyName::kLeftCommand;		// web
		//		fKeyName["Right GUI"] = KeyName::kRightCommand;		// web
		fKeyName[311] = KeyName::kCapsLock;
		//		fKeyName["ZoomIn"] = KeyName::kZoomIn;
		//		fKeyName["ZoomOut"] = KeyName::kZoomOut;
		//		fKeyName["Break"] = KeyName::kBreak;
		fKeyName[309] = KeyName::kMenu;
		//		fKeyName["Application"] = KeyName::kMenu;		// web
	}

	void KeyListener::notifyCharEvent(wxKeyEvent& e)
	{
		wxChar unicodeCharacter = e.GetUnicodeKey();
		if (unicodeCharacter != WXK_NONE)
		{
			wxCharBuffer utf8Buffer = wxString(e.GetUnicodeKey()).ToUTF8();
			const char *utf8Character = utf8Buffer.data();
			if (strlen(utf8Character) > 1 || isprint(utf8Character[0])) {
				CharacterEvent characterEvent(NULL, utf8Character);
				fRuntime.DispatchEvent(characterEvent);
			}
		}
	}

	void KeyListener::notifyKeyEvent(wxKeyEvent& e, bool down)
	{
		int mod = e.GetModifiers();
		bool isNumLockDown = false; // fixme
		bool isCapsLockDown = false; // fixme
		bool isShiftDown = mod & wxMOD_SHIFT ? true : false;
		bool isCtrlDown = mod & wxMOD_CONTROL ? true : false;
		bool isAltDown = mod & wxMOD_ALT ? true : false;
		bool isCommandDown = mod & wxMOD_WIN ? true : false;

		int	keycode = e.GetKeyCode();
		PlatformInputDevice *dev = NULL;

		auto it = fKeyName.find(keycode);
		const char* keyName = it == fKeyName.end() ? KeyName::kUnknown : it->second.c_str();

		if (down == false && (strcmp(keyName, "r") == 0) && isCtrlDown)
		{
			// relaunch
			wxCommandEvent ev(eventRelaunchProject);
			wxPostEvent(wxGetApp().getFrame(), ev);
		}
		else
		if (down == false && (strcmp(keyName, "w") == 0) && isCtrlDown)
		{
			// close
			wxCommandEvent ev(eventRelaunchProject);
			wxPostEvent(wxGetApp().getFrame(), ev);
		}

		KeyEvent ke(dev, down ? KeyEvent::kDown : KeyEvent::kUp, keyName, keycode, isShiftDown, isAltDown, isCtrlDown, isCommandDown);
		fRuntime.DispatchEvent(ke);
	}

	CoronaAppContext::CoronaAppContext(const char* path)
		: fRuntime(NULL)
		, fRuntimeDelegate(new LinuxRuntimeDelegate())
		, fMouseListener(NULL)
		, fKeyListener(NULL)
		, fPlatform(NULL)
		, fTouchDeviceExist(false)
		, fMode("normal")
		, fIsDebApp(false)
		, fSimulator(NULL)
		, fIsStarted(false)
	{

	std::string exeFileName;
	#ifdef _WIN32
		const char* homedir = getenv("USERPROFILE");
		const char* appPath = "Z:\\CoronaSimulator\\welcomescreen";
//		const char* appPath = "Z:\\tmp\\clock";
	#else
		struct passwd* pw = getpwuid(getuid());
		const char* homedir = pw->pw_dir;

		const char* appPath = getStartupPath(&exeFileName);
		//const char* appPath = "/home/vitaly/Clock";
	#endif

		// override appPath if arg isn't NULL
		if (path && *path != 0)
		{
			appPath = path;
		}

		// set app name
		if (strcmp(appPath, "/usr/bin") == 0)		// deb ?
		{
			// for .deb app the appName is exe file name
			fAppName = exeFileName;
		}
		else
		{
			const char* slash = strrchr(appPath, '/');
			if (slash)
			{
				fAppName = slash + 1;
			}
			else
			{
				slash = strrchr(appPath, '\\');
				if (slash)
				{
					fAppName = slash + 1;
				}
			}
		}
		Rtt_ASSERT(fAppName.size() > 0);
		std::string startDir(appPath);

		fSaveFolder = homedir;
		fSaveFolder += LUA_DIRSEP;
		fSaveFolder += "Documents";
		fSaveFolder += LUA_DIRSEP;
		fSaveFolder += "Corona Built Apps";


		//
		// tar.gz app ?
		//

		std::string assetsDir = startDir + "/resource.car";
		Rtt_LogException("Checking %s\n", assetsDir.c_str());
		if (Rtt_FileExists(assetsDir.c_str()))
		{
			fPathToApp = startDir;
			return;
		}

		// next check if main.lua exists
		assetsDir = startDir + "/main.lua";
		Rtt_LogException("Checking %s\n", assetsDir.c_str());
		if (Rtt_FileExists(assetsDir.c_str()))
		{
			fPathToApp = startDir;
			return;
		}

		//
		// look for DEB package ?
		//

		startDir = "/usr/share/corona/";
		startDir += fAppName;

		assetsDir = startDir + "/resource.car";
		Rtt_LogException("Checking %s\n", assetsDir.c_str());
		if (Rtt_FileExists(assetsDir.c_str()))
		{
			fPathToApp = startDir;
			fIsDebApp = true;
			return;
		}

		// next check if main.lua exists
		assetsDir = startDir + "/main.lua";
		Rtt_LogException("Checking %s\n", assetsDir.c_str());
		if (Rtt_FileExists(assetsDir.c_str()))
		{
			fPathToApp = startDir;
			fIsDebApp = true;
			return;
		}

		//
		// look for welcomescereen, Simulator ?
		//
		startDir = getStartupPath(NULL);
		startDir += "/Resources/homescreen";
		assetsDir = startDir + "/main.lua";
		Rtt_LogException("Checking %s\n", assetsDir.c_str());
		if (Rtt_FileExists(assetsDir.c_str()))
		{
			fAppName = "homescreen";
			fPathToApp = startDir;
			fIsDebApp = false;
			return;
		}

		Rtt_LogException("Failed to find app\n");
		Rtt_ASSERT(0);
	}

	CoronaAppContext::~CoronaAppContext()
	{
		close();
	}

	void CoronaAppContext::close()
	{
		delete fMouseListener;
		fMouseListener = NULL;
		delete fKeyListener;
		fKeyListener = NULL;
		delete fRuntime;
		fRuntime = NULL;
		delete fRuntimeDelegate;
		fRuntimeDelegate = NULL;
		delete fPlatform;
		fPlatform = NULL;
		delete fSimulator;
		fSimulator = NULL;

		setGlyphProvider(NULL);
	}

	bool		CoronaAppContext::Init()
	{
		// Initializes all available image handlers
		wxInitAllImageHandlers();

		const char *homeDir = NULL;
#ifdef _WIN32
		homeDir = getenv("USERPROFILE");
#else
		if ((homeDir = getenv("HOME")) == NULL) 
		{
			homeDir = getpwuid(getuid())->pw_dir;
		}
#endif

		std::string appDir = fPathToApp;
		if (fIsDebApp)
		{
			appDir = homeDir;
			appDir += "/.corona/";
			appDir += fAppName;
		}
		Rtt_LogException("sandbox: %s\n", appDir.c_str());
		Rtt_LogException("assets: %s\n", fPathToApp.c_str());

		std::string documentsDir = appDir;
		std::string temporaryDir = appDir;
		temporaryDir += "/temporaryDir";
		std::string cachesDir = appDir;
		cachesDir += "/cachesDir";
		std::string systemCachesDir = homeDir;
		systemCachesDir += "/.corona/systemCachesDir";

		if (Rtt_IsDirectory(documentsDir.c_str()) == false)
		{
			Rtt_MakeDirectory(documentsDir.c_str());
		}
		if (Rtt_IsDirectory(systemCachesDir.c_str()) == false)
		{
			Rtt_MakeDirectory(systemCachesDir.c_str());
		}

		// the first start ? copy sqlite3 databases
		std::vector<std::string> docFiles = Rtt_ListFiles(documentsDir.c_str());
		if (docFiles.size() == 0 && documentsDir.size() > 0 && fPathToApp.size() > 0)
		{
			std::string cmd = "cp ";
			cmd += fPathToApp;
			cmd += "/*.db ";
			cmd += documentsDir;
			system(cmd.c_str());
		}

		Rtt_DeleteDirectory(temporaryDir.c_str());
		if (Rtt_IsDirectory(temporaryDir.c_str()) == false)
		{
			Rtt_MakeDirectory(temporaryDir.c_str());
		}

		if (Rtt_IsDirectory(cachesDir.c_str()) == false)
		{
			Rtt_MakeDirectory(cachesDir.c_str());
		}

		setGlyphProvider(new glyph_freetype_provider(fPathToApp.c_str()));
		fPlatform = new LinuxPlatform(fPathToApp.c_str(), documentsDir.c_str(), temporaryDir.c_str(), cachesDir.c_str(), systemCachesDir.c_str(), getStartupPath(NULL));

		fRuntime = new LinuxRuntime(*fPlatform, NULL);
		fRuntime->SetDelegate(fRuntimeDelegate);
		fRuntime->SetProperty(Runtime::kLinuxMaskSet, true);

		// For debugging, use main.lua if it exists in the app folder
		{
			std::string main_lua = fPathToApp.c_str();
			main_lua += LUA_DIRSEP;
			main_lua += "main.lua";
			FILE* fi = fopen(main_lua.c_str(), "r");
			if (fi)
			{
				fclose(fi);
				fRuntime->SetProperty(Runtime::kLinuxMaskSet | Runtime::kIsApplicationNotArchived | Runtime::kShouldVerifyLicense, true);
			}
		}

		// settings

		std::string orientation;
		int w = 0;
		int h = 0;
		fRuntime->readSettings(&w, &h, &orientation, &fTitle, &fMode);
		if (orientation == "landscapeRight")
		{
			fRuntimeDelegate->fOrientation = DeviceOrientation::kSidewaysRight;	// bottom of device is to the right
			if (w > 0 && h > 0)
			{
				fRuntimeDelegate->fContentWidth = w;
				fRuntimeDelegate->fContentHeight = h;
			}
			else
			{
				// no valid defaultViewWidth & defaultViewHeight in 'build.settings', default values of fWidth & fHeight for Portrait
				// use swapped default settings 
				Swap(fRuntimeDelegate->fContentWidth, fRuntimeDelegate->fContentHeight);
			}
		}
		else if (orientation == "landscapeLeft")
		{
			fRuntimeDelegate->fOrientation = DeviceOrientation::kSidewaysLeft; 	// bottom of device is to the left
			if (w > 0 && h > 0)
			{
				fRuntimeDelegate->fContentWidth = w;
				fRuntimeDelegate->fContentHeight = h;
			}
			else
			{
				// no valid defaultViewWidth & defaultViewHeight in 'build.settings', default values of fWidth & fHeight for Portrait
				// use swapped default settings 
				Swap(fRuntimeDelegate->fContentWidth, fRuntimeDelegate->fContentHeight);
			}
		}
		else if (orientation == "portrait")
		{
			fRuntimeDelegate->fOrientation = DeviceOrientation::kUpright;	// bottom of device is at the bottom
			if (w > 0 && h > 0)
			{
				fRuntimeDelegate->fContentWidth = w;
				fRuntimeDelegate->fContentHeight = h;
			}
			else
			{
				// no valid defaultViewWidth & defaultViewHeight in 'build.settings', default values of fWidth & fHeight for Portrait
				// use default settings 
			}
		}
		else if (orientation == "portraitUpsideDown")
		{
			fRuntimeDelegate->fOrientation = DeviceOrientation::kUpsideDown;	// bottom of device is at the top
			if (w > 0 && h > 0)
			{
				fRuntimeDelegate->fContentWidth = w;
				fRuntimeDelegate->fContentHeight = h;
			}
			else
			{
				// no valid defaultViewWidth & defaultViewHeight in 'build.settings', default values of fWidth & fHeight for Portrait
				// use default settings 
			}
		}
		else
		{
			//Rtt_LogException("Unsupported orientation: '%s'", orientation.c_str());
		}

		bool fullScreen = fMode == "maximized";
		if (fullScreen)
		{
			wxDisplay screen(0);
			wxRect rect = screen.GetGeometry();
	//		fWidth = rect.GetWidth();
	//		fHeight = rect.GetHeight() - 1;
//			Rtt_LogException("Full Screen mode: %dx%d\n", fWidth, fHeight);
		}

		fPlatform->setWindow(this);

		fMouseListener = new MouseListener(*fRuntime);
		fKeyListener = new KeyListener(*fRuntime);

		// Initialize Joystick Support:
#ifndef _WIN32
		LinuxInputDeviceManager& deviceManager = (LinuxInputDeviceManager&) fPlatform->GetDevice().GetInputDeviceManager();
		deviceManager.init();
#endif

		return fullScreen;
	}

	bool CoronaAppContext::loadApp(MyGLCanvas* canvas)
	{
		fCanvas = canvas;

		if (Runtime::kSuccess != fRuntime->LoadApplication(Runtime::kLinuxLaunchOption, fRuntimeDelegate->fOrientation))
		{
			delete fRuntime;
			delete fPlatform;
			return false;
		}

		// add Resources to LUA_PATH
		std::string luapath;
		luapath = getenv("LUA_PATH");
		luapath += getStartupPath(NULL);
		luapath += "/Resources";		
		luapath += "/?.lua;";
		setenv("LUA_PATH", luapath.c_str(), true);
		
		if (fRuntimeDelegate->fOrientation == DeviceOrientation::kSidewaysRight || fRuntimeDelegate->fOrientation == DeviceOrientation::kSidewaysLeft)
		{
	//		Swap(fRuntimeDelegate->fContentWidth, fRuntimeDelegate->fContentHeight);
		}

#ifdef Rtt_SIMULATOR
		fSimulator = new LinuxSimulatorServices();
		lua_State *luaStatePointer = fRuntime->VMContext().L();
		lua_pushlightuserdata(luaStatePointer, fSimulator);
		Rtt::LuaContext::RegisterModuleLoader(luaStatePointer, Rtt::LuaLibSimulator::kName, Rtt::LuaLibSimulator::Open, 1);
#endif

		return true;
	}

	void CoronaAppContext::flush()
	{
		fCanvas->Refresh(false);
	}

	void CoronaAppContext::pause()
	{
		if (fRuntime->IsSuspended() == false)
		{
			fRuntime->Suspend();
		}
	}

	void CoronaAppContext::resume()
	{
		if (GetRuntime()->IsSuspended())
		{
			fRuntime->DispatchEvent(ResizeEvent());
			fRuntime->Resume();
		}
	}

	bool LinuxRuntime::readTable(lua_State *L, const char* table, int* w, int* h, std::string* title, std::string* mode) const
	{
		bool rc = false;
		int top = lua_gettop(L);

		lua_getfield(L, -1, table);
		if (lua_istable(L, -1))
		{
			rc = true;
			lua_getfield(L, -1, "defaultViewWidth");
			if ((!lua_isnil(L, -1)) && (lua_isnumber(L, -1)))
			{
				*w = lua_tointeger(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "defaultViewHeight");
			if ((!lua_isnil(L, -1)) && (lua_isnumber(L, -1)))
			{
				*h = lua_tointeger(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "defaultMode");
			if ((!lua_isnil(L, -1)) && (lua_isstring(L, -1)))
			{
				*mode = lua_tostring(L, -1);
			}
			lua_pop(L, 1);

			lua_getfield(L, -1, "titleText");
			if (lua_istable(L, -1))
			{
				lua_getfield(L, -1, "default");
				if ((!lua_isnil(L, -1)) && (lua_isstring(L, -1)))
				{
					*title = lua_tostring(L, -1);
				}
				lua_pop(L, 1);		// remove default
			}
			lua_pop(L, 1);		// remove titleText
		}

		lua_settop(L, top);
		return rc;
	}


	void LinuxRuntime::readSettings(int* w, int* h, std::string* orientation, std::string* title, std::string* mode)
	{
		Rtt_ASSERT(w != NULL && h != NULL);

		lua_State *L = VMContext().L();
		int status = 0;
		const MPlatform& p = Platform();
		int top = lua_gettop(L);

		//
		// read build.settings
		//
		const char kBuildSettings[] = "build.settings";
		String filePath(&p.GetAllocator());
		p.PathForFile(kBuildSettings, MPlatform::kResourceDir, MPlatform::kTestFileExists, filePath);
		const char *path = filePath.GetString();
		if (path)
		{
			status = VMContext().DoFile(path, 0, true);
		}
		else
		{
			// other cases assume a non-zero status means there's an error msg on the stack
			// so push a "fake" error msg on the stack so we are consistent with those cases
			lua_pushnil(L);
		}
		lua_pop(L, 1);		// remove DoFile result

		if (status == 0)
		{
			lua_getglobal(L, "settings"); // settings
			if (lua_istable(L, -1))
			{
				lua_getfield(L, -1, "orientation"); // settings.orientation
				if (lua_istable(L, -1))
				{
					lua_getfield(L, -1, "default");
					if ((!lua_isnil(L, -1)) && (lua_isstring(L, -1)))
					{
						*orientation = lua_tostring(L, -1);
					}
					lua_pop(L, 1);
				}
				lua_pop(L, 1);		// remove orientation

				// first try settings from 'web' table
				if (readTable(L, "linux", w, h, title, mode) == false)
				{
					// next try settings from 'window' table
					readTable(L, "window", w, h, title, mode);
				}
			}
			lua_pop(L, 1);		// remove settings
		}
		else
			if (status == 3)
			{
				Rtt_LogException("Invalid build.settings file\n");
			}
			else
			{
				Rtt_LogException("Failed to read build.settings file\n");
			}
		lua_settop(L, top);
	}

	int jsSystemEvent::Push(lua_State *L) const
	{
		if (Rtt_VERIFY(VirtualEvent::Push(L)))
		{
			lua_pushstring(L, fEventName.c_str());
			lua_setfield(L, -2, kTypeKey);
		}
		return 1;
	}

}


//
// wxWidgets
//

MyApp::MyApp()
{
}

MyApp::~MyApp()
{
}


// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	if (wxApp::OnInit() == true)
	{
		// create the main application window
		fFrame = new MyFrame();
		bool fullScreen = false;

		// As of October 2015 GTK+ needs the frame to be shown before we call SetCurrent()
		
		//Test if OGL context could be created.
		if (fFrame->m_mycanvas->OglCtxAvailable() == true)
		{
			if (fullScreen)
			{
				fFrame->ShowFullScreen(true);
			}
			else
			{
				fFrame->Show(true);
			}
			return true;
		}
	}
	return false;
}

wxWindow* MyApp::getParent()
{
#ifdef _WIN32
	return getCanvas();
#else
	return getFrame();
#endif		
}

LinuxPlatform* MyApp::getPlatform() const
{
	return fFrame->getContext()->getPlatform();
}

//
// main frame
//

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
	EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
	EVT_MENU(wxID_OPEN, MyFrame::OnOpenFileDialog)
	EVT_MENU(ID_MENU_WELCOME, MyFrame::OnOpenWelcome)
	EVT_MENU(ID_MENU_BUILD_ANDROID, MyFrame::OnBuildAndroid)
	EVT_MENU(ID_MENU_BUILD_WEB, MyFrame::OnBuildWeb)
	EVT_MENU(ID_MENU_BUILD_LINUX, MyFrame::OnBuildLinux)
	EVT_MENU(ID_MENU_RELAUNCH, MyFrame::OnRelaunch)
	EVT_MENU(ID_MENU_CLOSE, MyFrame::OnOpenWelcome)
	
	EVT_COMMAND(wxID_ANY, eventOpenProject, MyFrame::OnOpen)
	EVT_COMMAND(wxID_ANY, eventRelaunchProject, MyFrame::OnRelaunch)
	EVT_COMMAND(wxID_ANY, eventWelcomeProject, MyFrame::OnOpenWelcome)

wxEND_EVENT_TABLE()

MyFrame::MyFrame()
	: wxFrame(NULL, wxID_ANY, "", wxDefaultPosition, wxSize(0,0), wxCAPTION | wxCLOSE_BOX)
	, m_mycanvas(NULL)
	, fContext(NULL)
	, fMenuMain(NULL)
	, fMenuProject(NULL)
	, fWatcher(NULL)
{
	wxGLAttributes vAttrs;
	vAttrs.PlatformDefaults().Defaults().EndList();
	bool accepted = wxGLCanvas::IsDisplaySupported(vAttrs);
	if (accepted == false)
	{
		// Try again without sample buffers
		vAttrs.Reset();
		vAttrs.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();

		accepted = wxGLCanvas::IsDisplaySupported(vAttrs);
		if (accepted == false)
		{
			Rtt_LogException("Failed to init OpenGL");
			return;
		}
	}

	createMenus();
	m_mycanvas = new MyGLCanvas(this, vAttrs);

	SetWindowStyle(wxCAPTION | wxCLOSE_BOX);
}

MyFrame::~MyFrame()
{
	delete fWatcher;
	delete m_mycanvas;

	fContext->close();
	delete fContext;

	SetMenuBar(NULL);
	delete fMenuMain;
	delete fMenuProject;
}

void MyFrame::watchFolder(const char* path, const char* appName)
{
	// 	wxFileSystemWatcher
	if (fWatcher == NULL)
	{
		fWatcher = new wxFileSystemWatcher();
		fWatcher->SetOwner(this);
		Connect(wxEVT_FSWATCHER, wxFileSystemWatcherEventHandler(MyFrame::OnFileSystemEvent));
	}

	if (strcmp(appName, "homescreen") == 0)
	{
		// do not watch main screen folder
		return;
	}


	wxFileName fn = wxFileName::DirName(path);
	fn.DontFollowLink();

	fWatcher->RemoveAll();
	fWatcher->Add(fn);
}

void MyFrame::resetSize()
{
	int w = fContext->getWidth();
	int h = fContext->getHeight();
	SetClientSize(wxSize(w, h));
	Refresh(false);
}

void MyFrame::createMenus()
{
	wxMenuItem* mi;
	{
		fMenuMain = new wxMenuBar();

		// File Menu
		wxMenu* m_pFileMenu = new wxMenu();
		mi = m_pFileMenu->Append(wxID_NEW, _T("&New Project	\tCtrl-N")); mi->Enable(false);
		mi = m_pFileMenu->Append(wxID_OPEN, _T("&Open Project	\tCtrl-O"));
		m_pFileMenu->AppendSeparator();
		mi = m_pFileMenu->Append(wxID_SAVE, _T("&Relaunch Last Project	\tCtrl-R"));
		m_pFileMenu->AppendSeparator();
		mi = m_pFileMenu->Append(wxID_PREFERENCES, _T("&Preferences...")); mi->Enable(false);
		m_pFileMenu->AppendSeparator();
		mi = m_pFileMenu->Append(wxID_EXIT, _T("&Exit"));
		fMenuMain->Append(m_pFileMenu, _T("&File"));

		// About menu
		wxMenu* m_pHelpMenu = new wxMenu();
		mi = m_pHelpMenu->Append(wxID_HELP_CONTENTS, _T("&Online Documentation...")); mi->Enable(false);
		mi = m_pHelpMenu->Append(wxID_HELP_INDEX, _T("&Sample projects...")); mi->Enable(false);
		mi = m_pHelpMenu->Append(wxID_ABOUT, _T("&About Simulator...")); mi->Enable(false);
		fMenuMain->Append(m_pHelpMenu, _T("&Help"));
	}

	//
	// project's menu
	//
	{
		fMenuProject = new wxMenuBar();

		// File Menu
		wxMenu* m_pFileMenu = new wxMenu();
		mi = m_pFileMenu->Append(wxID_NEW, _T("&New Project	\tCtrl-N")); mi->Enable(false);
		mi = m_pFileMenu->Append(wxID_OPEN, _T("&Open Project	\tCtrl-O"));
		m_pFileMenu->AppendSeparator();

		wxMenu* m_pBuildMenu = new wxMenu();
		mi = m_pBuildMenu->Append(ID_MENU_BUILD_ANDROID, _T("Android	\tCtrl-B"));
		mi = m_pBuildMenu->Append(ID_MENU_BUILD_WEB, _T("HTML5	\tCtrl-Alt-B"));
		mi = m_pBuildMenu->Append(wxID_SAVE, _T("&iOS"));	mi->Enable(false);
		mi = m_pBuildMenu->Append(ID_MENU_BUILD_LINUX, _T("&Linux	\tCtrl-Shift-Alt-B"));
		mi = m_pBuildMenu->Append(wxID_SAVE, _T("&Windows"));	mi->Enable(false);
		mi = m_pFileMenu->AppendSubMenu(m_pBuildMenu, _T("&Build"));

		mi = m_pFileMenu->Append(wxID_SAVE, _T("&Open In Editor	\tCtrl-Shift-O"));
		mi = m_pFileMenu->Append(wxID_SAVE, _T("&Show Project Files")); mi->Enable(false);
		mi = m_pFileMenu->Append(wxID_SAVE, _T("&Show Project Sandbox")); mi->Enable(false);
		m_pFileMenu->AppendSeparator();
		mi = m_pFileMenu->Append(wxID_SAVE, _T("&Clear Project Sandbox")); mi->Enable(false);
		m_pFileMenu->AppendSeparator();
		mi = m_pFileMenu->Append(ID_MENU_RELAUNCH, _T("Relaunch	\tCtrl-R"));
		mi = m_pFileMenu->Append(ID_MENU_CLOSE, _T("Close Project	\tCtrl-W"));
		m_pFileMenu->AppendSeparator();
		mi = m_pFileMenu->Append(wxID_PREFERENCES, _T("&Preferences...")); mi->Enable(false);
		m_pFileMenu->AppendSeparator();
		mi = m_pFileMenu->Append(wxID_EXIT, _T("&Exit"));
		fMenuProject->Append(m_pFileMenu, _T("&File"));

		// hardware menu
		wxMenu* m_pHardwareMenu = new wxMenu();
		mi = m_pHardwareMenu->Append(wxID_HELP_CONTENTS, _T("&Rotate Left")); mi->Enable(false);
		mi = m_pHardwareMenu->Append(wxID_HELP_INDEX, _T("&Rotate Right")); mi->Enable(false);
		mi = m_pHardwareMenu->Append(wxID_ABOUT, _T("&Shake")); mi->Enable(false);
		m_pHardwareMenu->AppendSeparator();
		mi = m_pHardwareMenu->Append(wxID_ABOUT, _T("&Back")); mi->Enable(false);
		m_pHardwareMenu->AppendSeparator();
		mi = m_pHardwareMenu->Append(wxID_ABOUT, _T("&Suspend")); mi->Enable(false);
		fMenuProject->Append(m_pHardwareMenu, _T("&Hardware"));

		// View menu
		wxMenu* m_pViewMenu = new wxMenu();
		mi = m_pViewMenu->Append(wxID_HELP_CONTENTS, _T("&Zoom In")); mi->Enable(false);
		mi = m_pViewMenu->Append(wxID_HELP_INDEX, _T("&Zoom Out")); mi->Enable(false);
		m_pViewMenu->AppendSeparator();
		mi = m_pViewMenu->Append(wxID_ABOUT, _T("&View As")); mi->Enable(false);
		m_pViewMenu->AppendSeparator();
		mi = m_pViewMenu->Append(ID_MENU_WELCOME, _T("&Welcome Screen"));
		mi = m_pViewMenu->Append(wxID_ABOUT, _T("&Console")); mi->Enable(false);
		fMenuProject->Append(m_pViewMenu, _T("&View"));

		// About menu
		wxMenu* m_pHelpMenu = new wxMenu();
		mi = m_pHelpMenu->Append(wxID_HELP_CONTENTS, _T("&Online Documentation...")); mi->Enable(false);
		mi = m_pHelpMenu->Append(wxID_HELP_INDEX, _T("&Sample projects...")); mi->Enable(false);
		mi = m_pHelpMenu->Append(wxID_ABOUT, _T("&About Simulator...")); mi->Enable(false);
		fMenuProject->Append(m_pHelpMenu, _T("&Help"));
	}
}

void MyFrame::setMenu(const char* appPath)
{
#ifdef Rtt_SIMULATOR
	const std::string& appName = getContext()->getAppName();
	if (appName == "homescreen")
	{
		SetMenuBar(fMenuMain);
	}
	else
	{
		SetMenuBar(fMenuProject);
	}
#endif
}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
}

void MyFrame::OnFileSystemEvent(wxFileSystemWatcherEvent& event)
{
	int type = event.GetChangeType();
	const wxFileName& f = event.GetPath();
	wxString fn = f.GetFullName();
	wxString fp = f.GetFullPath();
	wxString ext = f.GetExt();
	switch (type)
	{
		case wxFSW_EVENT_CREATE:
		case wxFSW_EVENT_DELETE:
		case wxFSW_EVENT_RENAME:
		case wxFSW_EVENT_MODIFY:
		{
			if (ext == "lua")
			{
				// relaunch
				wxCommandEvent ev(eventRelaunchProject);
				wxPostEvent(wxGetApp().getFrame(), ev);
			}
			break;
		}
		default:
			break;
	}
}

// open home screen
void MyFrame::OnOpenWelcome(wxCommandEvent& ev)
{
	std::string path = getStartupPath(NULL);
	path += "/Resources/homescreen/main.lua";
	
	wxCommandEvent eventOpen(eventOpenProject); 
	eventOpen.SetString(path.c_str());
	wxPostEvent(this, eventOpen);
}

void MyFrame::OnBuildAndroid(wxCommandEvent& ev)
{
	LinuxSimulatorView::OnBuildForAndroid(getContext());
}

void MyFrame::OnBuildWeb(wxCommandEvent& ev)
{
	LinuxSimulatorView::OnBuildForWeb(getContext());
}

void MyFrame::OnBuildLinux(wxCommandEvent& ev)
{
	LinuxSimulatorView::OnBuildForLinux(getContext());
}

void MyFrame::OnOpenFileDialog(wxCommandEvent& event)
{
	wxFileDialog openFileDialog(wxGetApp().getParent(), _("Open"), "", "", "Simulator Files (main.lua)|main.lua", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}

	wxString path = openFileDialog.GetPath();
	if (Rtt_FileExists(path.c_str()) == false)
	{
		return;
	}

	// open project
	wxCommandEvent eventOpen(eventOpenProject);
	eventOpen.SetString(path.c_str());
	wxPostEvent(this, eventOpen);
}

void MyFrame::OnRelaunch(wxCommandEvent& event)
{
	if (fAppPath.size() > 0)
	{
		delete fContext;
		fContext = new CoronaAppContext(fAppPath.c_str());
		_chdir(fContext->getAppPath());

		watchFolder(fContext->getAppPath(), fContext->getAppName().c_str());

		bool fullScreen = fContext->Init();
		fContext->loadApp(m_mycanvas);
		resetSize();
		m_mycanvas->fContext = fContext;
		fContext->setCanvas(m_mycanvas);

		SetTitle(fContext->getTitle().c_str());
		setMenu(fAppPath.c_str());
		m_mycanvas->startTimer(1000.0f / (float)fContext->getFPS());
	}
}

void MyFrame::OnOpen(wxCommandEvent& event)
{
	wxString path = event.GetString();
	path = path.SubString(0, path.size() - 10);	// without main.lua

	delete fContext;
	fContext = new CoronaAppContext(path.c_str());
	_chdir(fContext->getAppPath());

	watchFolder(fContext->getAppPath(), fContext->getAppName().c_str());

	if (fContext->getAppName() != "homescreen")
	{
		fAppPath = fContext->getAppPath();		// save for relaunch
	}

	bool fullScreen = fContext->Init();
	fContext->loadApp(m_mycanvas);
	resetSize();
	m_mycanvas->fContext = fContext;
	fContext->setCanvas(m_mycanvas);

	SetTitle(fContext->getTitle().c_str());
	setMenu(path.c_str());
	m_mycanvas->startTimer(1000.0f / (float)fContext->getFPS());
}

// ----------------------------------------------------------------------------
// The canvas inside the frame. Our OpenGL connection
// ----------------------------------------------------------------------------

wxBEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
EVT_PAINT(MyGLCanvas::OnPaint)
EVT_SIZE(MyGLCanvas::OnSize)
EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
EVT_CHAR(MyGLCanvas::OnChar)
EVT_KEY_UP(MyGLCanvas::OnKeyUp)
EVT_KEY_DOWN(MyGLCanvas::OnKeyDown)
EVT_TIMER(TIMER_ID, MyGLCanvas::OnTimer)
wxEND_EVENT_TABLE()

//We create a wxGLContext in this constructor, do OGL initialization at OnSize().
MyGLCanvas::MyGLCanvas(MyFrame* parent, const wxGLAttributes& canvasAttrs)
: wxGLCanvas(parent, canvasAttrs)
, fContext(NULL)
, m_timer(this, TIMER_ID)
{
	m_parent = parent;
	m_oglContext = new wxGLContext(this, NULL, 0);
	if (!m_oglContext->IsOK())
	{
		delete m_oglContext;
		m_oglContext = NULL;
	}
}

MyGLCanvas::~MyGLCanvas()
{
	if (m_oglContext)
	{
		SetCurrent(*m_oglContext);
	}

	if (m_oglContext)
	{
		delete m_oglContext;
		m_oglContext = NULL;
	}
}

void MyGLCanvas::startTimer(float frameDuration)
{
	m_timer.Start((int) frameDuration);
}

void MyGLCanvas::OnTimer(wxTimerEvent& event)
{
	if (fContext->fIsStarted == false)
	{
		fContext->fIsStarted = true;
		fContext->GetRuntime()->BeginRunLoop();
	}
	
	Rtt::Runtime* runtime = fContext->GetRuntime();
	if (runtime->IsSuspended() == false)
	{
#ifndef _WIN32
		LinuxInputDeviceManager& deviceManager = (LinuxInputDeviceManager&) fContext->getPlatform()->GetDevice().GetInputDeviceManager();
		deviceManager.dispatchEvents(runtime);
#endif
		(*runtime)();
	}
}

void MyGLCanvas::OnKeyDown(wxKeyEvent& event)
{
	event.Skip();

	if (event.GetKeyCode() == WXK_ESCAPE)
	{
		m_parent->Close();		// close main window
	}
	else
	{
		fContext->GetKeyListener()->notifyKeyEvent(event, true);
	}
}

void MyGLCanvas::OnKeyUp(wxKeyEvent& event)
{
	event.Skip();
	if (fContext&& event.GetKeyCode() != WXK_ESCAPE)
	{
		fContext->GetKeyListener()->notifyKeyEvent(event, false);
	}
}

void MyGLCanvas::OnChar(wxKeyEvent& event)
{
	event.Skip();
	fContext->GetKeyListener()->notifyCharEvent(event);
}

void MyGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	if (m_winHeight > 0)
	{
		SwapBuffers();
	}
}

//Note:
// You may wonder why OpenGL initialization was not done at wxGLCanvas ctor.
// The reason is due to GTK+/X11 working asynchronously, we can't call
// SetCurrent() before the window is shown on screen (GTK+ doc's say that the
// window must be realized first).
// In wxGTK, window creation and sizing requires several size-events. At least
// one of them happens after GTK+ has notified the realization. We use this
// circumstance and do initialization then.

void MyGLCanvas::OnSize(wxSizeEvent& event)
{
	event.Skip();

	// If this window is not fully initialized, dismiss this event
	if (!IsShownOnScreen())
	{
		return;
	}

	//Now we have a context, retrieve pointers to OGL functions
	static bool isInited = false;
	if (isInited == false)
	{
		isInited = true;
		Rtt_ASSERT(m_oglContext);

		// The current context must be set before we get OGL pointers
		SetCurrent(*m_oglContext);

#if defined(_WIN32)
		glewExperimental = GL_TRUE;
		int rc = glewInit();
#endif

		// open home screen
		wxCommandEvent eventOpen(eventOpenProject);
		m_parent->OnOpen(eventOpen);
	}

	// This is normally only necessary if there is more than one wxGLCanvas
	// or more than one wxGLContext in the application.
	SetCurrent(*m_oglContext);

	// It's up to the application code to update the OpenGL viewport settings.
	m_winHeight = event.GetSize().y;
	//    m_oglManager->SetViewport(0, 0, event.GetSize().x, m_winHeight);

	// Generate paint event without erasing the background.
	Refresh(false);
}

void MyGLCanvas::OnMouse(wxMouseEvent& e)
{
	int x = e.GetX();
	int y = e.GetY();		// Y-coordinate is at bottom of the window

	float scrollWheelDeltaX = 0;
	float scrollWheelDeltaY = 0;

	// Fetch the current state of the "shift", "alt", and "ctrl" keys.
	const Uint8* key = 0; //SDL_GetKeyboardState(NULL);
	bool IsAltDown = false; //key[SDL_SCANCODE_LALT] | key[SDL_SCANCODE_RALT];
	bool IsShiftDown = false; //key[SDL_SCANCODE_LSHIFT] | key[SDL_SCANCODE_RSHIFT];
	bool IsControlDown = false; //key[SDL_SCANCODE_LCTRL] | key[SDL_SCANCODE_RCTRL];
	bool IsCommandDown = false; //key[SDL_SCANCODE_LGUI] | key[SDL_SCANCODE_RGUI];

	// Fetch the mouse's current up/down buttons states.
	bool isPrimaryDown = e.LeftIsDown();
	bool isSecondaryDown = e.RightIsDown();
	bool isMiddleDown = e.MiddleIsDown();

	Rtt::Runtime* runtime = fContext->GetRuntime();
	if (e.ButtonDown())
	{
		Rtt::MouseEvent::MouseEventType eventType = Rtt::MouseEvent::kDown;
		Rtt::MouseEvent mouseEvent(eventType, x, y, Rtt_FloatToReal(scrollWheelDeltaX), Rtt_FloatToReal(scrollWheelDeltaY), 0, isPrimaryDown, isSecondaryDown, isMiddleDown, IsShiftDown, IsAltDown, IsControlDown, IsCommandDown);

		runtime->DispatchEvent(mouseEvent);
		fContext->GetMouseListener()->TouchDown(x, y, 0);
	}
	else
		if (e.ButtonUp())
		{
			Rtt::MouseEvent::MouseEventType eventType = Rtt::MouseEvent::kUp;
			Rtt::MouseEvent mouseEvent(eventType, x, y, Rtt_FloatToReal(scrollWheelDeltaX), Rtt_FloatToReal(scrollWheelDeltaY), 0, isPrimaryDown, isSecondaryDown, isMiddleDown, IsShiftDown, IsAltDown, IsControlDown, IsCommandDown);

			runtime->DispatchEvent(mouseEvent);
			fContext->GetMouseListener()->TouchUp(x, y, 0);
		}
		else
		{
			Rtt::MouseEvent::MouseEventType eventType = Rtt::MouseEvent::kMove;

			// Determine if this is a "drag" event.
			if (isPrimaryDown || isSecondaryDown || isMiddleDown)
			{
				eventType = Rtt::MouseEvent::kDrag;
			}

			Rtt::MouseEvent mouseEvent(eventType, x, y, Rtt_FloatToReal(scrollWheelDeltaX), Rtt_FloatToReal(scrollWheelDeltaY), 0,
				isPrimaryDown, isSecondaryDown, isMiddleDown, IsShiftDown, IsAltDown, IsControlDown, IsCommandDown);

			runtime->DispatchEvent(mouseEvent);
			fContext->GetMouseListener()->TouchMoved(x, y, 0);

		}
}
