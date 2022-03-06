#include "BulletEffectManager.h"

using namespace Component;

BulletEffectManager::BulletEffectManager(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("BulletEffectManager");

	std::vector<GameObject*> _list;

	// ’e‰Šú‰»
	for (int i = 0; i < m_effectnum; i++)
	{
		GameObject* _effect = new GameObject();
		_effect->transform->position.x = 10000;
		_effect->AddComponent<Component::BulletEffect>();
		m_effectList.emplace_back(_effect);		
		_effect->SetExistState(false);
	}
	SceneManager::Instance()->AddList(&m_effectList);
}

BulletEffectManager::~BulletEffectManager()
{
}

void BulletEffectManager::Start()
{

}

void BulletEffectManager::Update()
{

}



void BulletEffectManager::Fire(Vector3 _pos)
{

	for (auto list : m_effectList) {

		if (!list->GetExistState())
		{
			list->SetExistState(true);
			list->transform->position = _pos;

			break;
		}
	}

}


