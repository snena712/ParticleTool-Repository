#include "SphereModel.h"
#include "../../DX11System/DX11Settransform.h"

using namespace Component;

SphereModel::SphereModel(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("player");

	model = owner->AddComponent<Component::ModelRenderInstance>(
		"assets/Model/sphere.fbx",		// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs3dtest.hlsl",				// ���_�V�F�[�_�[
		"shader/ps3dtest.hlsl",				// �s�N�Z���V�F�[�_�[
		"");		// �e�N�X�`���̑��݃t�H���_
}

SphereModel::~SphereModel()
{}

//-----------------------------
// create by Suehara
//-----------------------------
