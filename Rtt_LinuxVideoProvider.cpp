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
#include "Rtt_LinuxVideoProvider.h"
#include "Rtt_Lua.h"
#include "Rtt_LuaContext.h"
#include "Rtt_Runtime.h"

// ----------------------------------------------------------------------------

namespace Rtt
{

// ----------------------------------------------------------------------------

#pragma region Constructors/Destructors
/// Creates a new Video provider.
/// @param handle Reference to Lua state used to send Video selection notifications to a Lua listener function.
LinuxVideoProvider::LinuxVideoProvider(const ResourceHandle<lua_State> & handle, int w, int h)
:	PlatformVideoProvider(handle)
{
}

#pragma endregion


#pragma region Public Member Functions
/// Determines if the given Video source (Camera, Photo Library, etc.) is supported on this platform.
/// @param source Unique integer ID of the Video source. IDs are defined in PlatformVideoProvider class.
/// @return Returns true if given Video source is supported on this platform. Returns false if not.
bool LinuxVideoProvider::Supports(int source) const
{
	return false;
}

/// Displays a window for selecting an Video.
/// @param source Unique integer ID indicating what kind of window to display such as the Camera or Photo Library.
/// @return Returns true if the window was shown.
///         Returns false if given source is not provided or if an Video provider window is currently shown.
bool LinuxVideoProvider::Show(int source, lua_State* L, int maxTime, int quality)
{
	// Show the Video provider window.
	return false;
}

#pragma endregion

// ----------------------------------------------------------------------------

} // namespace Rtt

// ----------------------------------------------------------------------------
