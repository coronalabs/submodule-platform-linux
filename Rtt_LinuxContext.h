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

#include <string>
#include <map>
#include <vector>

#include "Rtt_Event.h"
#include "Core/Rtt_Types.h"
#include "Rtt_Runtime.h"
#include "Rtt_LinuxRuntimeDelegate.h"
#include "Core/Rtt_Math.h"
#include "Core/Rtt_Array.h"
#include "Rtt_LinuxInputDeviceManager.h"
#include "Rtt_LinuxSimulatorServices.h"

#include "wx/app.h"
#include "wx/frame.h"
#include "wx/glcanvas.h"
#include <wx/timer.h>
#include "wx/fswatcher.h"

#pragma once

class MyApp;
class MyFrame;
class MyGLCanvas;

wxDECLARE_EVENT(eventOpenProject, wxCommandEvent);
wxDECLARE_EVENT(eventRelaunchProject, wxCommandEvent);
wxDECLARE_EVENT(eventWelcomeProject, wxCommandEvent);
wxDECLARE_EVENT(eventNewProject, wxCommandEvent);

namespace Rtt
{
	class LinuxPlatform;

	class LinuxRuntime : public Runtime
	{
	public:
		LinuxRuntime(const MPlatform& platform, MCallback *viewCallback = NULL)
			: Runtime(platform, viewCallback)
		{
		}

		void readSettings(int* w, int* h, std::string* orientation, std::string* title, std::string* mode);
		bool readTable(lua_State *L, const char* name, int* w, int* h, std::string* title, std::string* mode) const;
	};

	class KeyListener
	{
	public:
		KeyListener(Runtime &runtime);

		void notifyKeyEvent(wxKeyEvent& e, bool down);

	private:
		Runtime& fRuntime;
		std::map<int, std::string> fKeyName;
	};

	class MouseListener
	{
	public:
		MouseListener(Runtime &runtime);

		void TouchDown(int x, int y, int id);
		void TouchMoved(int x, int y, int id);
		void TouchUp(int x, int y, int id);
		void DispatchEvent(const MEvent& e) const;

		float	fScaleX, fScaleY;

	private:

		struct pt
		{
			pt() : x(0), y(0) {}
			pt(int xx, int yy) : x(xx), y(yy) {}
			int x;
			int y;
		};

		Runtime& fRuntime;
		std::map<int, pt> fStartPoint;		// finger id ==> point
	};


	// Immediately broadcast to "Runtime"
	class jsSystemEvent : public VirtualEvent
	{
	public:
		jsSystemEvent(const char* e)
			: fEventName(e)
		{}

		virtual const char* Name() const { return "system"; }
		virtual int Push(lua_State *L) const;

	private:
		std::string fEventName;

	};

	struct CoronaAppContext
	{
		CoronaAppContext(const char* path);
		~CoronaAppContext();

		void close();
		bool Init();
		bool IsInitialized() const { return NULL != fRuntime; }
		void Start();
		//		bool ProcessEvent(SDL_Event& event);
		void enumerateFontFiles(const char* dir, std::vector<std::string>& fileList);

		Runtime* GetRuntime() { return fRuntime; }
		const Runtime *GetRuntime() const { return fRuntime; }

		MouseListener* GetMouseListener() { return fMouseListener; }
		const MouseListener *GetMouseListener() const { return fMouseListener; }
		KeyListener* GetKeyListener() { return fKeyListener; }
		const KeyListener *GetKeyListener() const { return fKeyListener; }
		void pause();
		void resume();
		int getFPS() const	{ return fRuntime ? fRuntime->GetFPS() : 30; }
		int getWidth() const	{ return fRuntimeDelegate->fContentWidth; }
		int getHeight() const	{ return fRuntimeDelegate->fContentHeight; }
		DeviceOrientation::Type getOrientation() const	{ return fRuntimeDelegate->fOrientation; }
		const std::string& getTitle() const { return fTitle; }
		void flush();		// redraw
		bool loadApp(MyGLCanvas* canvas);
		MyGLCanvas* getCanvas() const { return fCanvas; }
		void setCanvas(MyGLCanvas* val) { fCanvas = val; }
		const char* getAppPath() const { return fPathToApp.c_str(); }
		LinuxPlatform* getPlatform() const { return fPlatform; }
		const std::string& getAppName() const { return fAppName; }
		const std::string& getSaveFolder() const { return fSaveFolder; }
		
		bool fIsStarted;

	private:

		std::string fTitle;
		LinuxRuntime* fRuntime;
		LinuxRuntimeDelegate* fRuntimeDelegate;
		MouseListener* fMouseListener;
		KeyListener* fKeyListener;
		std::string fPathToApp;
		std::string fAppName;

		LinuxPlatform* fPlatform;
		bool fTouchDeviceExist;
		std::string fMode;
		MyGLCanvas* fCanvas;
		bool fIsDebApp;
		LinuxSimulatorServices* fSimulator;
		std::string fSaveFolder;
	};

};

//
// Define application
//

// The canvas window
class MyGLCanvas : public wxGLCanvas
{
public:
	MyGLCanvas(MyFrame* parent, const wxGLAttributes& canvasAttrs);
	~MyGLCanvas();

	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);
	void OnTimer(wxTimerEvent& event);

	//Used just to know if we must end now because OGL 3.2 isn't available
	bool OglCtxAvailable()
	{
		return m_oglContext != NULL;
	}

	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnMouse(wxMouseEvent& event);
	
	void startTimer(float duration);

	//private:
	MyFrame*      m_parent;
	wxGLContext*  m_oglContext;
	int           m_winHeight; // We use this var to know if we have been sized
	Rtt::CoronaAppContext* fContext;
	wxTimer m_timer;

	wxDECLARE_EVENT_TABLE();
};

// The main frame class
class MyFrame : public wxFrame
{
public:
	MyFrame();
	virtual ~MyFrame();

	void OnFileSystemEvent(wxFileSystemWatcherEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnQuit(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnRelaunch(wxCommandEvent& event);
	void OnOpenFileDialog(wxCommandEvent& event);
	void OnOpenWelcome(wxCommandEvent& event);
	void OnBuildAndroid(wxCommandEvent& ev);
	void OnBuildWeb(wxCommandEvent& ev);
	void OnBuildLinux(wxCommandEvent& ev);
	
	void SetOGLString(const wxString& ogls) { m_OGLString = ogls; }

	MyGLCanvas* getCanvas() const { return m_mycanvas; }
	Rtt::CoronaAppContext* getContext() const { return fContext; }
	void resetSize();
	void setMenu(const char* appPath);
	void createMenus();
	void watchFolder(const char* path, const char* appName);

//private:

	wxString     m_OGLString;
	MyGLCanvas*  m_mycanvas;
	Rtt::CoronaAppContext* fContext;

	wxMenuBar* fMenuMain;
	wxMenuBar* fMenuProject;
	std::string fAppPath;
	wxFileSystemWatcher* fWatcher;

	wxDECLARE_EVENT_TABLE();
};

wxDECLARE_APP(MyApp);
class MyApp : public wxApp
{
public:
	MyApp();
	virtual ~MyApp();

	bool OnInit() wxOVERRIDE;

	MyFrame* getFrame() { return fFrame; }
	MyGLCanvas* getCanvas() const { return fFrame->getCanvas(); }
	wxWindow* getParent();
	Rtt::LinuxPlatform* getPlatform() const;
	Rtt::Runtime* GetRuntime() { return fFrame->getContext()->GetRuntime(); };
	
private:
	MyFrame* fFrame;
	//Rtt::CoronaAppContext* fCoronaContext;
};

