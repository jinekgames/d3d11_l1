// 
// App.cpp
// 
// App class definition
// 
////////////////////////////////////////

#include "App.h"
#include "Melon.h"
#include "Pyramid.h"
#include "Box.h"
#include <memory>
#include <algorithm>
#include "MathModule.h"

#include <sstream>
#include <iomanip>



#define FPS_DOINGS_JNK	const float t = timer.Peek();	\
		const float fps = 1.0f / t;	\
		static UINT sumCount = 40; \
		static std::vector<UINT> fps10sum(sumCount); \
		static UINT rewriteIndex = 0;	\
		fps10sum[rewriteIndex] = fps;	\
		rewriteIndex = (rewriteIndex + 1) % sumCount;	\
		UINT fps10summed = 0;	\
		for (auto i : fps10sum) {	\
			fps10summed += i;	\
		}	\
		std::ostringstream oss;	\
		oss << "FPS: " << fps10summed / sumCount;	\
		wnd.SetTitle(oss.str());	\
		fpsLog.AddRec(fps);

#define GRAVITY_CONST 0.4f
void radRule(float& dt, float& dr, float& r) {

	if (abs(r) > 20) {
		//std::mt19937 rng(std::random_device{}());
		//std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
		//r = rdist(rng);
		dr = 0;
	}

	float ddr = ((r > 0) ? -1 : 1) * GRAVITY_CONST / (r * r);
	dr = dr + ddr * dt;
	r = r + dr + ddr * dt*dt / 2;

}



App::App(wndsets& WndSets)
	:
	wnd(WndSets),
	nDrawables(WndSets.hardness) {


	class Factory {

	public:
		Factory(Graphics& gfx, void (*RadRule)(float&, float&, float&))
			:
			gfx(gfx),
			RadRule(RadRule)
		{}

		std::unique_ptr<Drawable> operator()() {
			switch (/*typedist(rng)*/1) {
			case 0:
				return std::make_unique<Pyramid>(
					gfx, rng, adist, ddist,
					odist, rdist, RadRule
					);
			case 1:
				return std::make_unique<Box>(
					gfx, rng, adist, ddist,
					odist, rdist, bdist, RadRule
					);
			case 2:
				return std::make_unique<Melon>(
					gfx, rng, adist, ddist,
					odist, rdist, longdist, latdist
					);
			default:
				assert(false && "bad drawable type in factory");
				return {};
			}
		}

	private:
		Graphics& gfx;
		std::mt19937 rng{ std::random_device{}() };
		std::uniform_real_distribution<float> adist{ 0.0f,PI * 2.0f };
		std::uniform_real_distribution<float> ddist{ 0.0f,PI * 0.5f };
		std::uniform_real_distribution<float> odist{ 0.0f,PI * 0.08f };
		std::uniform_real_distribution<float> rdist{ 6.0f,20.0f };
		std::uniform_real_distribution<float> bdist{ 0.4f,3.0f };
		std::uniform_int_distribution<int> latdist{ 5,20 };
		std::uniform_int_distribution<int> longdist{ 10,40 };
		std::uniform_int_distribution<int> typedist{ 0,2 };
		void (*RadRule)(float&, float&, float&);
	};

	Factory f(wnd.Graph(), radRule);
	drawables.reserve(nDrawables);
	std::generate_n(std::back_inserter(drawables), nDrawables, f);
	wnd.Graph().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, ((float) WndSets.screenScale.Numerator ) / WndSets.screenScale.Denominator, 0.5f, 40.0f));

}

App::~App() {}

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

	auto dt = timer.Mark();


	// Game logic here
	wnd.GameProc();


	static FpsTimer colorTimerBck;
	float cbck = abs(sin(colorTimerBck.Peek()));
	wnd.Graph().ClearBackBuffer(0.8 + cbck / 8.0f, 0.1f + cbck / 2.0f, 0.4f + cbck / 3.0f);

	for (auto& d : drawables) {
		d->Update(dt);
		d->Draw(wnd.Graph());
	}

	wnd.Graph().SwapBuffers();



	FPS_DOINGS_JNK
}