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

#pragma once

#include "Core/Rtt_Build.h"
#include "Core/Rtt_Geometry.h"
#include "Rtt_PlatformDisplayObject.h"
#include "Rtt_LinuxContainer.h"

#include <wx/window.h>

#pragma region Forward Declarations

namespace Rtt
{
	class Display;
	class Renderer;
}
extern "C"
{
	struct lua_State;
}

#pragma endregion


namespace Rtt
{

	class LinuxDisplayObject : public PlatformDisplayObject
	{
	public:
		LinuxDisplayObject(const Rect& bounds, const char* elementType);
		virtual ~LinuxDisplayObject();

		void SetFocus();
		bool HasBackground() const;
		void SetBackgroundVisible(bool isVisible);
		virtual void DidMoveOffscreen(StageObject *oldStage);
		virtual void WillMoveOnscreen(StageObject *newStage);
		virtual bool CanCull() const;
		virtual void Draw(Renderer& renderer) const;
		virtual void GetSelfBounds(Rect& rect) const;
		virtual int ValueForKey(lua_State *L, const char key[]) const;
		virtual bool SetValueForKey(lua_State *L, const char key[], int valueIndex);
		virtual bool Initialize();
		virtual void Prepare(const Display& display);

		lua_State* GetLuaState() const {		return fLuaState; }
		int GetListenerRef() const { return fListenerRef; }

		void showControls(bool val);
		void setBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	protected:
		wxWindow* fWindow;

	private:
		Rect fSelfBounds;
		int fListenerRef;
		lua_State* fLuaState;
	};

} // namespace Rtt
