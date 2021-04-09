// 
// Window.cpp
// 
// a window class definision
// 
////////////////////////////////////////

#include "Window.h"
#include <string>


// Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hinstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

LPCWSTR Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hinstance;
}


// Window Stuff
Window::Window(int width, int height, LPCWSTR name) noexcept
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// create window & get hwnd
	hwnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	// show window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hwnd);
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pwnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pwnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window class handler
		return pwnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
{
	switch (msg)
	{
	case WM_CREATE:	// Initialisation
	{
		return 0;
	} break;
	case WM_MOVE:
	{
		//windowCoords.x = LOWORD(lparam);
		//windowCoords.y = HIWORD(lparam);
		return 0;
	} break;
	case WM_SIZE:
	{
		//windowCurSizeX = LOWORD(lparam);
		//windowCurSizeY = HIWORD(lparam);
		return 0;
	} break;
	case WM_MOUSEMOVE:
	{
		//cursorCoords.x = LOWORD(lparam);
		//cursorCoords.y = HIWORD(lparam);
		return 0;
	} break;
	case WM_CHAR:
	{
		static std::string windowName;
		windowName += wparam;
		SetWindowTextA(hwnd, windowName.c_str());
	} break;
	case WM_KEYDOWN:
	{

		// Debugging
		static char but[5];
		but[0] = wparam;
		std::string debugStr = but;
		debugStr += "\n";
		OutputDebugStringA(debugStr.c_str());
		////////////

		switch (wparam)
		{
		case VK_ESCAPE:
		{
			OutputDebugStringA("esc\n");
			return PostMessage(hwnd, WM_CLOSE, 0, 0);
		} break;

		}
	} break;
	case WM_PAINT:	//Обновление
	{
		drawCallNum++;

		//Объявление всего окна недействительным
		//InvalidateRect(hwnd, NULL, TRUE);

		//перерисовка окна
		//hdc = BeginPaint(hwnd, &ps);
		//
		//EndPaint(hwnd, &ps);

		return 0;
	} break;
	case WM_DESTROY:	//Закрытие
	{
		PostQuitMessage(300);
		return 0;
	} break;
	case WM_CLOSE:
	{
		return SendMessage(hwnd, WM_DESTROY, 0, 0);
	} break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}
