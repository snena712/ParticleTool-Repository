#include "PlayerJatParts.h"
#include "../../Component/SceneManager.h"
#include "../../DX11System/mystring.h"

using namespace Component;

PlayerJatParts::PlayerJatParts(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("PlayerJatParts");

	tra = owner->transform;

	// ���j�A�j���[�V����
	model = owner->AddComponent<Component::ModelRender>(
		"assets/F2_v2/f2a_ohane.fbx",		// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs.hlsl",						// ���_�V�F�[�_�[
		"shader/ps.hlsl",						// �s�N�Z���V�F�[�_�[
		"assets/F2_v2/f2a_texture/");			// �e�N�X�`���̑��݃t�H���_
}

PlayerJatParts::~PlayerJatParts()
{
}


void PlayerJatParts::Start()
{
	player = SceneManager::Instance()->GameObjectFindName("player");

}

void PlayerJatParts::Update()
{
	//RotUpdate();
}

// �p�x�̐ݒ�
void PlayerJatParts::SetRot(int _num)
{
	if (m_rot < m_maxrot && m_rot > -m_maxrot)
	{
		m_rot += _num;
		m_rotflg = true;
		owner->transform->RotateLocalXaxis(_num);
	}
}

// �p�x�̍X�V
void PlayerJatParts::RotUpdate()
{
	if (!m_rotflg)
	{
		if (m_rot > 0)
		{
			m_rot--;
			owner->transform->RotateLocalXaxis(-m_rotreturn);
		}
		if (m_rot < 0)
		{
			m_rot++;
			owner->transform->RotateLocalXaxis(m_rotreturn);
		}
	}
	else {
		m_rotflg = false;
	}
}


// new�ƃf���[�Ƃ������Ȃ�����ŏ����珉�������Ƃ�