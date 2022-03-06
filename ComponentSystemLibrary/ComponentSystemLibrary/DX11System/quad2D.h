#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>

// comptr
using Microsoft::WRL::ComPtr;

// ��`�N���X
class Quad2D {
public:
	// ���_�f�[�^
	struct Vertex {
		DirectX::XMFLOAT3 pos;		// ���W		
		DirectX::XMFLOAT4 color;	// �J���[
		DirectX::XMFLOAT2 tex;		// �e�N�X�`��
	};

	enum class UV_STATE {
		DEFAULT,	//�f�t�H���g
		TURN_X,		//X���Ŕ��]
		TURN_Y,		//Y���Ŕ��]
		TURN_XY		//XY���Ŕ��]
	};

	// ��`�̏�����
	bool Init(int width, int height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv);

	// �`��
	void Draw();

	// �}���`�e�N�X�`���̕`��
	void MultiDraw();

	void Exit();

	// �r���[�|�[�g�̃T�C�Y���Z�b�g
	bool SetViewPortSize(uint32_t width, uint32_t height);

	// �g��A�k��
	void SetScale(float sx, float sy, float sz);

	// �ʒu���Z�b�g
	void SetPosition(float x, float y, float z);

	// �ʒu�Ɖ�]
	void SetPosRot(Vector3 pos, Vector3 rot);

	// ���[���h�ϊ��s����Z�b�g����
	void SetMtx(DirectX::XMFLOAT4X4 mtx) {
		m_worldmtx = mtx;
	}

	// �e�N�X�`���t�@�C�����̃Z�b�g
	void SetTexture(std::string _texFileName) { m_texFileName = _texFileName; }

	// ���_�f�[�^�X�V
	void updateVertex(float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv);

	// ���_�o�b�t�@�X�V
	void updateVbuffer();

private:
	static const char* vsfilename[];
	static const char* psfilename[];

	DirectX::XMFLOAT4X4		m_worldmtx;				// ���[���h�ϊ��s��
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// ���_�o�b�t�@
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// �C���f�b�N�X�o�b�t�@
	Quad2D::Vertex			m_vertex[4];			// ��`�S���_
	float					m_width;				// ��
	float					m_height;				// ����
	std::string				m_texFileName;			// �e�N�X�`���t�@�C����
};