#include "SmogManager.h"

using namespace Component;

SmogManager::SmogManager(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("SmogManager");

	for (auto i = 0; i < smognum; i++)
	{
		// ‰_¶¬
		GameObject* smog = new GameObject();
		smog->AddComponent<Component::SmogPlayer>();
		smog->transform->position = Vector3(10000, 0, 0);
		smog->SetExistState(false);
		// ƒŠƒXƒg‚É’Ç‰Á
		smoglist.emplace_back(smog->GetComponent<Component::SmogPlayer>());
		SceneManager::Instance()->AddObject(smog, false);
	}	

	for (auto i = 0; i < wingnum; i++)
	{
		// ‰H‰_¶¬
		GameObject* wing = new GameObject();
		wing->AddComponent<Component::WingSmog>();
		wing->transform->position = Vector3(10000, 0, 0);
		wing->SetExistState(false);
		// ƒŠƒXƒg‚É’Ç‰Á
		winglist.emplace_back(wing->GetComponent<Component::WingSmog>());
		SceneManager::Instance()->AddObject(wing, false);
	}

	for (auto i = 0; i < enenum; i++)
	{
		// “G‰H‰_¶¬
		GameObject* ewing = new GameObject();
		ewing->AddComponent<Component::EnemyWingSmog>();
		ewing->transform->position = Vector3(10000, 0, 0);
		ewing->SetExistState(false);

		// ƒŠƒXƒg‚É’Ç‰Á
		ewinglist.emplace_back(ewing->GetComponent<Component::EnemyWingSmog>());
		SceneManager::Instance()->AddObject(ewing, false);
	}
}

SmogManager::~SmogManager()
{
}

void SmogManager::Start()
{

}

void SmogManager::Update()
{
}

// ˆø”‚ðêŠ‚É‚·‚é
void  SmogManager::SetActive(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce)
{
	auto a= smoglist;
	for (auto list : smoglist)
	{
		// Ž€‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == SmogPlayer::Status::DEAD)
		{
			list->SetStatus(SmogPlayer::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _vec;
			list->SetAlpha(_color);
			list->SetSize(_size.x, _size.y);
			list->SetReduce(_reduce);
			list->SetPlusSize(_pluszize);
			list->SetType(SmogPlayer::ObjType::Player);
			break;
		}
	}
}


void  SmogManager::SetWingSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, float _reduce)
{
	for (auto list : winglist)
	{
		// Ž€‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == WingSmog::Status::DEAD)
		{
			list->SetStatus(WingSmog::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _vec;
			list->SetAlpha(_color);
			list->SetSize(_size.x, _size.y);
			list->SetReduce(_reduce);
			break;
		}
	}
}

void  SmogManager::SetEnemyDeadSmog(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce)
{
	// Ž€‚ñ‚¾‚Æ‚«‚Éo‚·‚à‚Ì‚É‚·‚é
	for (auto list : smoglist)
	{
		// Ž€‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == SmogPlayer::Status::DEAD)
		{
			list->SetStatus(SmogPlayer::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _vec;
			list->SetAlpha(_color);
			list->SetSize(_size.x, _size.y);
			list->SetReduce(_reduce);
			list->SetPlusSize(_pluszize);
			list->SetType(SmogPlayer::ObjType::Enemy);

			break;
		}
	}
}

void  SmogManager::SetEnemySmog(Vector3 _vec, XMFLOAT4 _qt, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce)
{
	// ’ÊíŽž‚Éo‚·‚à‚Ì‚É‚·‚é

	for (auto list : ewinglist)
	{
		// Ž€‚ñ‚Å‚é‚Æ‚«
		if (list->GetStatus() == EnemyWingSmog::Status::DEAD)
		{

			list->SetStatus(EnemyWingSmog::Status::LIVE);
			list->GetOwner()->SetExistState(true);
			list->GetOwner()->transform->position = _vec;
			list->GetOwner()->transform->qt = _qt;			
			list->SetAlpha(_color);
			list->SetSize(_size.x, _size.y);
			list->SetReduce(_reduce);
			list->SetPlusSize(_pluszize);
			break;
		}
	}

}
