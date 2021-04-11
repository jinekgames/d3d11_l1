// 
// App.cpp
// 
// App class definision
// 
////////////////////////////////////////

#include "App.h"

#include <sstream>
#include <iomanip>



App::App()
	:
	wnd(400, 400, L"sosi")
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
	Sleep(16);


	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(3) << std::fixed << t << "s";
	wnd.SetTitle(oss.str());

}
