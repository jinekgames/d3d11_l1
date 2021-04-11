// 
// FpsTimer.h
// 
// FpsTimer class description
// 
////////////////////////////////////////

#pragma once

#include <chrono>



class FpsTimer {

public:
	FpsTimer();
	float Mark();
	float Peek() const;

private:
	std::chrono::steady_clock::time_point last;

};
