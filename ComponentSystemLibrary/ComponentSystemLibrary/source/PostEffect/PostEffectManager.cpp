#include "PostEffectManager.h"
#include <DirectXMath.h>

using namespace PostEffect;

void PostEffectManager::Init()
{
	// DirectGraphicsTextureの初期化
	bool sts = DirectGraphicsTexture::GetInstance()->Init();
	if (!sts)
	{
		MessageBox(Application::Instance()->GetHWnd(), "DirectGraphicsTexture", "error", MB_OK);
	}
	// ブルーム
	sts=PostEffect::BloomPostEffect::Instance()->Init();
	if (!sts)
	{
		MessageBox(Application::Instance()->GetHWnd(), "BloomPostEffect", "error", MB_OK);
	}

	// 陽炎
	sts=PostEffect::ShimmerPostEffect::Instance()->Init();
	if (!sts)
	{
		MessageBox(Application::Instance()->GetHWnd(), "ShimmerPostEffect", "error", MB_OK);
	}
}

/*------------------------
ポストエフェクトの描画処理
--------------------------*/
void PostEffectManager::ManagerDraw() {

	// ブルームの描画
	auto _outrtv = CDirectXGraphics::GetInstance()->GetRenderTargetView();
	auto _directrtv = DirectGraphicsTexture::GetInstance()->GetInputRTV();

	PostEffect::BloomPostEffect::Instance()->Draw();

	PostEffect::BloomPostEffect::Instance()->DrawFech(_directrtv, DirectGraphicsTexture::GetInstance()->GetRenderTarget2D().GetSRV(), PostEffect::BloomPostEffect::Instance()->GetSRVResult().GetSRV());
	PostEffect::BloomPostEffect::Instance()->DrawFech(_outrtv, DirectGraphicsTexture::GetInstance()->GetInputViewSRV(), nullptr);

	float _clearColor[4] = { 0.0f,0.0f,0.0f, 0.0f };	//red,green,blue,alpha（ＲＧＢの各値を0.0～1.0で指定）
	auto _rtv = PostEffect::BloomPostEffect::Instance()->GetSRVResult().GetRTV();
	CDirectXGraphics::GetInstance()->GetImmediateContext()->ClearRenderTargetView(_rtv, _clearColor);
	CDirectXGraphics::GetInstance()->TurnOnAlphaBlending();
	/*----------------------------------
	// ポストエフェクトのエフェクト描画
	------------------------------------*/
	PostEffect::ShimmerPostEffect::Instance()->SetSRVDefault(DirectGraphicsTexture::GetInstance()->GetInputRenderTarget2D(), _outrtv);
	PostEffect::ShimmerPostEffect::Instance()->Draw();
	PostEffect::ShimmerPostEffect::Instance()->SetSRVDefault(DirectGraphicsTexture::GetInstance()->GetRenderTarget2D(), _outrtv);
}

/*------------------------
ポストエフェクトの後処理
--------------------------*/
void PostEffectManager::AfterRenderClear() 
{
	auto _deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// RTVの場所の設定
	auto RTVdefault = DirectGraphicsTexture::GetInstance()->GetRTV();
	float _clearColor[4] = { 0.0f,0.0f,0.0f, 0.0f }; //red,green,blue,alpha（ＲＧＢの各値を0.0～1.0で指定）
	_deviceContex->ClearRenderTargetView(DirectGraphicsTexture::GetInstance()->GetRTV(), _clearColor);
	_deviceContex->OMSetRenderTargets(1, &RTVdefault, CDirectXGraphics::GetInstance()->GetDepthStencilView());
}