#include "../Title/BackLoad3DObj.h"
#include "../../Component/SceneManager.h"
#include "../../DX11System/mystring.h"

using namespace Component;

BackLoad3DObj::BackLoad3DObj(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("BackLoader");
	
	// �v���C���[
	BackLoad(
		"assets/F2/f2a_game_v2.fbx",	// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/ps.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/F2/f2a_texture/");			// �e�N�X�`���̑��݃t�H���_

	// �G
	BackLoad(
		"assets/f1/f1.x",			// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/ps.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/f1/");				// �e�N�X�`���̑��݃t�H���_
}

BackLoad3DObj::~BackLoad3DObj()
{

}


void BackLoad3DObj::BackLoad(
	const char* _filename,
	const char* _vsfile,
	const char* _psfile,
	const char* _texfolder) {

	bool sts;

	sts = ModelResourceManager::GetInstance()->Load(_filename, _texfolder);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}

	// ���_�f�[�^�̒�`�i�A�j���[�V�����Ή��j
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	ID3D11Device* dev;
	dev = CDirectXGraphics::GetInstance()->GetDXDevice();

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		_vsfile,
		layout,
		numElements);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return;
	}

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetPixelShader(_psfile);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return;
	}

	return;

}