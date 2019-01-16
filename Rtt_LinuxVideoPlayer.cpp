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
#include "Rtt_LinuxVideoPlayer.h"
#include "Rtt_Lua.h"
#include "Rtt_LuaContext.h"
#include "Display/Rtt_Display.h"
#include "Display/Rtt_LuaLibDisplay.h"
#include "Rtt_Runtime.h"

#if (wxUSE_MEDIACTRL == 1)
	
namespace Rtt
{

	LinuxVideoPlayer::LinuxVideoPlayer(const Rtt::ResourceHandle<lua_State> & handle, Rtt_Allocator & allocator, int w, int h)
		: PlatformVideoPlayer(handle)
		, fVideoObject(NULL)
	{
		Rect bounds;
		bounds.xMin = 0;		
		bounds.xMax = w;
		bounds.yMin = 0;
		bounds.yMax = h;
		fVideoObject = new LinuxVideoObject(bounds);

		lua_State *L = handle.Dereference();
		Runtime& runtime = *LuaContext::GetRuntime(L);
		Display& display = runtime.GetDisplay();
		int result = LuaLibDisplay::AssignParentAndPushResult(L, display, fVideoObject, NULL);
		lua_pop(L, result);

		fVideoObject->Initialize();
	}

	LinuxVideoPlayer::~LinuxVideoPlayer()
	{
		// fixme add smart-ptr delete fVideoObject;
	}

	bool LinuxVideoPlayer::Load(const char * path, bool isRemote)
	{
		fVideoObject->setBackgroundColor(0, 0, 0, 255);		// black
		fVideoObject->load(path, isRemote);
		return true;
	}

	void LinuxVideoPlayer::Play()
	{
		// fixme
		// not needs ?, 	fAutoPlay = true;
	}

	void LinuxVideoPlayer::SetProperty(U32 mask, bool newValue)
	{
		PlatformVideoPlayer::SetProperty(mask, newValue);

		if (mask & kShowControls)
		{
			fVideoObject->showControls(newValue);
		}
	}

	void LinuxVideoPlayer::NotificationCallback()
	{
		DidDismiss(NULL, NULL);
	}

} // namespace Rtt

#endif
