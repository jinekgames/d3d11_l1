// 
// WindowSettings.h
// 
// main settings of creating window
// 
////////////////////////////////////////

#pragma once

//#define WND_WIDTH 800
//#define WND_HEIGTH 800
//#define WND_NAME L"sosi"
#define WND_CLASS_NAME L"deep dark fantasy"
#define VIDEO_ADAPTER //sets in cmd line 1 arg


struct wndsets {
	UINT adapter;
	UINT hardness;
	UINT wndWidth;
	UINT wndHeigth;
	DXGI_RATIONAL screenScale;
	LPCWSTR wndName;
};
