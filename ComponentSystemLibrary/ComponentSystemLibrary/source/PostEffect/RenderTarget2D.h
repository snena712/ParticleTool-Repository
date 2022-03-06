#pragma once
#include	<d3d11.h>
#include "../../DX11System/CDirectxGraphics.h"

class RenderTarget2D
{
private:
	ComPtr<ID3D11Texture2D> mpRenderTargetTex2;			// レンダーターゲット専用のテクスチャ
	ComPtr < ID3D11ShaderResourceView> m_SRV;						// シェーダリソースビュー の二つ目におくもの
	ComPtr<ID3D11RenderTargetView> mpRenderTarget2RTV;	// 専用のレンダーターゲット


	bool InitShaderResourceiew(D3D11_RENDER_TARGET_VIEW_DESC _rtvdesc);
	D3D11_RENDER_TARGET_VIEW_DESC InitRenderTargetViewDESC(D3D11_TEXTURE2D_DESC _texDesc);
public:
	bool Init(D3D11_TEXTURE2D_DESC _desc);

	ID3D11Texture2D* GetTexture2D() { return mpRenderTargetTex2.Get(); }
	ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }
	ID3D11RenderTargetView* GetRTV() { return mpRenderTarget2RTV.Get(); }	// 専用のレンダーターゲット

	void SetSRV(ID3D11ShaderResourceView* _srv) { m_SRV=_srv; }
	void SetRTV(ID3D11RenderTargetView* _rtv) { mpRenderTarget2RTV = _rtv; }	// 専用のレンダーターゲット

	void ClearColorRenderTarget() {// レンダーターゲットの色初期化
		auto devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		float _clear[4] = { 0,0,0,0 };
		devcontext->ClearRenderTargetView(mpRenderTarget2RTV.Get(), _clear);
	}
};


//-----------------------------
// create by Suehara
//-----------------------------
