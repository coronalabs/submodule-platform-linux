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

#include "Rtt_LinuxDisplayObject.h"
#include "Rtt_MPlatform.h"

#if ( wxUSE_WEBVIEW == 1)		

#include <wx/webview.h>

namespace Rtt
{

class LinuxWebView : public LinuxDisplayObject
{
	public:
		typedef LinuxWebView Self;
		typedef LinuxDisplayObject Super;

		LinuxWebView(const Rect& bounds);
		virtual ~LinuxWebView();

		virtual bool Initialize();
		virtual const LuaProxyVTable& ProxyVTable() const;
		virtual int ValueForKey(lua_State *L, const char key[]) const;
		virtual bool SetValueForKey(lua_State *L, const char key[], int valueIndex);
		virtual void Prepare(const Display& display) override;
		 
		void openURL(const char* url);
		
		struct eventArg: public wxObject
		{
			eventArg(LinuxWebView* thiz, const char* url)
				: fThiz(thiz) 
				, fRequestURL(url)
			{};
			
			LinuxWebView* fThiz;
			std::string fRequestURL;
		};
		
		bool Close();

	protected:
		static int Load(lua_State *L);
		static int Request(lua_State *L);
		static int Stop(lua_State *L);
		static int Back(lua_State *L);
		static int Forward(lua_State *L);
		static int Reload(lua_State *L);
		static int Resize(lua_State *L);
		
		static void onWebPopupNavigatingEvent(wxWebViewEvent& e);
		static void onWebPopupNavigatedEvent(wxWebViewEvent& e);
		static void onWebPopupLoadedEvent(wxWebViewEvent& e);
		static void onWebPopupErrorEvent(wxWebViewEvent& e);		
		
		static int OnDeleteCookies(lua_State *L);
};

} // namespace Rtt

#endif