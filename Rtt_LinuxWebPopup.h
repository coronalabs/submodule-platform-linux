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

#include "Corona/CoronaLua.h"
#include "Rtt_PlatformWebPopup.h"
#include "Rtt_MPlatform.h"

#include <wx/webview.h>

#ifndef _Rtt_LinuxWebPopup_H__
#define _Rtt_LinuxWebPopup_H__

#if ( wxUSE_WEBVIEW == 1)		

namespace Rtt
{

class LinuxWebPopup : public PlatformWebPopup
{
	Rtt_CLASS_NO_COPIES(LinuxWebPopup)

	public:
		typedef PlatformWebPopup Super;

		LinuxWebPopup(); //Interop::RuntimeEnvironment& environment);
		virtual ~LinuxWebPopup();

		virtual void Show(const MPlatform& platform, const char* url) override;
		virtual bool Close() override;
		virtual void Reset() override;
		virtual void SetPostParams(const MKeyValueIterable& params) override;
		virtual int ValueForKey(lua_State *L, const char key[]) const override;
		virtual bool SetValueForKey(lua_State *L, const char key[], int valueIndex) override;
		
		static void onWebPopupNavigatingEvent(wxWebViewEvent& e);
		static void onWebPopupNavigatedEvent(wxWebViewEvent& e);
		static void onWebPopupLoadedEvent(wxWebViewEvent& e);
		static void onWebPopupErrorEvent(wxWebViewEvent& e);
		
		struct eventArg: public wxObject
		{
			eventArg(LinuxWebPopup* thiz) : fThiz(thiz) {};
			LinuxWebPopup* fThiz;
		};

	private:

		/// <summary>
		///  <para>Stores the Corona sandboxed base directory type when loading a local HTML file.</para>
		///  <para>The URL given to this popup is expected to be a relative path to file in this case.</para>
		///  <para>Set to "kUnknownDir" if not assigned when showing a popup.</para>
		/// </summary>
		MPlatform::Directory fBaseDirectory;

		/// <summary>
		///  <para>Set to a base directory path loading local HTML files or a URL domain.</para>
		///  <para>
		///   If this is not an empty string, then the URL to be loaded is expected to be a relative path
		///   and will be appended to this base path.
		///  </para>
		/// </summary>
		std::string fBaseUrl;

		/// <summary>Set true if the popup's control needs to be resized during Corona's next render pass.</summary>
		bool fWasResizeRequested;

		wxWebView* fWebBrowserPointer;
		wxEvtHandler fEvent;
};

} // namespace Rtt

#endif

#endif
