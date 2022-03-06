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

namespace PostEffect
{
	class ShimmerPostEffect:public PostEffectBase
	{
	private:
		// shader
		std::string m_vsstr = "shader/ScreenVS.hlsl";
		std::string m_psstr = "shader/Shimmerps.hlsl";

		// 各種設定
		ComPtr < ID3D11SamplerState> mpSmp;					// サンプラーステート
		ComPtr<ID3D11BlendState> mpBlendState2;				// ブレンドの設定
		RenderTarget2D* m_rt2d;								// ノイズ用RTV

		void InitBlendDesc();
		bool InitTexture2DDesc();
		bool InitSampler();
		void SetSampler();
	public:
		static ShimmerPostEffect* Instance() {
			static ShimmerPostEffect singleton;
			return &singleton;
		}

		ShimmerPostEffect() {};
		~ShimmerPostEffect() {};

		bool Init();
		void Draw();

		void SetBlendState() {
			auto m_devContext = CDirectXGraphics::GetInstance()->GetImmediateContext();

			// ブレンドステートの設定
			m_devContext->OMSetBlendState(mpBlendState2.Get(), factor.data(), 0xffffffff);
		}

		RenderTarget2D* GetPostEffectRTV()
		{
			return m_rt2d;
		}
	};
}


//-----------------------------
// create by Suehara
//-----------------------------