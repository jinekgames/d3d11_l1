// 
// App.h
// 
// App class declaration
// 
////////////////////////////////////////

#pragma once

#include "Window.h"
#include "FpsTimer.h"
#include "FPSdebugger.h"




class App {

public:
	App(wndsets& wndSets);
	// master frame / message loop
	int Go();
	~App();

private:
	void DoFrame();

private:
	Window wnd;
	FpsTimer timer;
	FPSdebugger fpsLog;
	std::vector<std::unique_ptr<class Drawable>> drawables;
	UINT nDrawables;

};
