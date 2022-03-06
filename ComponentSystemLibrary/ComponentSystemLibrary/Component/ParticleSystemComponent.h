#pragma once
#include "ComponentBaseClass.h"
#include <math.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <vector>

using Microsoft::WRL::ComPtr;

namespace Component {
	
	class ParticleSystem :public ComponentBase {

	public:
		// パーティクル１つの頂点情報（シェーダに送る）
		struct ParticleVertex
		{
			DirectX::XMFLOAT3	position;	// 座標
			DirectX::XMFLOAT4	color;		// RGBAカラー
			DirectX::XMFLOAT2	tex;		//テクスチャ座標
		};

		// パーティクル１つの情報
		struct Particle
		{
			ParticleVertex		vertex;
			DirectX::XMFLOAT3	velocity;	//加速度
		};

		ParticleSystem(GameObject* Owner);
		~ParticleSystem();
		static void SystemInit();
		void Update();
		void Draw();
		void updateVbuffer();
		static void SystemUninit();

	private:
		static std::vector<Particle>	particleArray;				// パーティクルの管理配列
		static std::vector<ParticleSystem*> particleSystemArray;	// パーティクルシステムの管理配列
		static DirectX::XMFLOAT4X4		m_worldmtx;					// ワールド変換行列
		static ComPtr<ID3D11Buffer>	m_vertexbuffer;					// 頂点バッファ
		float					m_depth;							// 深度値
		unsigned int TexID;

		void Emittion();	//パーティクルの放出

	};
}
