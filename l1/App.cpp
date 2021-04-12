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

	wnd.Graph().ConfigureTargetAndViewport();

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

	/*const float c = sin(colorTimer.Peek()) / 2.0f + 0.5f;
	wnd.Graph().ClearBackBuffer(c, c, 1.0f);*/


	const Graphics::Vertex earLeft[] = {
		{	-0.5f,	1.0f	- 0.25f	},
		{	-0.25f,	0.5f	- 0.25f	},
		{	-0.5f,	0.5f	- 0.25f	},
	};
	const Graphics::Vertex earRight[] = {
		{	0.25f,	0.5f	- 0.25f	},
		{	0.5f,	1.0f	- 0.25f	},
		{	0.5f,	0.5f	- 0.25f	},
	};
	const Graphics::Vertex bigDownPart[] = {
		{	-0.5f,	0.5f	- 0.25f	},
		{	0.5f,	0.5f	- 0.25f	},
		{	-0.5f,	-0.25f	- 0.25f	},
		{	0.5f,	-0.25f	- 0.25f	},
		{	-0.25f,	-0.5f	- 0.25f	},
		{	0.25f,	-0.5f	- 0.25f	},
	};

	wnd.Graph().SetShaders(L"Shaders\\PixelShader.cso", L"Shaders\\VertexShader.cso");
	VERTICES_DRAW_CALL_U(earLeft, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VERTICES_DRAW_CALL_U(earRight, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	VERTICES_DRAW_CALL_U(bigDownPart, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	wnd.Graph().SwapBuffers();


	// timer test (frame time view)
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(3) << std::fixed << t*1000 << "s";
	wnd.SetTitle(oss.str());

}
