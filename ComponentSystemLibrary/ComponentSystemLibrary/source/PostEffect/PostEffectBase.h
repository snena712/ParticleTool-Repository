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
	float width;										// ���̑傫��
	float height;										// �c�̑傫��
	DirectX::XMFLOAT4 color;							// �F
	float _clearColor[4] = { 0.0f,0.0f,0.0f, 0.0f };	//red,green,blue,alpha�i�q�f�a�̊e�l��0.0�`1.0�Ŏw��j
	std::vector<float> factor = { {1, 1, 1, 1} };		// �u�����h�p�ϐ�

	PostEffectQuad				m_quad;					// �摜�`��
	RenderTarget2D			    m_outRTV;				// �o�͐�
	RenderTarget2D				m_RTVDefault; 			// �m�C�Y�摜�ƍ�������RTV�̕ϐ�

public:
	virtual bool Init() { return true; };
	virtual void Draw() {};

	// �ʏ��RTV�摜�ݒ�
	void SetSRVDefault(ID3D11ShaderResourceView* _srv, ID3D11RenderTargetView* _rtv, ID3D11RenderTargetView* _outrtv) {
		m_RTVDefault.SetRTV(_rtv);
		m_RTVDefault.SetSRV(_srv);
		m_outRTV.SetRTV(_outrtv);
	}

	// �ʏ��RTV�摜�ݒ�
	void SetSRVDefault(RenderTarget2D _rt2d, ID3D11RenderTargetView* _outrtv) {
		m_RTVDefault = _rt2d;
		m_outRTV.SetRTV(_outrtv);
	}

	RenderTarget2D GetSRVDefault() {
		return m_RTVDefault;
	}

	// �p�[�e�B�N���ƒʏ�̍����摜��RTV�擾
	RenderTarget2D GetSRVResult() {
		return m_outRTV;
	}
};


//-----------------------------
// create by Suehara
//-----------------------------