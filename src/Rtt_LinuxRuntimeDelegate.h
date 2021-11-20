//////////////////////////////////////////////////////////////////////////////
//
// This file is part of the Corona game engine.
// For overview and more information on licensing please refer to README.md
// Home page: https://github.com/coronalabs/corona
// Contact: support@coronalabs.com
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Rtt_RuntimeDelegate.h"
#include "Rtt_DeviceOrientation.h"
#include <string>

namespace Rtt
{
	/// Delegate used to receive events from the Corona Runtime.
	/// This is done by assigning an instance of this class to the Runtime.SetDelegate() function.
	class LinuxRuntimeDelegate : public RuntimeDelegate
	{
	public:
		LinuxRuntimeDelegate();
		virtual ~LinuxRuntimeDelegate();

		virtual void DidInitLuaLibraries(const Runtime &sender) const;
		virtual void WillLoadMain(const Runtime &sender) const;
		virtual void WillLoadConfig(const Runtime &sender, lua_State *L) const;
		virtual void DidLoadConfig(const Runtime &sender, lua_State *L) const;

	public:
		mutable int fContentWidth;
		mutable int fContentHeight;
		mutable int fFPS;
		mutable std::string fScaleMode;
		mutable DeviceOrientation::Type fOrientation;
	};
}; // namespace Rtt
