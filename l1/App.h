// 
// App.h
// 
// App class declaration
// 
////////////////////////////////////////

#pragma once

#include "Window.h"
#include "FpsTimer.h"



class App {

public:
	App();
	// master frame / message loop
	int Go();

private:
	void DoFrame();

private:
	Window wnd;
	FpsTimer timer;
	FpsTimer colorTimer;

};
