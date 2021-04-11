// 
// FpsTimer.cpp
// 
// FpsTimer class definition
// 
////////////////////////////////////////

#include "FpsTimer.h"

using namespace std::chrono;




FpsTimer::FpsTimer() noexcept {
	last = steady_clock::now();
}

float FpsTimer::Mark() noexcept {
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float FpsTimer::Peek() const noexcept {
	return duration<float>(steady_clock::now() - last).count();
}