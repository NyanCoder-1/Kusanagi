#pragma once

#include <d3d11.h>
#include <wrl.h>

namespace Kusanagi::View::Render
{
	class RenderDX11;

	class BufferDX11
	{
	public:
		static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateVertexBuffer(Kusanagi::View::Render::RenderDX11* render, void* data, const unsigned int size, bool dynamic);
		static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateIndexBuffer(Kusanagi::View::Render::RenderDX11* render, void* data, const unsigned int size, bool dynamic);
		static Microsoft::WRL::ComPtr<ID3D11Buffer> CreateConstantBuffer(Kusanagi::View::Render::RenderDX11* render, const unsigned int size, bool dynamic);
	};
}