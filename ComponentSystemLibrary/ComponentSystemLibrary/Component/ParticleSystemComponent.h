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
		// �p�[�e�B�N���P�̒��_���i�V�F�[�_�ɑ���j
		struct ParticleVertex
		{
			DirectX::XMFLOAT3	position;	// ���W
			DirectX::XMFLOAT4	color;		// RGBA�J���[
			DirectX::XMFLOAT2	tex;		//�e�N�X�`�����W
		};

		// �p�[�e�B�N���P�̏��
		struct Particle
		{
			ParticleVertex		vertex;
			DirectX::XMFLOAT3	velocity;	//�����x
		};

		ParticleSystem(GameObject* Owner);
		~ParticleSystem();
		static void SystemInit();
		void Update();
		void Draw();
		void updateVbuffer();
		static void SystemUninit();

	private:
		static std::vector<Particle>	particleArray;				// �p�[�e�B�N���̊Ǘ��z��
		static std::vector<ParticleSystem*> particleSystemArray;	// �p�[�e�B�N���V�X�e���̊Ǘ��z��
		static DirectX::XMFLOAT4X4		m_worldmtx;					// ���[���h�ϊ��s��
		static ComPtr<ID3D11Buffer>	m_vertexbuffer;					// ���_�o�b�t�@
		float					m_depth;							// �[�x�l
		unsigned int TexID;

		void Emittion();	//�p�[�e�B�N���̕��o

	};
}
