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

#include "Core/Rtt_Build.h"
#include "Core/Rtt_FileSystem.h"
#include "Rtt_GPUStream.h"
#include "Rtt_LuaContext.h"
#include "Rtt_LuaLibNative.h"
#include "Rtt_Runtime.h"
#include "Rtt_LinuxPlatform.h"
#include "Rtt_LinuxAudioPlayer.h"
#include "Rtt_LinuxAudioRecorder.h"
#include "Rtt_LinuxBitmap.h"
//#include "Rtt_LinuxEventSound.h"
//#include "Rtt_LinuxFBConnect.h"
#include "Rtt_LinuxFont.h"
#include "Rtt_LinuxImageProvider.h"
//#include "Rtt_LinuxMapViewObject.h"
#include "Rtt_LinuxScreenSurface.h"
//#include "Rtt_LinuxStoreProvider.h"
#include "Rtt_LinuxTextBoxObject.h"
#include "Rtt_LinuxVideoObject.h"
#include "Rtt_LinuxVideoPlayer.h"
#include "Rtt_LinuxVideoProvider.h"
#include "Rtt_LinuxWebPopup.h"
#include "Rtt_LinuxWebView.h"
#include "Rtt_LinuxContainer.h"
#include "Rtt_PreferenceCollection.h"
#include "Rtt_Freetype.h"

#include <wx/wx.h>
#include "wx/activityindicator.h"

namespace Rtt
{

	LinuxPlatform::LinuxPlatform(const char *resourceDir, const char *documentsDir, const char *temporaryDir,
		const char *cachesDir, const char *systemCachesDir, const char *installDir)
		: fAllocator(Rtt_AllocatorCreate()),
		fDevice(*fAllocator),
		fAudioPlayer(NULL),
		fVideoPlayer(NULL),
		fImageProvider(NULL),
		fVideoProvider(NULL),
		fWebPopup(NULL),
		fResourceDir(fAllocator),
		fDocumentsDir(fAllocator),
		fTemporaryDir(fAllocator),
		fCachesDir(fAllocator),
		fSystemCachesDir(fAllocator),
		fInstallDir(fAllocator),
		fStoreProvider(NULL),
		fFBConnect(NULL),
		fScreenSurface(NULL)
	{
		fResourceDir.Set(resourceDir);
		fDocumentsDir.Set(documentsDir);
		fTemporaryDir.Set(temporaryDir);
		fCachesDir.Set(cachesDir);
		fSystemCachesDir.Set(systemCachesDir);
		fInstallDir.Set(installDir);
	}

	LinuxPlatform::~LinuxPlatform()
	{
		Rtt_DELETE(fFBConnect);
		Rtt_DELETE(fStoreProvider);
		Rtt_DELETE(fWebPopup);
		Rtt_DELETE(fVideoPlayer);
		Rtt_DELETE(fAudioPlayer);
		Rtt_DELETE(fImageProvider);
		Rtt_DELETE(fVideoProvider);

		// it will be deleted in Rtt_Display by Rtt_DELETE( fTarget );
		//Rtt_DELETE(fScreenSurface);
	}

	MPlatformDevice& LinuxPlatform::GetDevice() const
	{
		return const_cast<LinuxDevice&>(fDevice);
	}

	PlatformSurface* LinuxPlatform::CreateScreenSurface() const
	{
		Rtt_ASSERT(fScreenSurface == NULL);
		fScreenSurface = Rtt_NEW(fAllocator, LinuxScreenSurface());
		return fScreenSurface;
	}

	PlatformSurface* LinuxPlatform::CreateOffscreenSurface(const PlatformSurface& parent) const
	{
		return Rtt_NEW(fAllocator, LinuxOffscreenSurface(parent));
	}

	int LinuxPlatform::CanOpenURL(const char* url) const
	{
		return true;
	}

	bool LinuxPlatform::OpenURL(const char* url) const
	{
		return wxLaunchDefaultBrowser(url);
	}

	PlatformVideoPlayer* LinuxPlatform::GetVideoPlayer(const Rtt::ResourceHandle<lua_State> & handle) const
	{
#if (wxUSE_MEDIACTRL == 1)		
		if (!fVideoPlayer)
		{
			int w, h;
			fScreenSurface->getWindowSize(&w, &h);

			fVideoPlayer = Rtt_NEW(fAllocator, LinuxVideoPlayer(handle, *fAllocator, w, h));
		}
		return fVideoPlayer;
#else
		return NULL;
#endif
	}

	PlatformImageProvider* LinuxPlatform::GetImageProvider(const Rtt::ResourceHandle<lua_State> & handle) const
	{
		if (!fImageProvider)
		{
			int w, h;
			fScreenSurface->getWindowSize(&w, &h);
#ifndef _WIN32
			fImageProvider = Rtt_NEW(fAllocator, LinuxImageProvider(handle, w, h));
#endif
		}
		return fImageProvider;
	}

	PlatformVideoProvider* LinuxPlatform::GetVideoProvider(const Rtt::ResourceHandle<lua_State> & handle) const
	{
		if (!fVideoProvider)
		{
			int w, h;
			fScreenSurface->getWindowSize(&w, &h);

			fVideoProvider = Rtt_NEW(fAllocator, LinuxVideoProvider(handle, w, h));
		}
		return fVideoProvider;
	}

	PlatformStoreProvider* LinuxPlatform::GetStoreProvider(const ResourceHandle<lua_State>& handle) const
	{
//		if (!fStoreProvider)
//		{
//			fStoreProvider = Rtt_NEW(Allocator(), LinuxStoreProvider(handle));
//		}
		return NULL; //fStoreProvider;
	}

	void LinuxPlatform::SetIdleTimer(bool enabled) const
	{
	}

	bool LinuxPlatform::GetIdleTimer() const
	{
		return true;
	}

	NativeAlertRef LinuxPlatform::ShowNativeAlert(const char *title, const char *msg, const char **buttonLabels, U32 numButtons, LuaResource* resource) const
	{
		msgBox *msgs = new msgBox(title, msg, buttonLabels, numButtons, resource);
		msgs->Show(true);
		return NULL; // (NativeAlertRef) 0x1234;
	}

	void LinuxPlatform::CancelNativeAlert(NativeAlertRef alert, S32 index) const
	{
		// modal msgbox
		Rtt_ASSERT_MSG(0, "Code should NOT be reached");
	}

	Rtt_Allocator& LinuxPlatform::GetAllocator() const
	{
		return *fAllocator;
	}

	RenderingStream* LinuxPlatform::CreateRenderingStream(bool antialias) const
	{
		RenderingStream* result = Rtt_NEW(fAllocator, GPUStream(fAllocator));
		result->SetProperty(RenderingStream::kFlipHorizontalAxis, true);
		return result;
	}

	PlatformTimer* LinuxPlatform::CreateTimerWithCallback(MCallback& callback) const
	{
		return Rtt_NEW(fAllocator, LinuxTimer(callback));
	}

	PlatformBitmap* LinuxPlatform::CreateBitmap(const char* path, bool convertToGrayscale) const
	{
		PlatformBitmap *result = NULL;
		if (path)
		{
			if (convertToGrayscale)
			{
				result = Rtt_NEW(&GetAllocator(), LinuxMaskFileBitmap(GetAllocator(), path));
			}
			else
			{
				result = Rtt_NEW(&GetAllocator(), LinuxFileBitmap(GetAllocator(), path));
			}
		}

		if (result && result->GetFormat() == PlatformBitmap::kUndefined)
		{
			// failed to load bitmap
			Rtt_DELETE(result);
			result = NULL;
		}

		return result;
	}

	bool LinuxPlatform::FileExists(const char *filename) const
	{
		// Validate.
		if (Rtt_StringIsEmpty(filename))
		{
			return false;
		}

		// Determine if the given file name exists.
		bool fileExists = false;
		FILE *file = fopen(filename, "r");
		if (file)
		{
			fileExists = true;
			fclose(file);
		}
		return fileExists;
	}

	void LinuxPlatform::PathForFile(const char *filename, MPlatform::Directory baseDir, U32 flags, String& result) const
	{
		// Initialize result to an empty string in case the file was not found.
		result.Set(NULL);

		// Fetch the path for the given file.
		if (filename && strstr(filename, "://"))
		{
			// The given file name is a URL.
			// Accept it as is and ignore the base directory.
			result.Set(filename);
		}
		else
		{
			// Create an absolute path by appending the file name to the base directory.
			switch (baseDir)
			{
			case MPlatform::kResourceDir:
			case MPlatform::kSystemResourceDir:
				PathForFile(filename, fResourceDir.GetString(), result);
				if (filename != NULL && FileExists(result.GetString()) == false)
				{
					// look for Resources dir
					String resDir;
					resDir.Append(fInstallDir);
					resDir.Append("/Resources");
					PathForFile(filename, resDir.GetString(), result);
					Rtt_WARN_SIM(	!filename || FileExists(result.GetString()),	("WARNING: Cannot create path for resource file '%s (%s)'. File does not exist.\n\n", filename, result.GetString()));
				}
				break;

			case MPlatform::kCachesDir:
				PathForFile(filename, fCachesDir.GetString(), result);
				break;

			case MPlatform::kSystemCachesDir:
				PathForFile(filename, fSystemCachesDir.GetString(), result);
				break;

			case MPlatform::kTmpDir:
				PathForFile(filename, fTemporaryDir.GetString(), result);
				break;

			case MPlatform::kPluginsDir:
				break;

			case MPlatform::kDocumentsDir:
			default:
				PathForFile(filename, fDocumentsDir.GetString(), result);
				break;
			}
		}

		// Check if the file exists, if enabled.
		// Result will be set to an empty string if the file could not be found.
		if (flags & MPlatform::kTestFileExists)
		{
			if (FileExists(result.GetString()) == false)
			{
				result.Set(NULL);
			}
		}
	}

	void LinuxPlatform::SetProjectResourceDirectory(const char* filename)
	{
//		Rtt_ASSERT_NOT_IMPLEMENTED();
	}

	void LinuxPlatform::SetSkinResourceDirectory(const char* filename)
	{
//		Rtt_ASSERT_NOT_IMPLEMENTED();
	}

	void LinuxPlatform::PathForFile(const char * filename, const char * baseDir, String & result) const
	{
		result.Set(NULL);
		if (filename)
		{
			// Append absolute path to the given directory.
			result.Append(baseDir);

			// Append directory separator, if not already there.
			size_t npl = strlen(result.GetString());
			if (npl > 0 && result.GetString()[npl - 1] != '/')
			{
				result.Append("/");
			}

			// Append the file name.
			result.Append(filename);
		}
		else
		{
			// File name not provided. So, just provide the directory path.
			result.Set(baseDir);
		}
	}

	void  LinuxPlatform::HttpPost(const char* url, const char* key, const char* value) const
	{
		// Rtt_ASSERT_NOT_IMPLEMENTED();
	}

	PlatformEventSound * LinuxPlatform::CreateEventSound(const ResourceHandle<lua_State> & handle, const char* filePath) const
	{
//		LinuxEventSound *p = Rtt_NEW(fAllocator, LinuxEventSound(handle, *fAllocator));
//		p->Load(filePath);
		return NULL; //p;
	}

	void LinuxPlatform::ReleaseEventSound(PlatformEventSound * soundID) const
	{
		Rtt_DELETE(soundID);
	}

	void LinuxPlatform::PlayEventSound(PlatformEventSound * soundID) const
	{
		// soundID->Play();
	}

	PlatformAudioPlayer * LinuxPlatform::GetAudioPlayer(const ResourceHandle<lua_State> & handle) const
	{
//		if (!fAudioPlayer)
//		{
//			fAudioPlayer = Rtt_NEW(fAllocator, LinuxAudioPlayer(handle, *fAllocator));
//		}
		return NULL; //fAudioPlayer;
	}

	PlatformAudioRecorder * LinuxPlatform::CreateAudioRecorder(const Rtt::ResourceHandle<lua_State> & handle, const char * filePath) const
	{
		return Rtt_NEW(fAllocator, LinuxAudioRecorder(handle, *fAllocator, filePath));
	}

	void LinuxPlatform::RaiseError(MPlatform::Error e, const char * reason) const
	{
		const char kNull[] = "(null)";
		if (!reason) { reason = kNull; }
		Rtt_TRACE(("MPlatformFactory error(%d): %s\n", e, kNull));
	}

	bool LinuxPlatform::SaveImageToPhotoLibrary(const char* filePath) const
	{
		return false;
	}

	bool LinuxPlatform::SaveBitmap(PlatformBitmap * bitmap, const char * filePath, float jpegQuality) const
	{
		return LinuxBaseBitmap::SaveBitmap(fAllocator, bitmap, filePath);
	}

	bool LinuxPlatform::AddBitmapToPhotoLibrary(PlatformBitmap* bitmap) const
	{
		return SaveBitmap(bitmap, NULL, 1.0f);
	}

	void LinuxPlatform::SetStatusBarMode(MPlatform::StatusBarMode newValue) const
	{
	}

	MPlatform::StatusBarMode LinuxPlatform::GetStatusBarMode() const
	{
		return MPlatform::kDefaultStatusBar;
	}

	int LinuxPlatform::GetStatusBarHeight() const
	{
		return 0;
	}

	int LinuxPlatform::GetTopStatusBarHeightPixels()  const
	{
		return 0;
	}

	int LinuxPlatform::GetBottomStatusBarHeightPixels() const
	{
		return 0;
	}

	int LinuxPlatform::SetSync(lua_State* L) const
	{
		return 0;
	}

	int LinuxPlatform::GetSync(lua_State* L) const
	{
		return 0;
	}

	void LinuxPlatform::BeginRuntime(const Runtime& runtime) const
	{
	}

	void LinuxPlatform::EndRuntime(const Runtime& runtime) const
	{
	}

	PlatformExitCallback* LinuxPlatform::GetExitCallback()
	{
		return NULL;
	}

	bool LinuxPlatform::RequestSystem(lua_State *L, const char *actionName, int optionsIndex) const
	{
		return false;
	}

	void LinuxPlatform::RuntimeErrorNotification(const char *errorType, const char *message, const char *stacktrace) const
	{
	}

	const MCrypto& LinuxPlatform::GetCrypto() const
	{
		return fCrypto;
	}

	void LinuxPlatform::GetPreference(Category category, Rtt::String * value) const
	{
		// Validate.
		if (value == NULL)
		{
			return;
		} 

		// Fetch the requested preference value.
		const char *resultPointer = "";
		switch (category)
		{
		case kLocaleLanguage:
			resultPointer = "en"; // fixme: jsGetLocaleLanguage();
			break;
		case kLocaleCountry:
			resultPointer = "US"; // fixme: jsGetLocaleCountry();
			break;
		case kLocaleIdentifier:
		case kUILanguage:
		{
			resultPointer = "en_US"; // fixme: jsGetLanguage();
			break;
		}
		case kDefaultStatusBarFile:
			// todo
			break;
		case kDarkStatusBarFile:
			// todo
			break;
		case kTranslucentStatusBarFile:
			// todo
			break;
		case kLightTransparentStatusBarFile:
			// todo
			break;
		case kDarkTransparentStatusBarFile:
			// todo
			break;
		case kScreenDressingFile:
			// todo
			break;
		case kSubscription:
			// todo
			break;
		default:
			//			Rtt_ASSERT_NOT_REACHED();
			break;
		}

		// Copy the requested preference string to the given value.
		// Set's value to NULL (an empty string) if the given preference is not supported.
		value->Set(resultPointer);
	}

	void LinuxPlatform::SetActivityIndicator(bool visible) const
	{
		wxGetApp().getParent()->SetCursor(visible ? *wxHOURGLASS_CURSOR : *wxSTANDARD_CURSOR);
	}

	PlatformWebPopup* LinuxPlatform::GetWebPopup() const
	{
#if ( wxUSE_WEBVIEW == 1)		
		if (!fWebPopup)
		{
			fWebPopup = Rtt_NEW(&GetAllocator(), LinuxWebPopup());
		}
		return fWebPopup;
#else
		return NULL;
#endif
	}

	bool LinuxPlatform::CanShowPopup(const char *name) const
	{
		return true;
	}

	bool LinuxPlatform::ShowPopup(lua_State *L, const char *name, int optionsIndex) const
	{
		bool result = false;

		// Validate.
		if (!L || !name || (strlen(name) <= 0))
		{
			return false;
		}

		// Display the requested window.
		if (Rtt_StringCompareNoCase(name, "mail") == 0)
		{
			// Display a "Send Mail" window.
		}
		else if (Rtt_StringCompareNoCase(name, "sms") == 0)
		{
			// Display a "Send SMS" window.
		}
		else if ((Rtt_StringCompareNoCase(name, "rateApp") == 0) || (Rtt_StringCompareNoCase(name, "appStore") == 0))
		{
			// Display the app store window.
		}
		else
		{
			// Unknown window/popup name. Abort.
			result = false;
		}

		// Assume the window was displayed at this point.
		return result;
	}

	bool LinuxPlatform::HidePopup(const char *name) const
	{
		return true;
	}

	PlatformDisplayObject* LinuxPlatform::CreateNativeTextBox(const Rect& bounds) const
	{
		return Rtt_NEW(&GetAllocator(), LinuxTextBoxObject(bounds, false));
	}

	PlatformDisplayObject * LinuxPlatform::CreateNativeTextField(const Rect & bounds) const
	{
		return Rtt_NEW(&GetAllocator(), LinuxTextBoxObject(bounds, true));
	}

	void LinuxPlatform::SetKeyboardFocus(PlatformDisplayObject * textObject) const
	{
		if (textObject)
		{
			((LinuxDisplayObject*)textObject)->SetFocus();
		}
		else
		{
			//TODO: Remove keyboard focus from native display objects.
		}
	}

	PlatformDisplayObject * LinuxPlatform::CreateNativeMapView(const Rect& bounds) const
	{
		return 0; //Rtt_NEW(&GetAllocator(), LinuxMapViewObject(bounds));
	}

	PlatformDisplayObject * LinuxPlatform::CreateNativeWebView(const Rect& bounds) const
	{
#if ( wxUSE_WEBVIEW == 1)				
		return Rtt_NEW(&GetAllocator(), LinuxWebView(bounds));
#else
		return NULL;
#endif
	}

	PlatformDisplayObject * LinuxPlatform::CreateNativeVideo(const Rect& bounds) const
	{
#if (wxUSE_MEDIACTRL == 1)		
		return Rtt_NEW(&GetAllocator(), LinuxVideoObject(bounds));
#else
		return NULL;
#endif
	}

	PlatformDisplayObject * LinuxPlatform::GetNativeDisplayObjectById(const int objectId) const
	{
		return NULL;
	}

	Rtt_Real LinuxPlatform::GetStandardFontSize() const
	{
		return 32.0f;
	}

	S32 LinuxPlatform::GetFontNames(lua_State *L, int index) const
	{
		S32 numFonts = 0;
#if 0
		Rtt::StringArray fonts(fAllocator);
		if (NativeToJavaBridge::GetInstance()->GetFonts(fonts)) {
			numFonts = fonts.GetLength();

			for (int i = 0; i < numFonts; i++)
			{
				lua_pushstring(L, fonts.GetElement(i));
				lua_rawseti(L, index, i + 1);
			}
	}
#endif
		return numFonts;
}

	PlatformFont * LinuxPlatform::CreateFont(PlatformFont::SystemFont fontType, Rtt_Real size) const
	{
		return Rtt_NEW(fAllocator, LinuxFont(*fAllocator, fontType, size));
	}

	PlatformFont * LinuxPlatform::CreateFont(const char *fontName, Rtt_Real size) const
	{
		bool isBold = false;
		return Rtt_NEW(fAllocator, LinuxFont(*fAllocator, fontName, size, isBold));
	}

	void LinuxPlatform::SetTapDelay(Rtt_Real delay) const
	{
	}

	Rtt_Real LinuxPlatform::GetTapDelay() const
	{
		return 1.0f;
	}

	PlatformFBConnect* LinuxPlatform::GetFBConnect() const
	{
	//	if (NULL == fFBConnect)
	///	{
	//		fFBConnect = Rtt_NEW(fAllocator, LinuxFBConnect);
	//	}
		return 0; //fFBConnect;
	}

	void* LinuxPlatform::CreateAndScheduleNotification(lua_State *L, int index) const
	{
		return NULL;
	}

	void LinuxPlatform::ReleaseNotification(void *notificationId) const
	{
	}

	void LinuxPlatform::CancelNotification(void *notificationId) const
	{
		if (notificationId == NULL)
		{
			//TODO: Cancel all notifications.
		}
		else
		{
			//TODO: Cancel the given notification by its ID.
		}
	}

	void LinuxPlatform::FlurryInit(const char * applicationKey) const
	{
	}

	void LinuxPlatform::FlurryEvent(const char * eventId) const
	{
	}

	void LinuxPlatform::SetNativeProperty(lua_State *L, const char *key, int valueIndex) const
	{
		// Validate arguments.
		if (!L || Rtt_StringIsEmpty(key) || !valueIndex)
		{
			return;
		}

		// Handle the given property value.
		if (Rtt_StringCompare(key, "applicationIconBadgeNumber") == 0)
		{
			// Clear all notifications if the application badge number was set to zero.
			// Note: This is to match iOS' behavior.
			if (lua_type(L, valueIndex) == LUA_TNUMBER)
			{
				int badgeNumber = lua_tointeger(L, valueIndex);
				if (badgeNumber <= 0)
				{
					//TODO: Cancel all notifications.
				}
				//TODO: Update the badge count on the Windows Phone tile???
			}
		}
	}

	int LinuxPlatform::PushNativeProperty(lua_State *L, const char *key) const
	{
		// Validate.
		if (!L)
		{
			return 0;
		}

		int pushedValues = 0;

		// todo
/*
		// Push the requested native property information to Lua.
		if (Rtt_StringCompare(key, "windowTitleText") == 0)
		{
			// Fetch the window's title bar text.
			// todo
			lua_pushstring(L, "");
			pushedValues = 1;
		}
		else if (Rtt_StringCompare(key, "windowMode") == 0)
		{
			// Fetch the window's current mode such as "normal", "maximized", "fullscreen", etc.
			// todo
			lua_pushstring(L, "");
			pushedValues = 1;
		}
		else if (Rtt_StringCompare(key, "mouseCursorVisible") == 0)
		{
			// Fetch the mouse cursor's current visibility state.
			// todo
			lua_pushboolean(L, 1);
			pushedValues = 1;
		}
		else
		{
			// The given key is unknown. Log a warning.
			if (Rtt_StringIsEmpty(key))
			{
				Rtt_LogException("native.getProperty() was given a nil or empty string key.");
			}
			else
			{
				Rtt_LogException("native.getProperty() was given unknown key: %s", key);
			}
		}
		*/

		// Push nil if given a key that is unknown on this platform.
		if (pushedValues <= 0)
		{
			lua_pushnil(L);
			pushedValues = 1;
		}

		// Return the number of values pushed into Lua.
		return pushedValues;
	}

	int LinuxPlatform::PushSystemInfo(lua_State *L, const char *key) const
	{
		// Validate.
		if (!L)
		{
			return 0;
		}

		// Push the requested system information to Lua.
		int pushedValues = 0;
		if (Rtt_StringCompare(key, "appName") == 0)
		{
			// Fetch the application's name.
			lua_pushstring(L, "appName");		// todo
			pushedValues = 1;
		}
		else if (Rtt_StringCompare(key, "appVersionString") == 0)
		{
			// Return an empty version string since it is unknown by the simulator.
			lua_pushstring(L, "appVersionString");		// todo
			pushedValues = 1;
		}
		else if (Rtt_StringCompare(key, "bundleID") == 0)
		{
			lua_pushstring(L, "bundleID");		// todo
			pushedValues = 1;
		}
		else if (Rtt_StringCompare(key, "isoCountryCode") == 0)
		{
			// Fetch the ISO 3166-1 country code.
			lua_pushstring(L, "isoCountryCode");	// todo
			pushedValues = 1;
		}
		else if (Rtt_StringCompare(key, "isoLanguageCode") == 0)
		{
			// Fetch the ISO 639 language code with an ISO 15924 script code appended to it if available.
			// Note: This will return a 3 letter ISO 639-2 code if current language is not in the 2 letter ISO 639-1 standard.
			//       For example, this can happen with the Hawaiian language, which will return "haw".
			lua_pushstring(L, "isoLanguageCode");	// todo
			pushedValues = 1;
		}
		else
		{
			// Push nil if given a key that is unknown on this platform.
			lua_pushnil(L);
			pushedValues = 1;
		}

		// Return the number of values pushed into Lua.
		return pushedValues;
	}

	void LinuxPlatform::NetworkBaseRequest(lua_State *L, const char *url, const char *method, LuaResource *listener, int paramsIndex, const char *path) const
	{
	}

	void LinuxPlatform::NetworkRequest(lua_State *L, const char *url, const char *method, LuaResource *listener, int paramsIndex) const
	{
		NetworkBaseRequest(L, url, method, listener, paramsIndex, NULL);
	}

	void LinuxPlatform::NetworkDownload(lua_State *L, const char *url, const char *method, LuaResource *listener, int paramsIndex, const char *filename, MPlatform::Directory baseDir) const
	{
		Runtime& runtime = *LuaContext::GetRuntime(L);
		const MPlatform& platform = runtime.Platform();

		String filePath(runtime.GetAllocator());
		platform.PathForFile(filename, baseDir, MPlatform::kDefaultPathFlags, filePath);
		NetworkBaseRequest(L, url, method, listener, paramsIndex, filePath.GetString());
	}


	PlatformReachability* LinuxPlatform::NewReachability(const ResourceHandle<lua_State>& handle, PlatformReachability::PlatformReachabilityType type, const char* address) const
	{
		return Rtt_NEW(fAllocator, PlatformReachability(handle, type, address));
	}

	bool LinuxPlatform::SupportsNetworkStatus() const
	{
		return false;
	}

	PlatformBitmap* LinuxPlatform::CreateBitmapMask(const char str[], const PlatformFont& font, Real w, Real h, const char alignment[], Real& baselineOffset) const
	{
		return Rtt_NEW(&GetAllocator(), LinuxTextBitmap(GetAllocator(), str, font, (int)(w + 0.5f), (int)(h + 0.5f), alignment, baselineOffset));
	}


	FontMetricsMap LinuxPlatform::GetFontMetrics(const PlatformFont& font) const
	{
		FontMetricsMap ret;
		float ascent = 0;
		float descent = 0;
		float height = 0;
		float leading = 0;

		glyph_freetype_provider::getMetrics(font.Name(), font.Size(), &ascent, &descent, &height, &leading);

		ret["ascent"] = ascent;
		ret["descent"] = descent;
		ret["leading"] = leading;
		ret["height"] = height;
		return ret;
	}

	void LinuxPlatform::GetSafeAreaInsetsPixels(Rtt_Real &top, Rtt_Real &left, Rtt_Real &bottom, Rtt_Real &right) const
	{
		top = left = bottom = right = 0;
	}

	Preference::ReadValueResult LinuxPlatform::GetPreference(const char* categoryName, const char* keyName) const
	{
		// add category name as prefix
		std::string key(categoryName);
		if (strlen(keyName) > 0)
		{
			key += '.';
			key += keyName;
		}
		char val[1024];
		
		// maybe it would be better to use sqlite3 ??
		std::string path = fSystemCachesDir.GetString();
		path += '/';
		path += key;
		FILE* f = fopen(path.c_str(), "rb");
		bool rc = false;
		if (f)
		{
			int n = fread(val, 1, sizeof(val) - 1, f);
			val[n] = '\0';
			fclose(f);
			rc = true;
		}
		return rc == false ? Preference::ReadValueResult::kPreferenceNotFound : Preference::ReadValueResult::SucceededWith(val);
	}

	OperationResult LinuxPlatform::SetPreferences(const char* categoryName, const PreferenceCollection& preferences) const
	{
		bool rc = true;
		for (int index = preferences.GetCount() - 1; index >= 0 && rc == true; index--)
		{
			// Fetch the next preference to write to the database.
			auto preferencePointer = preferences.GetByIndex(index);
			if (preferencePointer != NULL)
			{
				// add category name as prefix
				std::string key(categoryName);
				key += '.';
				key += preferencePointer->GetKeyName();

				PreferenceValue pval = preferencePointer->GetValue();

				// Insert the preference value as string.
				PreferenceValue::StringResult strval = pval.ToString();
				const char* val = strval.GetValue()->c_str();

				// maybe it would be better to use sqlite3 ??
				std::string path = fSystemCachesDir.GetString();
				path += '/';
				path += key;
				FILE* f = fopen(path.c_str(), "wb");
				if (f)
				{
					fwrite(val, 1, strlen(val), f);
					fclose(f);
					rc = true;
				}
			}
		}
		return rc == false ? OperationResult::FailedWith("This API is not supported on this platform.") : Rtt::OperationResult::kSucceeded;
	}

	OperationResult LinuxPlatform::DeletePreferences(const char* categoryName, const char** keyNameArray, U32 keyNameCount) const
	{
		bool rc = true;
		for (int i = 0; i < keyNameCount && keyNameArray && rc; i++)
		{
			// Fetch the next preference key, add category name as prefix
			std::string key(categoryName);
			key += '.';
			key += keyNameArray[i];
			
			// maybe it would be better to use sqlite3 ??
			std::string path = fSystemCachesDir.GetString();
			path += '/';
			path += key;
			int n = Rtt_DeleteFile(path.c_str());
			rc = n == 0 ? true : false;
		}
		return rc == false ? OperationResult::FailedWith("This API is not supported on this platform.") : Rtt::OperationResult::kSucceeded;
	}

	bool LinuxPlatform::HttpDownload(const char* url, String& result, String& errorMesg, const std::map<std::string, std::string>& headers) const
	{
		Rtt_ASSERT_MSG(0, "Code should NOT be reached");
		return false;
	}

	bool LinuxPlatform::HttpDownloadFile(const char* url, const char* filename, String& errorMesg, const std::map<std::string, std::string>& headers) const
	{
		Rtt_ASSERT_MSG(0, "Code should NOT be reached");
		return false;
	}


	void LinuxPlatform::Suspend() const
	{
	}

	void LinuxPlatform::Resume() const
	{
	}

	void LinuxPlatform::setWindow(void* ctx)
	{
		Rtt_ASSERT(fScreenSurface);
		fScreenSurface->setWindow(ctx); 
	}

	//
	// for alert
	//

	msgBox::msgBox(const char *title, const char *msg, const char **buttonLabels, U32 numButtons, LuaResource* resource)
		: wxFrame(wxGetApp().getFrame(), wxID_ANY, title, wxDefaultPosition, wxSize(420, 180), wxCAPTION|wxCLOSE_BOX)
		, fCallback(resource)
	{
		wxSize sz = GetSize();
		wxPanel *panel = new wxPanel(this, wxID_ANY);
		wxBoxSizer *hbox = new wxBoxSizer(wxVERTICAL);

		wxStaticText* txt = new wxStaticText(panel, wxID_ANY, msg, wxPoint(0, 10), wxSize(sz.x - 20, 60), wxALIGN_CENTRE_HORIZONTAL);
		wxFont font = txt->GetFont();
//		font.SetPointSize(font.GetPointSize() + 2);
		font.SetWeight(wxFONTWEIGHT_BOLD);
		txt->SetFont(font);

		// add buttons
		wxGridSizer *gs = new wxGridSizer(1, numButtons, 1, numButtons);
		for (int i = 0; i < numButtons; i++)
		{
			const char* caption = buttonLabels[i];
			wxButton *b = new wxButton(panel, i, caption);
			Connect(i, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(msgBox::ShowMessage));
			gs->Add(b, 1);
		}

		Connect(wxID_ANY, wxEVT_CLOSE_WINDOW, wxCloseEventHandler(msgBox::OnClose)); 

		hbox->Add(txt, 0, wxALL, 15);
		hbox->Add(gs, 0, wxALL, 15);
		panel->SetSizer(hbox);
		Center();
	}

	msgBox::~msgBox()
	{
	}

	void msgBox::ShowMessage(wxCommandEvent& e)
	{
		// Invoke the Lua listener.
		int id = e.GetId();
		if (fCallback)
		{
			// Invoke the Lua listener.
			Rtt::LuaResource *pLuaResource = (Rtt::LuaResource *) fCallback;
			bool wasCanceled = id >= 0 ? false : true;
			Rtt::LuaLibNative::AlertComplete(*pLuaResource, id, wasCanceled);

			// Delete the Lua resource.
			Rtt_DELETE(pLuaResource);
		}
		Destroy();
	}

	void msgBox::OnClose(wxCloseEvent& event)
	{
		// Invoke the Lua listener.
		Rtt::LuaResource *pLuaResource = (Rtt::LuaResource *) fCallback;
		bool wasCanceled = true;
		Rtt::LuaLibNative::AlertComplete(*pLuaResource, -1, wasCanceled);

		// Delete the Lua resource.
		Rtt_DELETE(pLuaResource);

		Destroy();
	}

} // namespace Rtt
