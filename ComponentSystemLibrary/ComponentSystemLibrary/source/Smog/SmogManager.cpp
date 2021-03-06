#include "SmogManager.h"

using namespace Component;

SmogManager::SmogManager(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("SmogManager");

	for (auto i = 0; i < smognum; i++)
	{
		// 雲生成
		GameObject* smog = new GameObject();
		smog->AddComponent<Component::SmogPlayer>();
		smog->transform->position = Vector3(10000, 0, 0);
		smog->SetExistState(false);
		// リストに追加
		smoglist.emplace_back(smog->GetComponent<Component::SmogPlayer>());
		SceneManager::Instance()->AddObject(smog, false);
	}	

	for (auto i = 0; i < wingnum; i++)
	{
		// 羽雲生成
		GameObject* wing = new GameObject();
		wing->AddComponent<Component::WingSmog>();
		wing->transform->position = Vector3(10000, 0, 0);
		wing->SetExistState(false);
		// リストに追加
		winglist.emplace_back(wing->GetComponent<Component::WingSmog>());
		SceneManager::Instance()->AddObject(wing, false);
	}

	for (auto i = 0; i < enenum; i++)
	{
		// 敵羽雲生成
		GameObject* ewing = new GameObject();
		ewing->AddComponent<Component::EnemyWingSmog>();
		ewing->transform->position = Vector3(10000, 0, 0);
		ewing->SetExistState(false);

		// リストに追加
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

// 引数を場所にする
void  SmogManager::SetActive(Vector3 _vec, Vector3 _size, XMFLOAT4 _color, Vector3 _pluszize, float _reduce)
{
	auto a= smoglist;
	for (auto list : smoglist)
	{
		// 死んでるとき
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
		// 死んでるとき
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
	// 死んだときに出すものにする
	for (auto list : smoglist)
	{
		// 死んでるとき
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
	// 通常時に出すものにする

	for (auto list : ewinglist)
	{
		// 死んでるとき
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
