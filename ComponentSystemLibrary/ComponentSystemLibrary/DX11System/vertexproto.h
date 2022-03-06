#pragma once
#include	<directxmath.h>

// ���_
struct Vertex {
	DirectX::XMFLOAT3 m_Pos;		// �ʒu
	DirectX::XMFLOAT3 m_Normal;		// �@��
	DirectX::XMFLOAT2 m_Tex;		// UV���W
};

// �}�e���A��
struct Material {
	float m_Alpha;
	DirectX::XMFLOAT3 m_Diffuse;
	DirectX::XMFLOAT3 m_Specular;
	float	m_SpecularPower;
	DirectX::XMFLOAT3 m_Ambient;
	DirectX::XMFLOAT3 m_Emissive;

	Material()
		: m_Alpha(0),
		m_Diffuse(DirectX::XMFLOAT3(0, 0, 0)),
		m_Specular(DirectX::XMFLOAT3(0, 0, 0)),
		m_SpecularPower(0),
		m_Ambient(DirectX::XMFLOAT3(0, 0, 0)),
		m_Emissive(DirectX::XMFLOAT3(0, 0, 0))
	{
	}
};

// �萔�o�b�t�@��`�i�}�e���A���j
struct ConstantBufferMaterial {
	DirectX::XMFLOAT4	AmbientMaterial;		// �����̔��˗�
	DirectX::XMFLOAT4	DiffuseMaterial;		// �f�B�t���[�Y���̔��˗�
	DirectX::XMFLOAT4	SpecularMaterial;		// �X�y�L�������̔��˗�
};