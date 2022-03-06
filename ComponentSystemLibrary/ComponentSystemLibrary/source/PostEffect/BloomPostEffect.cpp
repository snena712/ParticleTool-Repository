#include "BloomPostEffect.h"

using namespace PostEffect;

//-------------------------------------------------------------------------------------------------
//      ガウスの重みを計算します.
//-------------------------------------------------------------------------------------------------
inline float GaussianDistribution(const Vector3 pos, float rho)
{
	return exp(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * rho * rho));
}

//-------------------------------------------------------------------------------------------------
//      ブラーパラメータを計算します.
//-------------------------------------------------------------------------------------------------
inline GaussBlurParam CalcBlurParam(int width, int height, Vector3 dir, float deviation, float multiply)
{
	GaussBlurParam result;
	result.SampleCount = 15;
	auto tu = 1.0f / float(width);
	auto tv = 1.0f / float(height);

	result.Offset[0].z = GaussianDistribution(Vector3(0.0f, 0.0f,0.0f), deviation) * multiply;
	auto total_weight = result.Offset[0].z;

	result.Offset[0].x = 0.0f;
	result.Offset[0].y = 0.0f;

	for (auto i = 1; i < 8; ++i)
	{
		result.Offset[i].x = dir.x * i * tu;
		result.Offset[i].y = dir.y * i * tv;
		result.Offset[i].z = GaussianDistribution(dir * float(i), deviation) * multiply;
		total_weight += result.Offset[i].z * 2.0f;
	}

	for (auto i = 0; i < 8; ++i)
	{
		result.Offset[i].z /= total_weight;
	}
	for (auto i = 8; i < 15; ++i)
	{
		result.Offset[i].x = -result.Offset[i - 7].x;
		result.Offset[i].y = -result.Offset[i - 7].y;
		result.Offset[i].z = result.Offset[i - 7].z;
	}

	return result;
}



/////////////////////////////////////////////////////////////////////////////////////////
// SampleApplication class
/////////////////////////////////////////////////////////////////////////////////////////
BloomPostEffect::BloomPostEffect()
{ /* DO_NOTHING */
}

//---------------------------------------------------------------------------------------
//      デストラクタです.
//---------------------------------------------------------------------------------------
BloomPostEffect::~BloomPostEffect()
{ /* DO_NOTHING */
}

//---------------------------------------------------------------------------------------
//      初期化時の処理です.
//---------------------------------------------------------------------------------------
bool BloomPostEffect::Init()
{
	HRESULT hr = S_OK;
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	{
		// ピクセルシェーダを生成.
		// コンポジットPS　　合体		
		if (!Helper::SetPS(m_psCopyStr))
		{
			return false;
		}
	}

	{
		// ガウスPS　ぼかし処理
		if (!Helper::SetPS(m_psBlurStr))
		{
			return false;
		}
	}

	{
		if (!Helper::SetPS(m_psScreenCopy))
		{
			return false;
		}
	}

	{
		// 画像？
		//m_quad = new PostEffectQuad();
		XMFLOAT2 uv2[4] = { {0.0f, 0.0f}, { 1.0f,0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f } };
		m_quad.Init(2, 2, XMFLOAT4(1,1,1,1), uv2, 0.0f, m_vsstr, m_psBlurStr);
	}

	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.MipLODBias = 0;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.BorderColor[0] = 0.0f;
		desc.BorderColor[1] = 0.0f;
		desc.BorderColor[2] = 0.0f;
		desc.BorderColor[3] = 0.0f;
		desc.MinLOD = -3.402823466e+38F; // -FLT_MAX
		desc.MaxLOD = 3.402823466e+38F; // FLT_MAX

		hr = _device->CreateSamplerState(&desc, &m_pPointSampler);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "ID3D11Device::CreateSamplerState() Failed", "error", MB_OK);
			return false;
		}
	}

	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.MipLODBias = 0;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.BorderColor[0] = 0.0f;
		desc.BorderColor[1] = 0.0f;
		desc.BorderColor[2] = 0.0f;
		desc.BorderColor[3] = 0.0f;
		desc.MinLOD = -3.402823466e+38F; // -FLT_MAX
		desc.MaxLOD = 3.402823466e+38F; // FLT_MAX

		hr = _device->CreateSamplerState(&desc, &m_pLinearSampler);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "ID3D11Device::CreateSamplerState() Failed", "error", MB_OK);
			return false;
		}
	}

	{
		D3D11_BLEND_DESC desc = {};
		desc.AlphaToCoverageEnable = FALSE;
		desc.IndependentBlendEnable = FALSE;
		desc.RenderTarget[0].BlendEnable = FALSE;
		desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = _device->CreateBlendState(&desc, &m_pOpequeBS);
		if (FAILED(hr))
		{
			MessageBox(nullptr, " ID3D11Device::CreateBlendState() Failed.", "error", MB_OK);
			return false;
		}
	}

	{

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

		//ID3D11BlendStateの作成
		auto hr = _device->CreateBlendState(&desc, mpBlendState2.GetAddressOf());
		if (FAILED(hr)) {
			MessageBox(nullptr, "Create BlendState error", "error", MB_OK);
			return false;
		}
	}

	{
		m_BlurBuffer = new ConstBuffer();
		m_BlurBuffer->CreateConStructuredBuffer<GaussBlurParam>(ConstBuffer::Type::Multi, 1);
	}

	{
		// サイズの確認
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

		// 出力先
		m_RTVDefault.Init(texDesc);
		m_outRTV.Init(texDesc);

		texDesc.Width = Application::Instance()->CLIENT_WIDTH / 4;
		texDesc.Height = Application::Instance()->CLIENT_HEIGHT / 4;

		for (auto i = 0; i < 12; i += 2)
		{
			if (!m_PingPong[i + 0].Init(texDesc))
			{
				return false;;
			}

			if (!m_PingPong[i + 1].Init(texDesc))
			{
				return false;;
			}

			texDesc.Width >>= 1;
			texDesc.Height >>= 1;
		}
	}

	return true;
}



//---------------------------------------------------------------------------------------
//      描画時の処理です.
//---------------------------------------------------------------------------------------
void BloomPostEffect::Draw()
{
	// 各種
	auto _ParicleRTV = m_RTVDefault;									// パーティクル描画用のRenderTex
	auto pSrcSRV = _ParicleRTV.GetSRV();								// パーティクル描画用のRenderTexのSRV
	auto pDstRTV = m_PingPong[0].GetRTV();								// パーティクル描画用のRenderTexのRTV
	auto _rasterize = CDirectXGraphics::GetInstance()->GetRasterizerState();	// デフォルトのラスタライザ
	auto _DSState = CDirectXGraphics::GetInstance()->GetDepthStencilState();	// デフォルトの深度
	
	// ブラー用パラメーター
	auto w = Application::CLIENT_WIDTH / 4;
	auto h = Application::CLIENT_HEIGHT / 4;
	auto deviation = 2.5f;
	int _StencilRef = 0;

	auto _deviceContext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	float _clearColor[4] = { 0.0f,0.0f,0.0f, 0.0f }; //red,green,blue,alpha（ＲＧＢの各値を0.0～1.0で指定）
	
	XMFLOAT2 uv2[4] = { {0.0f, 0.0f}, { 1.0f,0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f } };	// 画像のUV値

	// 最初のパス.
	{
		// クリア処理.
		_deviceContext->ClearRenderTargetView(pDstRTV, _clearColor);

		// 出力マネージャに設定.
		_deviceContext->OMSetRenderTargets(1, &pDstRTV, nullptr);

		float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		UINT sampleMask = D3D11_DEFAULT_SAMPLE_MASK;
		// ステートを設定.
		_deviceContext->RSSetState(_rasterize);
		
		//------一時削除 
		_deviceContext->OMSetBlendState(m_pOpequeBS.Get(), blendFactor, sampleMask);
		_deviceContext->OMSetDepthStencilState(_DSState, _StencilRef);

		// シェーダの設定.（画面全体＋ブラー）
		m_quad.SetVS(m_vsstr);
		_deviceContext->GSSetShader(nullptr, nullptr, 0);
		_deviceContext->HSSetShader(nullptr, nullptr, 0);
		_deviceContext->DSSetShader(nullptr, nullptr, 0);
		m_quad.SetPS(m_psBlurStr);

		// シェーダリソースビューを設定.
		auto pSRV = pSrcSRV;
		_deviceContext->PSSetShaderResources(0, 1, &pSrcSRV);
		_deviceContext->PSSetSamplers(0, 1, &m_pPointSampler);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = float(w);
		viewport.Height = float(h);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		_deviceContext->RSSetViewports(1, &viewport);

		GaussBlurParam src = CalcBlurParam(w, h, Vector3(1.0f, 0.0f, 0.0f), deviation, 1.0f);
		m_BlurBuffer->Update(&src);
		m_BlurBuffer->SetPSBuffer(3);

		// 描画.
		m_quad.ScreenDraw();

		pDstRTV = m_PingPong[1].GetRTV();

		// クリア処理.
		_deviceContext->ClearRenderTargetView(pDstRTV, _clearColor);

		// 出力マネージャに設定.
		_deviceContext->OMSetRenderTargets(1, &pDstRTV, nullptr);

		// シェーダリソースビューを設定.
		pSRV = m_PingPong[0].GetSRV();
		_deviceContext->PSSetShaderResources(0, 1, &pSRV);
		_deviceContext->PSSetSamplers(0, 1, &m_pPointSampler);

		// バッファの更新
		src = CalcBlurParam(w, h, Vector3(0.0f, 1.0f, 0.0f), deviation, 1.0f);
		m_BlurBuffer->Update(&src);
		m_BlurBuffer->SetPSBuffer(3);

		// ステートを設定.
		_deviceContext->RSSetState(_rasterize);
		_deviceContext->OMSetBlendState(m_pOpequeBS.Get(), blendFactor, sampleMask);
		_deviceContext->OMSetDepthStencilState(_DSState, _StencilRef);

		// 描画.
		m_quad.ScreenDraw();

		// シェーダリソースをクリア.
		ID3D11ShaderResourceView* nullTarget[1] = { nullptr };
		_deviceContext->PSSetShaderResources(0, 1, nullTarget);

		w >>= 1;
		h >>= 1;

		pDstRTV = m_PingPong[2].GetRTV();
		pSrcSRV = m_PingPong[1].GetSRV();
	}

	// ブラーパス
	float m = 2.0f;
	for (auto i = 1; i < 5; ++i)
	{
		// クリア処理.
		_deviceContext->ClearRenderTargetView(pDstRTV, _clearColor);

		// 出力マネージャに設定.
		_deviceContext->OMSetRenderTargets(1, &pDstRTV, nullptr);

		float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		UINT sampleMask = D3D11_DEFAULT_SAMPLE_MASK;
		// ステートを設定.
		_deviceContext->RSSetState(_rasterize);
		_deviceContext->OMSetBlendState(m_pOpequeBS.Get(), blendFactor, sampleMask);
		_deviceContext->OMSetDepthStencilState(_DSState, _StencilRef);

		// シェーダの設定.
		m_quad.SetVS(m_vsstr);
		_deviceContext->GSSetShader(nullptr, nullptr, 0);
		_deviceContext->HSSetShader(nullptr, nullptr, 0);
		_deviceContext->DSSetShader(nullptr, nullptr, 0);
		m_quad.SetPS(m_psBlurStr);

		// シェーダリソースビューを設定.
		auto pSRV = pSrcSRV;
		_deviceContext->PSSetShaderResources(0, 1, &pSrcSRV);
		_deviceContext->PSSetSamplers(0, 1, &m_pPointSampler);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = float(w);
		viewport.Height = float(h);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		_deviceContext->RSSetViewports(1, &viewport);

		
		GaussBlurParam src = CalcBlurParam(w, h, Vector3(1.0f, 0.0f, 0.0f), deviation, m);
		m_BlurBuffer->Update(&src);
		m_BlurBuffer->SetPSBuffer(3);

		// 描画.
		m_quad.ScreenDraw();


		pDstRTV = m_PingPong[i * 2 + 1].GetRTV();

		// クリア処理.
		_deviceContext->ClearRenderTargetView(pDstRTV, _clearColor);

		ID3D11DepthStencilView* pDSV = CDirectXGraphics::GetInstance()->GetDepthStencilView();
		// 出力マネージャに設定.
		_deviceContext->OMSetRenderTargets(1, &pDstRTV, nullptr);

		// シェーダリソースビューを設定.
		pSRV = m_PingPong[i * 2 + 0].GetSRV();
		_deviceContext->PSSetShaderResources(0, 1, &pSRV);
		_deviceContext->PSSetSamplers(0, 1, &m_pPointSampler);

		src = CalcBlurParam(w, h, Vector3(0.0f, 1.0f, 0.0f), deviation, m);
		m_BlurBuffer->Update(&src);
		m_BlurBuffer->SetPSBuffer(3);

		// ステートを設定.
		_deviceContext->RSSetState(_rasterize);
		_deviceContext->OMSetBlendState(m_pOpequeBS.Get(), blendFactor, sampleMask);
		_deviceContext->OMSetDepthStencilState(_DSState, _StencilRef);

		// 描画.
		m_quad.ScreenDraw();

		// シェーダリソースをクリア.
		ID3D11ShaderResourceView* nullTarget[1] = { nullptr };
		_deviceContext->PSSetShaderResources(0, 1, nullTarget);

		w >>= 1;
		h >>= 1;
		m *= 2.0f;

		pDstRTV = m_PingPong[i * 2 + 2].GetRTV();
		pSrcSRV = m_PingPong[i * 2 + 1].GetSRV();
	}

	{
		// レンダーターゲットビュー・深度ステンシルビューを取得.
		pDstRTV = m_outRTV.GetRTV();

		ID3D11DepthStencilView* pDSV = CDirectXGraphics::GetInstance()->GetDepthStencilView();	
		_deviceContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// 出力マネージャに設定
		_deviceContext->OMSetRenderTargets(1, &pDstRTV, pDSV);
		// クリア処理.
		_deviceContext->ClearRenderTargetView(pDstRTV, _clearColor);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = float(Application::CLIENT_WIDTH);
		viewport.Height = float(Application::CLIENT_HEIGHT);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		_deviceContext->RSSetViewports(1, &viewport);

		{
			// シェーダの設定.
			m_quad.SetVS(m_vsstr);
			_deviceContext->GSSetShader(nullptr, nullptr, 0);
			_deviceContext->HSSetShader(nullptr, nullptr, 0);
			_deviceContext->DSSetShader(nullptr, nullptr, 0);
			m_quad.SetPS(m_psCopyStr);
		}

		// シェーダリソースビューを設定.
		ID3D11ShaderResourceView* pSRV[] = {
			_ParicleRTV.GetSRV(),	// 基本画像
			m_PingPong[1].GetSRV(),
			m_PingPong[3].GetSRV(),
			m_PingPong[5].GetSRV(),
			m_PingPong[7].GetSRV(),
			m_PingPong[9].GetSRV(),
			m_PingPong[11].GetSRV()
		};

		_deviceContext->PSSetShaderResources(0, 7, pSRV);
		_deviceContext->PSSetSamplers(0, 1, &m_pLinearSampler);

		// 描画.
		m_quad.ScreenDraw();

		// 基本SRVに結果SRV移す
		//m_RTVDefault.SetSRV(m_outRTV.GetSRV());
		pDstRTV = m_RTVDefault.GetRTV();
		_deviceContext->OMSetRenderTargets(1, &pDstRTV, pDSV);
		// クリア処理.
		_deviceContext->ClearRenderTargetView(pDstRTV, _clearColor);

		pDstRTV = CDirectXGraphics::GetInstance()->GetRenderTargetView();
		_deviceContext->OMSetRenderTargets(1, &pDstRTV, pDSV);

		// シェーダリソースをクリア.
		ID3D11ShaderResourceView* nullTarget[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		_deviceContext->PSSetShaderResources(0, 7, nullTarget);
	}
}


void BloomPostEffect::DrawFech(ID3D11RenderTargetView* _outrtv, ID3D11ShaderResourceView* _srv, ID3D11ShaderResourceView* _srv2)
{
	auto _deviceContext = CDirectXGraphics::GetInstance()->GetImmediateContext();
	auto _rasterize = CDirectXGraphics::GetInstance()->GetRasterizerState();	// デフォルトのラスタライザ

	ID3D11ShaderResourceView* mSRV[] = {
		_srv,	// 基本画像
		_srv2,		// デフォルトの画像
		nullptr, nullptr, nullptr, nullptr, nullptr
	};

	m_quad.SetVS(m_vsstr);
	_deviceContext->GSSetShader(nullptr, nullptr, 0);
	_deviceContext->HSSetShader(nullptr, nullptr, 0);
	_deviceContext->DSSetShader(nullptr, nullptr, 0);
	m_quad.SetPS(m_psScreenCopy);

	// 出力マネージャに設定　クリア
	auto pDstRTV = _outrtv;
	_deviceContext->OMSetRenderTargets(1, &pDstRTV, nullptr);

	_deviceContext->PSSetShaderResources(0, 7, mSRV);

	_deviceContext->RSSetState(_rasterize);

	auto _samp = DirectGraphicsTexture::GetInstance()->GetViewSample();
	_deviceContext->PSSetSamplers(0, 1, &_samp);

	//_deviceContext->OMSetBlendState(mpBlendState2.Get(), factor.data(), 0xffffffff);

	// 描画.
	m_quad.ScreenDraw();

	// シェーダリソースをクリア.
	ID3D11ShaderResourceView* nullTarget[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	_deviceContext->PSSetShaderResources(0, 7, nullTarget);
}

//-----------------------------
// create by Suehara
//-----------------------------