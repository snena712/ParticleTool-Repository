#include "DirectGraphicsTexture.h"

bool DirectGraphicsTexture::Init()
{
	HRESULT	hr = S_OK;

	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = Application::CLIENT_WIDTH;
	texDesc.Height = Application::CLIENT_HEIGHT;
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	//texDesc.SampleDesc.Quality = 0;
	texDesc.MiscFlags = 0;

	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	hr = m_Default.Init(texDesc);
	hr = m_DefaultInput.Init(texDesc);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Render2d Init  error", "Error", MB_OK);
		return false;
	}
	
	// サンプラステートの設定
	D3D11_SAMPLER_DESC smpDesc;
	memset(&smpDesc, 0, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	smpDesc.MinLOD = 0;
	smpDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// サンプラステート生成
	hr = _device->CreateSamplerState(&smpDesc, &m_Sample);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "Create RenderTarget error", "error", MB_OK);
		return false;
	}

	return true;
}