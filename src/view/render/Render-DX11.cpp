#include "view/render/Render-DX11.h"

#include "utils/Exception.h"
#include "Core.h"
#include "window/WinWindow.h"

#include <DirectXColors.h>


Kusanagi::View::Render::RenderDX11::RenderDX11(Kusanagi::Core *c)
	: Kusanagi::View::Render::IRender(c)
{
	driverType = D3D_DRIVER_TYPE_HARDWARE;
	featureLevel = D3D_FEATURE_LEVEL_11_1;
}
Kusanagi::View::Render::RenderDX11::~RenderDX11()
{
	if (d3dImmediateContext) d3dImmediateContext->ClearState();
}


void Kusanagi::View::Render::RenderDX11::Init()
{
	createDeviceAndContext();
	createSwapChain();
	createRenderTarget();

	// Подключаем объект заднего буфера к контексту устройства
	d3dImmediateContext->OMSetRenderTargets(1, d3dRTV.GetAddressOf(), d3dDSV.Get());

	RECT rc;
	GetClientRect(reinterpret_cast<Kusanagi::Window::WinWindow*>(core->GetWindow())->GetHandler(), &rc);
	UINT width = rc.right - rc.left;           // получаем ширину
	UINT height = rc.bottom - rc.top;   // и высоту окна
	// Настройка вьюпорта
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	// Подключаем вьюпорт к контексту устройства
	d3dImmediateContext->RSSetViewports(1, &vp);
}
void Kusanagi::View::Render::RenderDX11::DrawBegin()
{
	// Просто очищаем задний буфер
	d3dImmediateContext->ClearRenderTargetView(d3dRTV.Get(), DirectX::Colors::OliveDrab);
	d3dImmediateContext->ClearDepthStencilView(d3dDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0)
}
void Kusanagi::View::Render::RenderDX11::DrawEnd()
{
	// Выбросить задний буфер на экран
	swapChain->Present(0, 0);
}
void Kusanagi::View::Render::RenderDX11::Resize(unsigned long width, unsigned long height)
{

}

ID3D11Device1 * Kusanagi::View::Render::RenderDX11::GetDevice()
{
	return d3dDevice.Get();
}

ID3D11DeviceContext1 * Kusanagi::View::Render::RenderDX11::GetDeviceContext()
{
	return d3dImmediateContext.Get();
}


void Kusanagi::View::Render::RenderDX11::createDeviceAndContext()
{
	HRESULT hr = S_OK;
	UINT createDeviceFlags = 0;
#if defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] 
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	// Тут мы создаем список поддерживаемых версий DirectX
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _immediateContext;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		driverType = driverTypes[driverTypeIndex];
		//hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext);
		hr = D3D11CreateDevice(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, _device.GetAddressOf(), &featureLevel, _immediateContext.GetAddressOf());
		if (SUCCEEDED(hr)) // Если устройства созданы успешно, то выходим из цикла
			break;
	}
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("", 0x02000001, __FILE__, __LINE__);

	_device.As(&d3dDevice);
	_immediateContext.As(&d3dImmediateContext);
}
void Kusanagi::View::Render::RenderDX11::createSwapChain()
{
	HWND hWnd = reinterpret_cast<Kusanagi::Window::WinWindow*>(core->GetWindow())->GetHandler();
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;           // получаем ширину
	UINT height = rc.bottom - rc.top;   // и высоту окна

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
	//hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(dxgiFactory2.GetAddressOf()));
	if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)) && (featureLevel >= D3D_FEATURE_LEVEL_11_1))
	{
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC1));    // очищаем ее
		sd.BufferCount = 1;                               // у нас один задний буфер
		sd.Width = width;                     // ширина буфера
		sd.Height = height;                          // высота буфера
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // формат пикселя в буфере
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // назначение буфера - задний буфер
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC sfsd;
		ZeroMemory(&sfsd, sizeof(DXGI_SWAP_CHAIN_FULLSCREEN_DESC));    // очищаем ее
		sfsd.RefreshRate.Numerator = 75;
		sfsd.RefreshRate.Denominator = 1;
		sfsd.Scaling = DXGI_MODE_SCALING_STRETCHED;
		sfsd.Windowed = TRUE;
		
		// This system has DirectX 11.1 or later installed, so we can use this interface
		Microsoft::WRL::ComPtr<IDXGISwapChain1> _swapChain;
		dxgiFactory2->CreateSwapChainForHwnd(d3dDevice.Get(), hWnd, &sd, &sfsd, nullptr, _swapChain.GetAddressOf());
		dxgiFactory2->Release();
		_swapChain.As(&swapChain);
	}
	else
	{
		// Сейчас мы создадим устройства DirectX. Для начала заполним структуру,
		// которая описывает свойства переднего буфера и привязывает его к нашему окну.
		DXGI_SWAP_CHAIN_DESC sd;            // Структура, описывающая цепь связи (Swap Chain)
		ZeroMemory(&sd, sizeof(sd));    // очищаем ее
		sd.BufferCount = 1;                               // у нас один задний буфер
		sd.BufferDesc.Width = width;                     // ширина буфера
		sd.BufferDesc.Height = height;                          // высота буфера
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // формат пикселя в буфере
		sd.BufferDesc.RefreshRate.Numerator = 75;         // частота обновления экрана
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // назначение буфера - задний буфер
		sd.OutputWindow = hWnd;                               // привязываем к нашему окну
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;                               // не полноэкранный режим

		// This system only has DirectX 11.0 installed
		dxgiFactory->CreateSwapChain(d3dDevice.Get(), &sd, swapChain.GetAddressOf());
	}
}

void Kusanagi::View::Render::RenderDX11::createRenderTarget()
{
	// Теперь создаем задний буфер. Обратите внимание, в SDK
	// RenderTargetOutput - это передний буфер, а RenderTargetView - задний.
	ID3D11Texture2D* pBackBuffer = NULL;
	HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("Can't create render target", 0x02000004, __FILE__, __LINE__);

	// Я уже упоминал, что интерфейс g_pd3dDevice будет
	// использоваться для создания остальных объектов
	hr = d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, d3dRTV.GetAddressOf());
	pBackBuffer->Release();
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("Can't create render target", 0x02000005, __FILE__, __LINE__);
}

void Kusanagi::View::Render::RenderDX11::createDepthStencil()
{
	RECT rc;
	GetClientRect(reinterpret_cast<Kusanagi::Window::WinWindow*>(core->GetWindow())->GetHandler(), &rc);
	UINT width = rc.right - rc.left;           // получаем ширину
	UINT height = rc.bottom - rc.top;   // и высоту окна

	// Переходим к созданию буфера глубин
	// Создаем текстуру-описание буфера глубин
	D3D11_TEXTURE2D_DESC descDepth;     // Структура с параметрами
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;            // ширина и
	descDepth.Height = height;    // высота текстуры
	descDepth.MipLevels = 1;            // уровень интерполяции
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // формат (размер пикселя)
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;         // вид - буфер глубин
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// При помощи заполненной структуры-описания создаем объект текстуры
	HRESULT hr = d3dDevice->CreateTexture2D(&descDepth, NULL, &d3dDepthStencil);
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("Depth stencil texture creating failed", 0x02000006, __FILE__, __LINE__);

	// Теперь надо создать сам объект буфера глубин
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;            // Структура с параметрами
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;         // формат как в текстуре
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// При помощи заполненной структуры-описания и текстуры создаем объект буфера глубин
	hr = d3dDevice->CreateDepthStencilView(d3dDepthStencil.Get(), &descDSV, d3dDSV.GetAddressOf());
	if (FAILED(hr)) throw Kusanagi::Utils::Exception("Depth stencil view creating failed", 0x02000007, __FILE__, __LINE__);
}

void Kusanagi::View::Render::RenderDX11::addDeferredContexts()
{

}