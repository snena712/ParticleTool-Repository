#include "ShimmerPostEffect.h"
#include "BloomPostEffect.h"

using namespace PostEffect;

bool ShimmerPostEffect::Init()
{
	bool sts = InitTexture2DDesc();
	if (!sts)
	{
		MessageBox(nullptr, "Shimmer InitTexture2DDesc error", "error", MB_OK);
		return false;
	}

	XMFLOAT2 uv2[4] = { {0.0f, 0.0f}, { 1.0f,0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f } };
	sts = m_quad.Init(width, height, color, uv2, 0.0f, m_vsstr, m_psstr);
	if (!sts)
	{
		MessageBox(nullptr, "Shimmer Init quad error", "error", MB_OK);
		return false;
	}

	// サンプラーの初期化
	sts = InitSampler();
	if (!sts)
	{
		MessageBox(nullptr, "Shimmer InitSampler error", "error", MB_OK);
		return false;
	}

	return true;
}

void ShimmerPostEffect::Draw()
{
	auto m_devContext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// 頂点シェーダー、ピクセルシェーダー、頂点レイアウト取得		
	m_devContext->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_vsstr), nullptr, 0);
	m_devContext->GSSetShader(nullptr, nullptr, 0);
	m_devContext->HSSetShader(nullptr, nullptr, 0);
	m_devContext->DSSetShader(nullptr, nullptr, 0);
	m_devContext->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_psstr), nullptr, 0);

	// ブレンドステートの設定
	m_devContext->OMSetBlendState(mpBlendState2.Get(), factor.data(), 0xffffffff);

	//========== レンダーターゲットの修正　現状、合体させる面に描画　　メインで完了したい===========//
	// RTVの設定（深度値の部分をnullptrにしないとバグる）
	auto _rtv =m_outRTV.GetRTV();
	m_devContext->OMSetRenderTargets(1, &_rtv, nullptr);

	// SRVの設定
	ID3D11ShaderResourceView* srv[] = {
		m_RTVDefault.GetSRV(),
		m_rt2d->GetSRV()
	};
	m_devContext->PSSetShaderResources(0, 2, srv);

	// サンプラーの設定
	ID3D11SamplerState* smp[] = {
		DirectGraphicsTexture::GetInstance()->GetViewSample()
	};
	m_devContext->PSSetSamplers(0, 1, smp);

	// transformの設定
	height = 2;
	width = 2;

	//　ポストエフェクトの設定と描画
	XMFLOAT2 uv2[4] = { {0.0f, 0.0f}, { 1.0f,0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f } };

	//頂点データ、頂点バッファ更新
	m_quad.updateVertex_3D(2, 2, color, uv2, 0.0f);
	m_quad.updateVbuffer();

	// 画面統合
	m_quad.SetPS(m_psstr);
	m_quad.SetVS(m_vsstr);

	// ポストエフェクトの実行
	m_quad.ScreenDraw();

	//=============== 通常のシーン描画面に戻す=================//
	// 画像合成用のrtvのクリア
	auto _r2d=m_rt2d->GetRTV();
	m_devContext->OMSetRenderTargets(1, &_r2d, nullptr);
	m_devContext->ClearRenderTargetView(_r2d, _clearColor);

	std::vector<ID3D11RenderTargetView*> RTVdefault = { {
			DirectGraphicsTexture::GetInstance()->GetRTV()
		} };
	m_devContext->OMSetRenderTargets(static_cast<UINT>(RTVdefault.size()), RTVdefault.data(), CDirectXGraphics::GetInstance()->GetDepthStencilView());
	// αブレンドオン
	CDirectXGraphics::GetInstance()->TurnOnAlphaBlending();
}


// ブレンドステートの作成（要修正）
void ShimmerPostEffect::InitBlendDesc()
{
	auto m_device = CDirectXGraphics::GetInstance()->GetDXDevice();

	//ブレンドステートの作成
	D3D11_BLEND_DESC desc = {};
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = true;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	desc.RenderTarget[1].BlendEnable = false;
	desc.RenderTarget[1].SrcBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[1].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
	desc.RenderTarget[1].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[1].SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[1].DestBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[1].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[1].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//ID3D11BlendStateの作成
	auto hr = m_device->CreateBlendState(&desc,mpBlendState2.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, "Create BlendState error", "error", MB_OK);
	}
}

// テクスチャ2Dの作成
bool ShimmerPostEffect::InitTexture2DDesc()
{
	// 画像設定
	D3D11_TEXTURE2D_DESC texDesc;
	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = Application::Instance()->CLIENT_WIDTH;
	texDesc.Height = Application::Instance()->CLIENT_HEIGHT;
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	//texDesc.SampleDesc.Quality = 0;
	texDesc.MiscFlags = 0;

	// 画面用のSRV作成
	m_rt2d = new RenderTarget2D();
	bool sts = m_rt2d->Init(texDesc);
	if (!sts)
	{
		return false;
	}

	sts = m_RTVDefault.Init(texDesc);
	if (!sts)
	{
		return false;
	}
	return true;
}

// サンプラーの初期化（descの切換えできるようにする）
bool ShimmerPostEffect::InitSampler()
{
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 初期化
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
	auto hr = _device->CreateSamplerState(&smpDesc, &mpSmp);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "Create RenderTarget error", "error", MB_OK);
		return false;
	}
	return true;

}

// サンプラーの初期化（descの切換えできるようにする）
void ShimmerPostEffect::SetSampler()
{
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 初期化
	mpSmp = nullptr;
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
	auto hr = _device->CreateSamplerState(&smpDesc, &mpSmp);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "Create RenderTarget error", "error", MB_OK);
	}
}