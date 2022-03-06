#include "PostEffectManager.h"
#include <DirectXMath.h>

using namespace PostEffect;

void PostEffectManager::Init()
{
	// DirectGraphicsTexture�̏�����
	bool sts = DirectGraphicsTexture::GetInstance()->Init();
	if (!sts)
	{
		MessageBox(Application::Instance()->GetHWnd(), "DirectGraphicsTexture", "error", MB_OK);
	}
	// �u���[��
	sts=PostEffect::BloomPostEffect::Instance()->Init();
	if (!sts)
	{
		MessageBox(Application::Instance()->GetHWnd(), "BloomPostEffect", "error", MB_OK);
	}

	// �z��
	sts=PostEffect::ShimmerPostEffect::Instance()->Init();
	if (!sts)
	{
		MessageBox(Application::Instance()->GetHWnd(), "ShimmerPostEffect", "error", MB_OK);
	}
}

/*------------------------
�|�X�g�G�t�F�N�g�̕`�揈��
--------------------------*/
void PostEffectManager::ManagerDraw() {

	// �u���[���̕`��
	auto _outrtv = CDirectXGraphics::GetInstance()->GetRenderTargetView();
	auto _directrtv = DirectGraphicsTexture::GetInstance()->GetInputRTV();

	PostEffect::BloomPostEffect::Instance()->Draw();

	PostEffect::BloomPostEffect::Instance()->DrawFech(_directrtv, DirectGraphicsTexture::GetInstance()->GetRenderTarget2D().GetSRV(), PostEffect::BloomPostEffect::Instance()->GetSRVResult().GetSRV());
	PostEffect::BloomPostEffect::Instance()->DrawFech(_outrtv, DirectGraphicsTexture::GetInstance()->GetInputViewSRV(), nullptr);

	float _clearColor[4] = { 0.0f,0.0f,0.0f, 0.0f };	//red,green,blue,alpha�i�q�f�a�̊e�l��0.0�`1.0�Ŏw��j
	auto _rtv = PostEffect::BloomPostEffect::Instance()->GetSRVResult().GetRTV();
	CDirectXGraphics::GetInstance()->GetImmediateContext()->ClearRenderTargetView(_rtv, _clearColor);
	CDirectXGraphics::GetInstance()->TurnOnAlphaBlending();
	/*----------------------------------
	// �|�X�g�G�t�F�N�g�̃G�t�F�N�g�`��
	------------------------------------*/
	PostEffect::ShimmerPostEffect::Instance()->SetSRVDefault(DirectGraphicsTexture::GetInstance()->GetInputRenderTarget2D(), _outrtv);
	PostEffect::ShimmerPostEffect::Instance()->Draw();
	PostEffect::ShimmerPostEffect::Instance()->SetSRVDefault(DirectGraphicsTexture::GetInstance()->GetRenderTarget2D(), _outrtv);
}

/*------------------------
�|�X�g�G�t�F�N�g�̌㏈��
--------------------------*/
void PostEffectManager::AfterRenderClear() 
{
	auto _deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// RTV�̏ꏊ�̐ݒ�
	auto RTVdefault = DirectGraphicsTexture::GetInstance()->GetRTV();
	float _clearColor[4] = { 0.0f,0.0f,0.0f, 0.0f }; //red,green,blue,alpha�i�q�f�a�̊e�l��0.0�`1.0�Ŏw��j
	_deviceContex->ClearRenderTargetView(DirectGraphicsTexture::GetInstance()->GetRTV(), _clearColor);
	_deviceContex->OMSetRenderTargets(1, &RTVdefault, CDirectXGraphics::GetInstance()->GetDepthStencilView());
}