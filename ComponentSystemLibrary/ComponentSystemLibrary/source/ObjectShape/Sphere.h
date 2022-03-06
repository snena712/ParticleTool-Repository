#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<math.h>
#include	<vector>
#include    "../../DX11System\DX11util.h"
#include    "../../DX11System\shaderhashmap.h"
#include	"../../DX11System/dx11mathutil.h"
#include    "../../Component/GameObject.h"
#include	"../../DX11System/DX11Settransform.h"
#include "../../ComputeDateHeader.h"

using Microsoft::WRL::ComPtr;

class Sphere {
private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// ���_�o�b�t�@
	ComPtr<ID3D11Buffer> m_pIndexBuffer;		// �C���f�b�N�X�o�b�t�@
	unsigned int		m_divX;					// ���������̕�����
	unsigned int		m_divY;					// ���������̕�����
	float				m_radius;				// ���a
	XMFLOAT4			m_color;				// ���_�J���[
	std::string	m_PixelShaderName = "shader/pssphere.hlsl";		// PS�V�F�[�_�[��
	std::string	m_VertexShaderName = "shader/vssphere.hlsl";	// VS�V�F�[�_�[��

	// ���_�f�[�^
	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
		DirectX::XMFLOAT4	Color;
	};

	// �C���f�b�N�X�f�[�^
	struct Face {
		unsigned int idx[3];
	};

	std::vector<Vertex>		m_vertex;			//�@���_�f�[�^
	std::vector<Face>		m_face;				//	�C���f�b�N�X�f�[�^


	// �C���f�b�N�X�f�[�^���쐬����
	void CreateIndex();
	// ���_�f�[�^���쐬����
	void CreateVertex();
	// ���K��
	void Normalize(XMFLOAT3 vector, XMFLOAT3& Normal);

public:
	// �`��
	void Draw(unsigned int _num, ParticleBase* _particleBase);

	// ������
	bool Init(float r,						// ���a
		int division_horizontal,			// ���������̕�����
		int division_vertical,				// ���������̕�����
		std::string _VS,					// ���_�V�F�[�_�[
		std::string _PS);					// �s�N�Z���V�F�[�_�[

	// ���_�o�b�t�@�̍X�V
	void ColorUpdate(XMFLOAT4 _color);

	void SetTransform(XMFLOAT3 _pos, float _Radius, float _size);
};