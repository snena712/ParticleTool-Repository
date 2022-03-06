#pragma once
#include "../../source/Buffer/ConstBuffer.h"
#include "RenderTarget2D.h"
#include <DirectXTex.h>
#include "../../DX11System/CDirectxGraphics.h"
#include "../../DX11System/PostEffectQuad.h"
#include "DirectGraphicsTexture.h"

class PostEffectBase
{
protected:
	float width;										// 横の大きさ
	float height;										// 縦の大きさ
	DirectX::XMFLOAT4 color;							// 色
	float _clearColor[4] = { 0.0f,0.0f,0.0f, 0.0f };	//red,green,blue,alpha（ＲＧＢの各値を0.0～1.0で指定）
	std::vector<float> factor = { {1, 1, 1, 1} };		// ブレンド用変数

	PostEffectQuad				m_quad;					// 画像描画
	RenderTarget2D			    m_outRTV;				// 出力先
	RenderTarget2D				m_RTVDefault; 			// ノイズ画像と合成するRTVの変数

public:
	virtual bool Init() { return true; };
	virtual void Draw() {};

	// 通常のRTV画像設定
	void SetSRVDefault(ID3D11ShaderResourceView* _srv, ID3D11RenderTargetView* _rtv, ID3D11RenderTargetView* _outrtv) {
		m_RTVDefault.SetRTV(_rtv);
		m_RTVDefault.SetSRV(_srv);
		m_outRTV.SetRTV(_outrtv);
	}

	// 通常のRTV画像設定
	void SetSRVDefault(RenderTarget2D _rt2d, ID3D11RenderTargetView* _outrtv) {
		m_RTVDefault = _rt2d;
		m_outRTV.SetRTV(_outrtv);
	}

	RenderTarget2D GetSRVDefault() {
		return m_RTVDefault;
	}

	// パーティクルと通常の合成画像とRTV取得
	RenderTarget2D GetSRVResult() {
		return m_outRTV;
	}
};


//-----------------------------
// create by Suehara
//-----------------------------