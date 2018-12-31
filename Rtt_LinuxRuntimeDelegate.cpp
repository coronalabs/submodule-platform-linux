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

#include "Rtt_LinuxRuntimeDelegate.h"
//#include "Rtt_Lua.h"
#include "Rtt_Runtime.h"
#include "Rtt_LuaContext.h"

// Lua Loader for flattened directories
// This allows .so plugins to load.
extern "C" int loader_Cflat (lua_State *L);

namespace Rtt
{

	/// Creates a new delegate used to receive events from the Corona runtime.
	LinuxRuntimeDelegate::LinuxRuntimeDelegate()
		: RuntimeDelegate()
		, fContentWidth(0)
		, fContentHeight(0)
		, fFPS(30)
		, fOrientation(DeviceOrientation::kUpright)
	{
	}

	/// Destructor. Cleans up allocated resources.
	LinuxRuntimeDelegate::~LinuxRuntimeDelegate()
	{
	}

#pragma endregion


#pragma region Public Member Functions

	void LinuxRuntimeDelegate::DidInitLuaLibraries( const Runtime& sender ) const
	{
		lua_State *L = sender.VMContext().L();
		Lua::InsertPackageLoader( L, &loader_Cflat, -1 );
	}


	/// Called just before the "main.lua" file has been loaded.
	/// This is the application's opportunity to register custom APIs into Lua.
	void LinuxRuntimeDelegate::WillLoadMain(const Runtime& sender) const
	{
		lua_State *L = sender.VMContext().L();

		if (Rtt_VERIFY(const_cast<Runtime&>(sender).PushLaunchArgs(true) > 0))
		{
			//		NativeToJavaBridge::GetInstance()->PushLaunchArgumentsToLuaTable(L);
			lua_pop(L, 1);
		}
	}

	void LinuxRuntimeDelegate::WillLoadConfig( const Runtime& sender, lua_State *L ) const
	{
		// Rtt_ASSERT( ! fDisplay );
		Rtt_ASSERT(1 == lua_gettop(L));
		Rtt_ASSERT(lua_istable(L, -1));

		lua_getfield(L, -1, "width");
		if (lua_tonumber(L, -1) > 0 && fContentWidth == 0)		// use width from build.settings if it exist there
		{
			fContentWidth = lua_tonumber(L, -1);
		}
		lua_pop(L, 1);

		lua_getfield(L, -1, "height");
		if (lua_tonumber(L, -1) > 0 && fContentHeight == 0)	// use width from build.settings if it exist there
		{
			fContentHeight = lua_tonumber(L, -1);
		}
		lua_pop(L, 1);

		lua_getfield(L, -1, "fps");
		int fps = (int)lua_tointeger(L, -1);
		if (60 == fps)	// Besides default (30), only 60 fps is supported
		{
			fFPS = 60;
		}
		lua_pop(L, 1);

		// default
		if (fContentWidth == 0)
		{
			fContentWidth = 320;
		}
		if (fContentHeight == 0)
		{
			fContentHeight = 480;
		}

		Rtt_ASSERT(1 == lua_gettop(L));
	}

	void LinuxRuntimeDelegate::DidLoadConfig( const Runtime& sender, lua_State *L ) const
	{
	}


#pragma endregion


} // namespace Rtt
