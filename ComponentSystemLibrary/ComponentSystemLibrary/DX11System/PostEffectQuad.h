#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	"Vector.h"
#include    "../source/Compute/SomeSupportCompute.h"
using Microsoft::WRL::ComPtr;

// ��`�N���X
class PostEffectQuad {
public:
	// ���_�f�[�^
	struct Vertex {
		DirectX::XMFLOAT3 pos;		// ���W		
		DirectX::XMFLOAT4 color;	// �J���[
		DirectX::XMFLOAT2 tex;		// �e�N�X�`��
	};

	// ���ݎg�p�s��
	enum class RenderMode {
		DEFAULT,	//�ʏ�
		BILLBOARD,	//�r���{�[�h
	};

public:

	// ��`�̏�����
	bool Init(int width, int height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2 *uv, float z, std::string _vs, std::string _ps );

	//bool InitVS(const std::string _vs);
	bool InitPS(const std::string _vs);
	bool InitVS(const std::string _vs);
	void SetVS(std::string _vs) {
		m_vsstr = _vs; };
	void SetPS(std::string _ps) { m_psstr = _ps; };

	// �`��
	void NormalDraw(Vector3* _date);

	void NormalSetting();

	void ScreenDraw();

	void Exit();

	// �g��A�k��
	void SetScale(float sx, float sy, float sz);

	// �ʒu���Z�b�g
	void SetPosition(float x, float y, float z);

	// �`�惂�[�h�̃Z�b�g
	void SetRenderMode(RenderMode mode) { m_mode = mode; }

	// ���[���h�ϊ��s����Z�b�g����
	void SetMtx(DirectX::XMFLOAT4X4 mtx) {
		m_worldmtx = mtx;
	}

	// ���_�f�[�^�X�V
	void updateVertex_3D(float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv, float z = 0.0f);

	// ���_�o�b�t�@�X�V
	void updateVbuffer();

private:

	//// �z��f�[�^
	std::string m_vsstr = "";
	std::string m_psstr = "";

	DirectX::XMFLOAT4X4		m_worldmtx;				// ���[���h�ϊ��s��
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// ���_�o�b�t�@
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// �C���f�b�N�X�o�b�t�@
	PostEffectQuad::Vertex	m_vertex[4];			// ��`�S���_
	float					m_width;				// ��
	float					m_height;				// ����
	float					m_depth;				// �[�x�l
	RenderMode				m_mode;					// �`�惂�[�h
};

//-----------------------------
// create by Suehara
//-----------------------------