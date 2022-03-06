#include "RenderTarget2D.h"


// �����_�[�^�[�Q�b�g�̏�����
D3D11_RENDER_TARGET_VIEW_DESC RenderTarget2D::InitRenderTargetViewDESC( D3D11_TEXTURE2D_DESC _texDesc)
{
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	//ID3D11RenderTargetView�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = _texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	//ID3D11RenderTargetView�̍쐬
	auto hr = _device->CreateRenderTargetView(mpRenderTargetTex2.Get(), &rtvDesc, mpRenderTarget2RTV.GetAddressOf());
	if (FAILED(hr)) {
		MessageBox(nullptr, "Create RenderTarget error", "error", MB_OK);
	}
	return rtvDesc;
}

// SRV�̏�����
bool RenderTarget2D::InitShaderResourceiew( D3D11_RENDER_TARGET_VIEW_DESC _rtvdesc)
{
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// �V�F�[�_���\�[�X�r���[�̐ݒ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = _rtvdesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	// �V�F�[�_���\�[�X�r���[�̐���
	auto hr = _device->CreateShaderResourceView(mpRenderTargetTex2.Get(), &srvDesc, m_SRV.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(nullptr, "Create RenderTarget error", "error", MB_OK);
		return false;
	}
	return true;
}


// �e�N�X�`��2D�̍쐬
bool RenderTarget2D::Init( D3D11_TEXTURE2D_DESC _desc)
{
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// �摜�쐬
	auto hr = _device->CreateTexture2D(&_desc, nullptr, mpRenderTargetTex2.GetAddressOf());
	if (FAILED(hr)) {
		//("�����_�[�^�[�Q�b�g2�̍쐬�Ɏ��s");
		MessageBox(nullptr, "Create RenderTarget error", "error", MB_OK);
		return false;
	}

	auto rtvdesc =InitRenderTargetViewDESC( _desc);
	if (!InitShaderResourceiew(rtvdesc))
	{
		return false;
	}
	return true;;
}