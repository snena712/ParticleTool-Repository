#include "PlayerWing.h"


using namespace Component;

PlayerWing::PlayerWing(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("PlayerWing");

	

	// ���j�A�j���[�V����
	 owner->AddComponent<Component::ModelRender>(
		"assets/F2_v2/f2a_hane.fbx",	// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/ps.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/F2_v2/f2a_texture/");			// �e�N�X�`���̑��݃t�H���_
}

PlayerWing::~PlayerWing()
{

}



