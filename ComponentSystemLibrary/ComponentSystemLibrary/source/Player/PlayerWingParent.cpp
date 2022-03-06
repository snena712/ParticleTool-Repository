#include "PlayerWingParent.h"
#include "../../Component/SceneManager.h"

using namespace Component;

PlayerWingParent::PlayerWingParent(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("PlayerWingParent");

	// 関節のいち換えるためにモデルを子とする
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

// 角度の設定
void PlayerWingParent::SetRot(int _num)
{
	if (m_rot < m_maxrot && m_rot > -m_maxrot)
	{
		m_rot += _num;
		m_rotflg = true;
		owner->transform->RotateXaxis(_num);
	}
}

// 角度の更新
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


