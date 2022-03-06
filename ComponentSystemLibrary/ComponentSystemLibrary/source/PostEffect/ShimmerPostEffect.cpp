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

	// �T���v���[�̏�����
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

	// ���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[�A���_���C�A�E�g�擾		
	m_devContext->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_vsstr), nullptr, 0);
	m_devContext->GSSetShader(nullptr, nullptr, 0);
	m_devContext->HSSetShader(nullptr, nullptr, 0);
	m_devContext->DSSetShader(nullptr, nullptr, 0);
	m_devContext->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_psstr), nullptr, 0);

	// �u�����h�X�e�[�g�̐ݒ�
	m_devContext->OMSetBlendState(mpBlendState2.Get(), factor.data(), 0xffffffff);

	//========== �����_�[�^�[�Q�b�g�̏C���@����A���̂�����ʂɕ`��@�@���C���Ŋ���������===========//
	// RTV�̐ݒ�i�[�x�l�̕�����nullptr�ɂ��Ȃ��ƃo�O��j
	auto _rtv =m_outRTV.GetRTV();
	m_devContext->OMSetRenderTargets(1, &_rtv, nullptr);

	// SRV�̐ݒ�
	ID3D11ShaderResourceView* srv[] = {
		m_RTVDefault.GetSRV(),
		m_rt2d->GetSRV()
	};
	m_devContext->PSSetShaderResources(0, 2, srv);

	// �T���v���[�̐ݒ�
	ID3D11SamplerState* smp[] = {
		DirectGraphicsTexture::GetInstance()->GetViewSample()
	};
	m_devContext->PSSetSamplers(0, 1, smp);

	// transform�̐ݒ�
	height = 2;
	width = 2;

	//�@�|�X�g�G�t�F�N�g�̐ݒ�ƕ`��
	XMFLOAT2 uv2[4] = { {0.0f, 0.0f}, { 1.0f,0.0f }, { 0.0f,1.0f }, { 1.0f,1.0f } };

	//���_�f�[�^�A���_�o�b�t�@�X�V
	m_quad.updateVertex_3D(2, 2, color, uv2, 0.0f);
	m_quad.updateVbuffer();

	// ��ʓ���
	m_quad.SetPS(m_psstr);
	m_quad.SetVS(m_vsstr);

	// �|�X�g�G�t�F�N�g�̎��s
	m_quad.ScreenDraw();

	//=============== �ʏ�̃V�[���`��ʂɖ߂�=================//
	// �摜�����p��rtv�̃N���A
	auto _r2d=m_rt2d->GetRTV();
	m_devContext->OMSetRenderTargets(1, &_r2d, nullptr);
	m_devContext->ClearRenderTargetView(_r2d, _clearColor);

	std::vector<ID3D11RenderTargetView*> RTVdefault = { {
			DirectGraphicsTexture::GetInstance()->GetRTV()
		} };
	m_devContext->OMSetRenderTargets(static_cast<UINT>(RTVdefault.size()), RTVdefault.data(), CDirectXGraphics::GetInstance()->GetDepthStencilView());
	// ���u�����h�I��
	CDirectXGraphics::GetInstance()->TurnOnAlphaBlending();
}


// �u�����h�X�e�[�g�̍쐬�i�v�C���j
void ShimmerPostEffect::InitBlendDesc()
{
	auto m_device = CDirectXGraphics::GetInstance()->GetDXDevice();

	//�u�����h�X�e�[�g�̍쐬
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
	//ID3D11BlendState�̍쐬
	auto hr = m_device->CreateBlendState(&desc,mpBlendState2.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, "Create BlendState error", "error", MB_OK);
	}
}

// �e�N�X�`��2D�̍쐬
bool ShimmerPostEffect::InitTexture2DDesc()
{
	// �摜�ݒ�
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

	// ��ʗp��SRV�쐬
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

// �T���v���[�̏������idesc�̐؊����ł���悤�ɂ���j
bool ShimmerPostEffect::InitSampler()
{
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ������
	// �T���v���X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC smpDesc;
	memset(&smpDesc, 0, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	smpDesc.MinLOD = 0;
	smpDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �T���v���X�e�[�g����
	auto hr = _device->CreateSamplerState(&smpDesc, &mpSmp);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "Create RenderTarget error", "error", MB_OK);
		return false;
	}
	return true;

}

// �T���v���[�̏������idesc�̐؊����ł���悤�ɂ���j
void ShimmerPostEffect::SetSampler()
{
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ������
	mpSmp = nullptr;
	// �T���v���X�e�[�g�̐ݒ�
	D3D11_SAMPLER_DESC smpDesc;
	memset(&smpDesc, 0, sizeof(smpDesc));
	smpDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	smpDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	smpDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	smpDesc.MinLOD = 0;
	smpDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �T���v���X�e�[�g����
	auto hr = _device->CreateSamplerState(&smpDesc, &mpSmp);
	if (FAILED(hr))
	{
		MessageBox(nullptr, "Create RenderTarget error", "error", MB_OK);
	}
}