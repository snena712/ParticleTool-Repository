#include "PlayerObj.h"
#include "../Component/QuadRenderComponent.h"
#include "../Component/ModelComponent.h"

PlayerObj::PlayerObj()
{
	this->SetName("player");

	transform->position = Vector3(0.0f, 0.0f, 0.0f);
	transform->scale = Vector3(1.0f, 1.0f, 1.0f);

	this->AddComponent<Component::ModelComponent>(
		"assets/fbx/Star.fbx",	// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/ps.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/fbx/tex/");			// �e�N�X�`���̑��݃t�H���_

	this->AddComponent<Component::QuadRender>("assets/dice/RED_SCISSOR.png");
	this->GetComponent<Component::QuadRender>()->SetHeight(30.0f);
	this->GetComponent<Component::QuadRender>()->SetWidth(30.0f);
}

PlayerObj::~PlayerObj()
{
	for (auto& comList : componentList) {

		if (comList != nullptr) {
			delete comList;
		}
	}
}

void  PlayerObj::Update()
{
	Start();

	//�R���|�[�l���g�X�V
	GameObject::Update();
}

void  PlayerObj::Draw()
{
	//�R���|�[�l���g�`��
	GameObject::Draw();
}

void PlayerObj::Start()
{

}
