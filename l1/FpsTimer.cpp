// 
// FpsTimer.cpp
// 
// FpsTimer class definision
// 
////////////////////////////////////////

#include "FpsTimer.h"

using namespace std::chrono;




FpsTimer::FpsTimer() {
	last = steady_clock::now();
}

float FpsTimer::Mark() {
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float FpsTimer::Peek() const {
	return duration<float>(steady_clock::now() - last).count();
}