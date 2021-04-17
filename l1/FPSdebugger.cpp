
#include "FPSdebugger.h"



#define ERROR_FILEOPEN_MSG "\n=======================\nFPSdebugger\nerror at file opening\n=======================\n\n"


FPSdebugger::FPSdebugger(LPCSTR filename) noexcept
	:
	fout(filename, std::ios_base::out),
	frameNumber(0) {

	if (!fout.is_open()) {
		fileOpened = false;
		MessageBoxA(0, ERROR_FILEOPEN_MSG, "FPSdebugger", MB_ICONERROR);
#ifndef NDEBUG
		OutputDebugStringA(ERROR_FILEOPEN_MSG);
		return;
#endif // !NDEBUG

	}

	fileOpened = true;

	fout << "Excel-compatible framerate/time log from jnk_gms app\n"
		<< "Frame\tValue\n";

}

FPSdebugger::~FPSdebugger() noexcept {

	if (fileOpened) {
		fout.close();
	}

}

bool FPSdebugger::IsFileOpened() noexcept {
	return fileOpened;
}

void FPSdebugger::AddRec(float value) noexcept {

	if (!fileOpened)	return;

	++frameNumber;
	fout << frameNumber << "\t" << value << "\n";

}


