// 
// Graphics.h
// 
// Graphics class declaration
// 
////////////////////////////////////////

#pragma once

#pragma warning (disable : 4996)

#include "MainConsts.h"
#include "Exception.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>

#define _CRT_SECURE_NO_WARNINGS



class Graphics {

	friend class Bindable;

public:
	class Exception : public myException {
		using myException::myException;
	};
	class HrException : public Exception {
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception {
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException {
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};

public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void ConfigureTargetAndViewport();
	void SwapBuffers();
	void ClearBackBuffer( float red, float green, float blue );
	void DrawIndexed(UINT count) noexcept(!IS_DEBUG);
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;


// jnk_gms code
// for drawing triangles
public:
	struct Vertex {
		float x;
		float y;
	};
	struct VertexColor
	{
		struct {
			float x;
			float y;
		} coords;
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};
private:
	bool isShaderLoded;
public:
	bool IsShaderLoded();
	void SetShaders(LPCWSTR pixelShaderPath, LPCWSTR vertexShaderPath);
	void SetShadersColor(LPCWSTR pixelShaderPath, LPCWSTR vertexShaderPath);
	void ReleaseShaders(); // does absolutely nothing
public:
#define RANDOM_COORD	(float)(rand() % 100) / 50 - 1.0f
#define VERTICES_DRAW_CALL_U(vertices, primitivesTopology)	wnd.Graph().DrawTriangleU(vertices, std::size(vertices), sizeof(vertices), primitivesTopology);
#define VERTICES_DRAW_CALL_U_COLOR(vertices, primitivesTopology)	wnd.Graph().DrawTriangleUColor(vertices, std::size(vertices), sizeof(vertices), primitivesTopology);
#define VERTICES_DRAW_CALL(vertices, primitivesTopology, pixelShaderPath, vertexShaderPath)	wnd.Graph().DrawTriangle(vertices, std::size(vertices), sizeof(vertices), primitivesTopology, pixelShaderPath, vertexShaderPath);
#define VERTICES_DRAW_CALL_COLOR(vertices, primitivesTopology, pixelShaderPath, vertexShaderPath)	wnd.Graph().DrawTriangleColor(vertices, std::size(vertices), sizeof(vertices), primitivesTopology, pixelShaderPath, vertexShaderPath);
	void DrawTriangleU(const void* vertices, UINT vertexCount, UINT sizeofVertices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology);
	void DrawTriangleUColor(const void* vertices, UINT vertexCount, UINT sizeofVertices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology);
	void DrawTriangle(const void* vertices, UINT vertexCount, UINT sizeofVertices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology, LPCWSTR pixelShaderPath, LPCWSTR vertexShaderPath);
	void DrawTriangleColor(const void* vertices, UINT vertexCount, UINT sizeofVertices, D3D_PRIMITIVE_TOPOLOGY primitiveTopology, LPCWSTR pixelShaderPath, LPCWSTR vertexShaderPath);
// --



private:
	DirectX::XMMATRIX projection;
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

};