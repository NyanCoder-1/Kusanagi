#include "view/render/Render-DX11.h"

#include "utils/Exception.h"
#include "Core.h"
#include "window/WinWindow.h"

#include <DirectXColors.h>


Kusanagi::View::Render::RenderDX11::RenderDX11(Kusanagi::Core *c)
	: Kusanagi::View::Render::IRender(c) {
	driverType = D3D_DRIVER_TYPE_HARDWARE;
	featureLevel = D3D_FEATURE_LEVEL_11_1;
}
Kusanagi::View::Render::RenderDX11::~RenderDX11() {
	if (d3dImmediateContext) d3dImmediateContext->ClearState();
}


void Kusanagi::View::Render::RenderDX11::Init() {
	createDeviceAndContext();
	createSwapChain();

	createRenderTarget();
	createDepthStencil();
	d3dImmediateContext->OMSetRenderTargets(1, d3dRTV.GetAddressOf(), d3dDSV.Get());

	// Window size
	RECT rc;
	GetClientRect(reinterpret_cast<Kusanagi::Window::WinWindow*>(core->GetWindow())->GetHandler(), &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	d3dImmediateContext->RSSetViewports(1, &vp);
}
void Kusanagi::View::Render::RenderDX11::DrawBegin() {
	d3dImmediateContext->ClearRenderTargetView(d3dRTV.Get(), DirectX::Colors::OliveDrab);
	d3dImmediateContext->ClearDepthStencilView(d3dDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void Kusanagi::View::Render::RenderDX11::DrawEnd() {
	swapChain->Present(0, 0);
}
void Kusanagi::View::Render::RenderDX11::Resize(unsigned long width, unsigned long height) {

}

ID3D11Device1 * Kusanagi::View::Render::RenderDX11::GetDevice() {
	return d3dDevice.Get();
}

ID3D11DeviceContext1 * Kusanagi::View::Render::RenderDX11::GetDeviceContext() {
	return d3dImmediateContext.Get();
}


void Kusanagi::View::Render::RenderDX11::createDeviceAndContext() {
	HRESULT hr = S_OK;
	UINT createDeviceFlags = 0;
#if defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// Supported feature level list
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _immediateContext;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
		driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, _device.GetAddressOf(), &featureLevel, _immediateContext.GetAddressOf());
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("", 0x02000001, __FILE__, __LINE__);

	_device.As(&d3dDevice);
	_immediateContext.As(&d3dImmediateContext);
}
void Kusanagi::View::Render::RenderDX11::createSwapChain()
{
	HWND hWnd = reinterpret_cast<Kusanagi::Window::WinWindow*>(core->GetWindow())->GetHandler();
	// Window size
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	Microsoft::WRL::ComPtr<IDXGIDevice> _device;
	d3dDevice.As(&_device);

	Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter = 0;
	HRESULT hr = _device->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(dxgiAdapter.GetAddressOf()));
	if (FAILED(hr))
		throw Kusanagi::Utils::Exception("Can't create DXGI swap chain", 0x02000002, __FILE__, __LINE__);
	Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory = 0;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory.GetAddressOf()));
	if (FAILED(hr))
		throw Kusanagi::Utils::Exception("Can't create DXGI swap chain", 0x02000003, __FILE__, __LINE__);

	Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory2 = 0;
	if ((featureLevel >= D3D_FEATURE_LEVEL_11_1) && SUCCEEDED(dxgiFactory.As(&dxgiFactory2))) {
		// This system has DirectX 11.1 or later installed, so we can use this interface
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC1));
		sd.BufferCount = 1;
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC sfsd;
		ZeroMemory(&sfsd, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));
		sfsd.RefreshRate.Numerator = 75;
		sfsd.RefreshRate.Denominator = 1;
		sfsd.Scaling = DXGI_MODE_SCALING_STRETCHED;
		sfsd.Windowed = TRUE;
		
		Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
		dxgiFactory2->CreateSwapChainForHwnd(d3dDevice.Get(), hWnd, &sd, &sfsd, nullptr, _swapChain.GetAddressOf());
		dxgiFactory2->Release();
		_swapChain.As(&swapChain);
	}
	else {
		// This system only has DirectX 11.0 installed
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 75;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		dxgiFactory->CreateSwapChain(d3dDevice.Get(), &sd, swapChain.GetAddressOf());
	}
}

void Kusanagi::View::Render::RenderDX11::createRenderTarget() {
	ID3D11Texture2D* pBackBuffer = NULL;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("Can't create render target", 0x02000004, __FILE__, __LINE__);

	hr = d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, d3dRTV.GetAddressOf());
	pBackBuffer->Release();
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("Can't create render target", 0x02000005, __FILE__, __LINE__);
}

void Kusanagi::View::Render::RenderDX11::createDepthStencil() {
	// Window size
	RECT rc;
	GetClientRect(reinterpret_cast<Kusanagi::Window::WinWindow*>(core->GetWindow())->GetHandler(), &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = d3dDevice->CreateTexture2D(&descDepth, NULL, &d3dDepthStencil);
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("Depth stencil texture creating failed", 0x02000006, __FILE__, __LINE__);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = d3dDevice->CreateDepthStencilView(d3dDepthStencil.Get(), &descDSV, d3dDSV.GetAddressOf());
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("Depth stencil view creating failed", 0x02000007, __FILE__, __LINE__);
}

void Kusanagi::View::Render::RenderDX11::addDeferredContexts() {

}