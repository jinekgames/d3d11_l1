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

	//wnd.Graph().ConfigureTargetAndViewport();

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

	//timer.Mark();


	// Game logic here
	wnd.GameProc();

	//static int c = 1;
	//static FpsTimer colorTimer;
	//if ((colorTimer.Peek() * 1000) > 200) {
	//	colorTimer.Mark();
	//	c = 1 - c;
	//}
	//wnd.Graph().ClearBackBuffer(1.0f * c, 0, 0);

	static FpsTimer colorTimerBck;
	float cbck = abs(sin(colorTimerBck.Peek()));
	wnd.Graph().ClearBackBuffer(0.8 + cbck / 8.0f, 0.1f + cbck / 2.0f, 0.4f + cbck / 3.0f);



	wnd.Graph().DrawTestTriangle(
		-timer.Peek(),
		0.0f,
		0.0f
	);
	wnd.Graph().DrawTestTriangle(
		timer.Peek(),
		wnd.mouse.GetPosX() / 400.0f - 1.0f,
		-wnd.mouse.GetPosY() / 300.0f + 1.0f
	);





	/*
	static FpsTimer colorTimerBck;
	float cbck = abs( sin(colorTimerBck.Peek()) );
	wnd.Graph().ClearBackBuffer( 0.8 + cbck / 8.0f, 0.1f + cbck / 2.0f, 0.4f + cbck / 3.0f);

	static FpsTimer colorTimer;
	UCHAR cr = abs(sin(colorTimer.Peek()) * 255);
	UCHAR c = abs(sin(sin(colorTimer.Peek() * 2) * 0.37) * 255);
	UCHAR cb = abs(sin(colorTimer.Peek() + 3743) * 255);


	wnd.Graph().SetShadersColor(L"Shaders\\PixelShaderColor.cso", L"Shaders\\VertexShaderColor.cso");

	// create vertex buffer
	//const Graphics::VertexColor ears[] = {
	//	{	-0.5f,	1.0f - 0.25f,	cr,c,cb	},
	//	{	-0.25f,	0.5f - 0.25f,	cr,c,cb	},
	//	{	-0.5f,	0.5f - 0.25f,	cr,c,cb	},
	//	{	0.25f,	0.5f - 0.25f,	cr,c,cb	},
	//	{	0.5f,	1.0f - 0.25f,	cr,c,cb	},
	//	{	0.5f,	0.5f - 0.25f,	cr,c,cb	},
	//};
	//const Graphics::VertexColor bigDownPart[] = {
	//	{	-0.5f,	0.5f - 0.25f,	cr,c,cb	},
	//	{	0.5f,	0.5f - 0.25f,	cr,c,cb	},
	//	{	-0.5f,	-0.25f - 0.25f,	cr,c,cb	},
	//	{	0.5f,	-0.25f - 0.25f,	cr,c,cb	},
	//	{	-0.25f,	-0.5f - 0.25f,	cr,c,cb	},
	//	{	0.25f,	-0.5f - 0.25f,	cr,c,cb	},
	//};
	
	const Graphics::VertexColor heart1strip[] = {
		{	-0.5f,	0.5f,	cr,c,cb },
		{	-0.25f,	0.5f,	cr,c,cb },
		{	-0.75f,	0.25f,	cr,c,cb },
		{	0,		0.25f,	cr,c,cb },
	};
	const Graphics::VertexColor heart2strip[] = {
		{	0.25f,	0.5f,	cr,c,cb },
		{	0.5f,	0.5f,	cr,c,cb },
		{	0,		0.25f,	cr,c,cb },
		{	0.75f,	0.25f,	cr,c,cb },
	};
	const Graphics::VertexColor heart3strip[] = {
		{	-0.75f,	0.25f,	cr,c,cb },
		{	0.75f,	0.25f,	cr,c,cb },
		{	-0.75f,	0,		cr,c,cb },
		{	0.75f,	0,		cr,c,cb },
		{	-0.25f,	-0.5,	cr,c,cb },
		{	0.25f,	-0.5,	cr,c,cb },
	};
	VERTICES_DRAW_CALL_U_COLOR(heart1strip, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	VERTICES_DRAW_CALL_U_COLOR(heart2strip, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	VERTICES_DRAW_CALL_U_COLOR(heart3strip, D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	*/


	wnd.Graph().SwapBuffers();


	// timer test (frame time view)
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed: " << std::setprecision(3) << std::fixed << t*1000 << "s";
	//wnd.SetTitle(oss.str());

}
