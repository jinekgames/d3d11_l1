// 
// App.cpp
// 
// App class definition
// 
////////////////////////////////////////

#include "App.h"

#include <sstream>
#include <iomanip>



App::App()
	:
	wnd(WND_WIDTH, WND_HEIGTH, WND_NAME)
{}

int App::Go()
{

	while ( true ) {

		// process all messages pending, but to not block for new messages
		if (const auto ecode = Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}

		DoFrame();

	}

}

void App::DoFrame() {

	timer.Mark();


	// Game logic here
	wnd.GameProc();

	static int c = 1;
	if ( (colorTimer.Peek() * 1000) > 200 ) {
		colorTimer.Mark();
		c = 1 - c;
	}
	wnd.Graph().ClearBackBuffer(1.0f * c, 0,0);
	wnd.Graph().SwapBuffers();


	// timer test (frame time view)
	const float t = colorTimer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " /*<< std::setprecision(3) << std::fixed*/ << t*1000 << "s";
	wnd.SetTitle(oss.str());

}
