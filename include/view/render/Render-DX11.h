#pragma once

#include "view/render/IRender.h"
#include <d3d11_1.h>
#include <dxgi.h>
#include <wrl.h>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace Kusanagi::View::Render
{
	class RenderDX11 : public Kusanagi::View::Render::IRender
	{
	private:
		Microsoft::WRL::ComPtr<ID3D11Device1> d3dDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext1> d3dImmediateContext;
		std::vector<Microsoft::WRL::ComPtr<ID3D11DeviceContext1>> d3dDeferredContexts;
		Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> d3dRTV;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> d3dDepthStencil;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> d3dDSV;
		D3D_DRIVER_TYPE driverType;
		D3D_FEATURE_LEVEL featureLevel;

	public:
		RenderDX11(Kusanagi::Core *core);
		virtual ~RenderDX11() override;

		void Init() override;
		void Draw() override;
		void Resize(unsigned long width, unsigned long height) override;

	private:
		void createDeviceAndContext();
		void createSwapChain();
		void createRenderTarget();
		void createDepthStencil();

		void addDeferredContexts();
	};
}