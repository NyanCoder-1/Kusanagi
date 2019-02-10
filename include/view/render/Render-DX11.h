#pragma once

#include "view/render/IRender.h"
#include <d3d11_1.h>
#include <dxgi.h>
#include <wrl.h>
#include <vector>

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

	public:
		RenderDX11(Kusanagi::Core *core);
		virtual ~RenderDX11() override;

		void Init() override;
		void Draw() override;
		void Resize(unsigned long width, unsigned long height) override;

	private:
		void createDeviceAndContext();
		void createSwapChain();

		void addDeferredContexts();
	};
}