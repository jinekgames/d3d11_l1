// 
// Window.h
// 
// a window class description
// 
////////////////////////////////////////

#pragma once

#include "MainConsts.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"



class Window {

public:
	class wndException : public myException
	{
	public:
		wndException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
};

private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	private:
		static constexpr LPCWSTR wndClassName = L"deep dark fantasy";
		static WindowClass wndClass;
		HINSTANCE hinstance;
	};

public:
	Window(int width, int height, LPCWSTR name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Keyboard keyboard;
	Mouse mouse;
	void GameProc();

private:
	int width;
	int height;
	HWND hwnd;

	// number of WM_DRAW calls
	WORD drawCallNum = 0;
};



// error exception helper macro
#define CHWND_EXCEPT( hr ) Window::wndException( __LINE__,__FILE__,hr )
#define CHWND_LAST_EXCEPT() Window::wndException( __LINE__,__FILE__,GetLastError() )