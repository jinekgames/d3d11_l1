// 
// Window.h
// 
// a window class declaration
// 
////////////////////////////////////////

#pragma once

#include "MainConsts.h"
#include "Exception.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include <optional>
#include <memory>

#include "WindowSettings.h"



class Window {

public:
	class Exception : public myException {
		using myException::myException;
	public:
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
	};
	class HrException : public Exception {
	public:
		HrException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGfxException : public Exception {
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};

private:
	// singleton manages registration/cleanup of window class
	class WindowClass {
	public:
		static LPCWSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
	private:
		static constexpr LPCWSTR wndClassName = WND_CLASS_NAME;
		static WindowClass wndClass;
		HINSTANCE hinstance;
	};

public:
	Window(int width, int height, LPCWSTR name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages() noexcept;
	Graphics& Graph();

private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
	Keyboard keyboard;
	Mouse mouse;

public:
	void GameProc();

private:
	int width;
	int height;
	HWND hwnd;
	std::unique_ptr<Graphics> pGraph;

	// number of WM_DRAW calls
	WORD drawCallNum = 0;
};
