// 
// Window.cpp
// 
// a window class definition
// 
////////////////////////////////////////

#include "Window.h"

#include <Mmsystem.h>
#include <string>
#include <sstream>
#include "resource.h"



// Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hinstance(GetModuleHandle(nullptr)) {

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(hinstance, MAKEINTRESOURCE(IDI_ICON0), IMAGE_ICON, 64,64,0));
	wc.hCursor = static_cast<HCURSOR>(LoadImage(hinstance, MAKEINTRESOURCE(IDI_CUR0), IMAGE_CURSOR, 32, 32, 0));;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(hinstance, MAKEINTRESOURCE(IDI_ICON0), IMAGE_ICON, 16, 16, 0));;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass() {
	UnregisterClass(wndClassName, GetInstance());
}

LPCWSTR Window::WindowClass::GetName() noexcept {
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept {
	return wndClass.hinstance;
}


// Window Stuff
Window::Window(int width, int height, LPCWSTR name) 
	:
	width(width), height(height) {


	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
		throw CHWND_LAST_EXCEPT();
	}

	// create window & get hwnd
	hwnd = CreateWindow(
		WindowClass::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);

	// check for error
	if (hwnd == nullptr) {
		throw CHWND_LAST_EXCEPT();
	}


	// show window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	// create graphics object
	pGraph = std::make_unique<Graphics>(hwnd);
}

Window::~Window() {
	DestroyWindow(hwnd);
}

void Window::SetTitle(const std::string& title) {
	if (SetWindowTextA(hwnd, title.c_str()) == 0) {
		throw CHWND_LAST_EXCEPT();
	}
}

std::optional<int> Window::ProcessMessages() noexcept {

	// message variable
	MSG msg;

	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {

		// check for quit because peekmessage does not signal this via return val
		if (msg.message == WM_QUIT) {
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return (int)msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	// return empty optional when not quitting app
	return {};
	
}

Graphics& Window::Graph() {
	if (!pGraph)
	{
		throw CHWND_NOGFX_EXCEPT();
	}
	return *pGraph;
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE) {
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pwnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pwnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pwnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept {
	switch (msg) {
		case WM_CREATE:		// Initialisation
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
		case WM_KILLFOCUS:
		{
			keyboard.ClearState();
		} break;

		// ----- Keyboard handling ----- //
		case WM_CHAR:
		{
			keyboard.OnChar(static_cast<unsigned char>(wparam));
		} break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:	//	(.Y.)
		{
			// filter of sleeping keys
			if ( !(lparam & 0x40000000) || keyboard.AutorepeatIsEnabled() ) {
				keyboard.OnKeyPressed(static_cast<unsigned char>(wparam));
			}
		} break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			keyboard.OnKeyReleased(static_cast<unsigned char>(wparam));
		} break;
		// -END- Keyboard handling ----- //

		// ----- Mouse handling ----- //
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lparam);
			// in client region -> log move, and log enter + capture mouse (if not previously in window)
			if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
			{
				mouse.OnMouseMove(pt.x, pt.y);
				if (!mouse.IsInWindow())
				{
					SetCapture(hwnd);
					mouse.OnMouseEnter();
				}
			}
			// not in client -> log move / maintain capture if button down
			else
			{
				if (wparam & (MK_LBUTTON | MK_RBUTTON))
				{
					mouse.OnMouseMove(pt.x, pt.y);
				}
				// button up -> release capture / log event for leaving
				else
				{
					ReleaseCapture();
					mouse.OnMouseLeave();
				}
			}
		} break;
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lparam);
			mouse.OnLeftPressed(pt.x, pt.y);
		} break;
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lparam);
			mouse.OnRightPressed(pt.x, pt.y);
		} break;
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lparam);
			mouse.OnLeftReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		} break;
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lparam);
			mouse.OnRightReleased(pt.x, pt.y);
			// release mouse if outside of window
			if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		} break;
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lparam);
			const int delta = GET_WHEEL_DELTA_WPARAM(wparam);
			mouse.OnWheelDelta(pt.x, pt.y, delta);
		} break;
		// -END- Mouse handling ----- //

		//case WM_PAINT:		// Window redrawing
		//{
		//	drawCallNum++;

		//	//Объявление всего окна недействительным
		//	InvalidateRect(hwnd, NULL, TRUE);

		//	//перерисовка окна
		//	//hdc = BeginPaint(hwnd, &ps);
		//	//
		//	//EndPaint(hwnd, &ps);

		//	return 0;
		//} break;
		case WM_DESTROY:	// Closing
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

void Window::GameProc() {

	// sm game logic that need wnd private variables

}



// Window Exception Stuff

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept {
	char* pMsgBuf = nullptr;

	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);

	// 0 string length returned indicates a failure
	if (nMsgLen == 0) {
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept
	:
	Exception(line, file),
	hr(hr)
{}

const char* Window::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::HrException::GetType() const noexcept
{
	return "Chili Window Exception";
}

HRESULT Window::HrException::GetErrorCode() const noexcept {
	return hr;
}

std::string Window::HrException::GetErrorDescription() const noexcept {
	return Exception::TranslateErrorCode(hr);
}


const char* Window::NoGfxException::GetType() const noexcept
{
	return "Window Exception [No Graphics]";
}
