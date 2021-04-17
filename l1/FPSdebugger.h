// 
// FPSdebugger.h
// 
// Class for debugging framerate/frametime loges to defined file
// 
////////////////////////////////////////

#pragma once

#include <windows.h>
#include <fstream>



#define DEFAULT_FILENAME "C:\\Games\\Jinek_Games\\d3d11test\\logs.txt"


class FPSdebugger {

public:
	FPSdebugger(LPCSTR filename = DEFAULT_FILENAME) noexcept;
	FPSdebugger(const FPSdebugger&) = delete;
	FPSdebugger& operator=(const FPSdebugger&) = delete;
	~FPSdebugger() noexcept;

public:
	bool IsFileOpened() noexcept;
	void AddRec(float value) noexcept;

private:
	std::ofstream fout;
	bool fileOpened;
	UINT frameNumber;

};
