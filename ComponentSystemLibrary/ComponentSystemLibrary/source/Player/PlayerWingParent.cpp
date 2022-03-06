#include "PlayerWingParent.h"
#include "../../Component/SceneManager.h"

using namespace Component;

PlayerWingParent::PlayerWingParent(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("PlayerWingParent");

	// �֐߂̂��������邽�߂Ƀ��f�����q�Ƃ���
	GameObject* wing = new GameObject();
	wing->transform->SetParent(owner->transform);
	wing->AddComponent<Component::PlayerWing>();
	wing->transform->position.z -= 0.35f;
	SceneManager::Instance()->AddObject(wing, false);
}

PlayerWingParent::~PlayerWingParent()
{

}

void PlayerWingParent::Update()
{
	RotUpdate();
}

// �p�x�̐ݒ�
void PlayerWingParent::SetRot(int _num)
{
	if (m_rot < m_maxrot && m_rot > -m_maxrot)
	{
		m_rot += _num;
		m_rotflg = true;
		owner->transform->RotateXaxis(_num);
	}
}

// �p�x�̍X�V
void PlayerWingParent::RotUpdate()
{
	if (!m_rotflg)
	{
		if (m_rot > 0)
		{
			m_rot -= 3;
			owner->transform->RotateXaxis(-m_rotreturn);
		}
		if (m_rot < 0)
		{
			m_rot += 3;
			owner->transform->RotateXaxis(m_rotreturn);
		}
	}
	else {
		m_rotflg = false;
	}
}


