#pragma once
#include	<d3d11.h>
#include	<wrl/client.h>
#include  "../../Application.h"
#include  "RenderTarget2D.h"

using Microsoft::WRL::ComPtr;
class DirectGraphicsTexture
{
private:
	DirectGraphicsTexture() {}
	
	ComPtr < ID3D11SamplerState> m_Sample;			// �V�F�[�_���\�[�X�r���[  �w�b�_�[�ɒu��
	RenderTarget2D			    m_Default;			// �f�t�H���g�̉摜�ƃp�[�e�B�N���̌v�Z���ʂ̍����摜��RTV
	RenderTarget2D			    m_DefaultInput;			// �f�t�H���g�̉摜�ƃp�[�e�B�N���̌v�Z���ʂ̍����摜��RTV

public:

	DirectGraphicsTexture(const DirectGraphicsTexture&) = delete;
	DirectGraphicsTexture& operator=(const DirectGraphicsTexture&) = delete;
	DirectGraphicsTexture(DirectGraphicsTexture&&) = delete;
	DirectGraphicsTexture& operator=(DirectGraphicsTexture&&) = delete;

	static DirectGraphicsTexture* GetInstance() {
		static DirectGraphicsTexture instance;
		return &instance;
	}

	// ��������
	bool Init();

	ID3D11ShaderResourceView* GetViewSRV() { return m_Default.GetSRV(); }
	ID3D11SamplerState* GetViewSample() { return m_Sample.Get(); }
	ID3D11RenderTargetView* GetRTV() { return m_Default.GetRTV(); }
	RenderTarget2D GetRenderTarget2D() { return m_Default; }
	
	ID3D11ShaderResourceView* GetInputViewSRV() { return m_DefaultInput.GetSRV(); }
	ID3D11RenderTargetView* GetInputRTV() { return m_DefaultInput.GetRTV(); }
	RenderTarget2D GetInputRenderTarget2D() { return m_DefaultInput; }
};

