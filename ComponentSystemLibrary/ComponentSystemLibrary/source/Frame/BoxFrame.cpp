#include "BoxFrame.h"


using namespace Component;

BoxFrame::BoxFrame(GameObject* _owner) : ComponentBase(_owner)
{
	// ���j�A�j���[�V����
	model = owner->AddComponent<Component::ModelRender>(
		"assets/Model/Frame/BoxFrame.fbx",		// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/basicvs.hlsl",						// ���_�V�F�[�_�[
		"shader/basicps.hlsl",						// �s�N�Z���V�F�[�_�[
		"");			// �e�N�X�`���̑��݃t�H���_
}

BoxFrame::~BoxFrame()
{}

void BoxFrame::DrawObject()
{
}

void BoxFrame::Update()
{

}

