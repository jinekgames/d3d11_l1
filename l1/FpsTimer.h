// 
// FpsTimer.h
// 
// FpsTimer class declaration
// 
////////////////////////////////////////

#pragma once

#include <chrono>



class FpsTimer {

public:
	FpsTimer() noexcept;
	float Mark() noexcept;
	float Peek() const noexcept;

private:
	std::chrono::steady_clock::time_point last;

};
