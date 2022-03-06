#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	"Vector.h"

// comptr
using Microsoft::WRL::ComPtr;

// ��`�N���X
class Quad {
public:
	// ���_�f�[�^
	struct Vertex {
		DirectX::XMFLOAT3 pos;		// ���W		
		DirectX::XMFLOAT4 color;	// �J���[
		DirectX::XMFLOAT2 tex;		// �e�N�X�`��
	};

	enum class RenderMode {
		DEFAULT,	//�ʏ�
		BILLBOARD,	//�r���{�[�h
	};

	enum class UV_STATE {
		DEFAULT,	//�f�t�H���g
		TURN_X,		//X���Ŕ��]
		TURN_Y,		//Y���Ŕ��]
		TURN_XY		//XY���Ŕ��]
	};

	// ��`�̏�����
	bool Init(int width, int height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv, float z = 0.0f);

	// �`��
	void Draw();

	// �}���`�e�N�X�`���̕`��
	void MultiDraw();

	void Exit();

	// �g��A�k��
	void SetScale(float sx, float sy, float sz);

	// �ʒu���Z�b�g
	void SetPosition(float x, float y, float z);

	// ��]
	void SetRotationX(float angle);
	void SetRotationY(float angle);
	void SetRotationZ(float angle);

	// �ʒu�Ɖ�]
	void SetPosRot(Vector3 pos, Vector3 rot);

	// �`�惂�[�h�̃Z�b�g
	void SetRenderMode(RenderMode mode) { m_mode = mode; }

	// ���[���h�ϊ��s����Z�b�g����
	void SetMtx(DirectX::XMFLOAT4X4 mtx) {
		m_worldmtx = mtx;
	}

	// ���_�f�[�^�X�V
	void updateVertex(float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv, float z = 0.0f);

	// ���_�o�b�t�@�X�V
	void updateVbuffer();

private:
	static const char* vsfilename[];
	static const char* psfilename[];

	DirectX::XMFLOAT4X4		m_worldmtx;				// ���[���h�ϊ��s��
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// ���_�o�b�t�@
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// �C���f�b�N�X�o�b�t�@
	Quad::Vertex			m_vertex[4];			// ��`�S���_
	float					m_width;				// ��
	float					m_height;				// ����
	float					m_depth;				// �[�x�l
	RenderMode				m_mode;			// �`�惂�[�h
};