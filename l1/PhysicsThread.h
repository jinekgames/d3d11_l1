
#pragma once

#include <thread>
#include <mutex>
#include "FpsTimer.h"


#define GRAVITY_CONST 0.4f

class PhysicsThread {

public:

	PhysicsThread() noexcept :
		r(0), dr(0), ddr(0), dt(0), breakThread(false)
	{}
	//PhysicsThread(PhysicsThread& physClass) noexcept :
	//	thr(threadFunction, physClass) {
	//
	//	thr.detach();
	//
	//}
	PhysicsThread(const PhysicsThread& t) = delete;
	PhysicsThread& operator=(const PhysicsThread& t) = delete;
	~PhysicsThread() {
		breakThread = true;
	}

	std::mutex _lock;
	//std::thread thr;

	float r;
	float dr;
	float ddr;
	float dt;

	bool breakThread;

	inline void Calculations(/*float& cr, float& cdr, float& cddr, float& cdt*/) {

		static float cr = 14.0f, cdr, cddr, cdt;
		static FpsTimer Timer;

		while (true) {

			cdt = Timer.Mark() / 10000;

			if (abs(cr) > 20) {
				//std::mt19937 rng(std::random_device{}());
				//std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
				//r = rdist(rng);
				cdr = 0;
			}

			cddr = ((cr > 0) ? -1 : 1) * GRAVITY_CONST / (cr * cr);
			cdr = cdr + cddr * cdt;
			cr = cr + cdr + cddr * cdt * cdt / 2;

			if (this != nullptr) {
				_lock.lock();
				r = cr;
				dr = cdr;
				_lock.unlock();
			}


			if (breakThread) {
				break;
			}

			//Sleep(1);


		}

	}

};



