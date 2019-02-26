#include "view/render/Buffer-DX11.h"
#include "view/render/Render-DX11.h"

Microsoft::WRL::ComPtr<ID3D11Buffer> Kusanagi::View::Render::BufferDX11::CreateVertexBuffer(Kusanagi::View::Render::RenderDX11* render, void * data, const unsigned int size, bool dynamic)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> ret;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = data;

	if (FAILED(render->GetDevice()->CreateBuffer(&bd, &InitData, ret.GetAddressOf()))) return nullptr;
	return ret;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Kusanagi::View::Render::BufferDX11::CreateIndexBuffer(Kusanagi::View::Render::RenderDX11* render, void * data, const unsigned int size, bool dynamic)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> ret;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = data;

	if (FAILED(render->GetDevice()->CreateBuffer(&bd, &InitData, ret.GetAddressOf()))) return nullptr;
	return ret;
}

Microsoft::WRL::ComPtr<ID3D11Buffer> Kusanagi::View::Render::BufferDX11::CreateConstantBuffer(Kusanagi::View::Render::RenderDX11* render, const unsigned int size, bool dynamic)
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> ret;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;

	if (FAILED(render->GetDevice()->CreateBuffer(&bd, nullptr, ret.GetAddressOf()))) return nullptr;
	return ret;
}
