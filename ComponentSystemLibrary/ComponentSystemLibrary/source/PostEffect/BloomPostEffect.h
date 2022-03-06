#pragma once
#include "../../DX11System/Vector.h"
#include "../../DX11System/dx11mathutil.h"
#include "../../source/Buffer/ConstBuffer.h"
#include "RenderTarget2D.h"
#include <DirectXTex.h>
#include "../../DX11System/CDirectxGraphics.h"
#include "../Compute/SomeSupportCompute.h"
#include "../../DX11System/PostEffectQuad.h"
#include "../Compute/ComputeManager.h"
#include "DirectGraphicsTexture.h"
#include "PostEffectBase.h"

struct GaussBlurParam
{
	int					SampleCount;
	int					Dummy[3];
	DirectX::XMFLOAT4   Offset[16];    // �I�t�Z�b�g�ł�.
};

namespace PostEffect
{

	class BloomPostEffect :public PostEffectBase
	{
	private:
		// shader
		std::string m_vsstr = "shader/ScreenVS.hlsl";
		std::string m_psBlurStr = "shader/GaussBlurPS.hlsl";
		std::string m_psCopyStr = "shader/CompositePS.hlsl";
		std::string m_psScreenCopy = "shader/CopyPS.hlsl";

		// �e��ݒ�
		ComPtr<ID3D11SamplerState>         m_pPointSampler = nullptr;      // �|�C���g�T���v���[
		ComPtr<ID3D11SamplerState>         m_pLinearSampler = nullptr;     // �|�C���g�T���v���[
		ComPtr<ID3D11BlendState>		   m_pOpequeBS = nullptr;
		ComPtr<ID3D11BlendState>		   mpBlendState2;				// �u�����h�̐ݒ�
		ConstBuffer*		        m_BlurBuffer;			// �u���[�̏��o�b�t�@
		RenderTarget2D			    m_PingPong[12];			// �u���[�|�������ʉ摜��RTV
		PostEffectQuad				m_quad;					// �摜�`��
	public:
		static BloomPostEffect* Instance() {
			static BloomPostEffect singleton;
			return &singleton;
		}

		bool Init();
		void Draw();
		BloomPostEffect();
		~BloomPostEffect();		
		
		void DrawFech(ID3D11RenderTargetView* _outrtv, ID3D11ShaderResourceView* _srv, ID3D11ShaderResourceView* _srv2);

	};
}


//-----------------------------
// create by Suehara
//-----------------------------