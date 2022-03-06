#include "PlayerJatPartsParent.h"
#include "../../Component/SceneManager.h"

using namespace Component;
int _rot = 0;float _leng = 0.2f;
GameObject* _jet;
PlayerJatPartsParent::PlayerJatPartsParent(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("PlayerJatPartsParent");

	tra = owner->transform;

	// オブジェクトのシーンへの入れ込み
	std::vector<GameObject*> _list;	


	Vector3 _center;
	

	//for (int i = 0; i < m_jetnum; i++)
	//{
	//	GameObject* _jet = new GameObject();
	//	_jet->AddComponent<Component::PlayerJatParts>();
	//	_jet->transform->SetParent(owner->transform);
	//	_jet->transform->RotateZaxis(_rot);
	//	_rot += 45;

	//	double radi = _rot * 3.141592653589793f / 180;

	//	_jet->transform->position.x = _center.x + _leng  * cos(_rot);
	//	_jet->transform->position.y = _center.y + _leng  * sin(_rot);		

	//	m_jetlist.emplace_back(_jet);
	//	_list.emplace_back(_jet);
	//}

	_rot = 90;
	_jet = new GameObject();
	_jet->AddComponent<Component::PlayerJatParts>();
	_jet->transform->SetParent(owner->transform);
	_jet->transform->RotateLocalZaxis(_rot);
	
	_rot = 180;
	double radi = _rot * 3.141592653589793f / 180;
	_jet->transform->position.x = _center.x + _leng * cos(radi);
	_jet->transform->position.y = _center.y + _leng * sin(radi);		// 下がマイナスになったのでマイナス処理
	_list.emplace_back(_jet);


	//_rot = -40;
	//_jet = new GameObject();
	//_jet->AddComponent<Component::PlayerJatParts>();
	//_jet->transform->SetParent(owner->transform);
	//_jet->transform->RotateLocalZaxis(_rot);

	//_rot = 130;
	// radi = _rot * 3.141592653589793f / 180;
	//_jet->transform->position.x = _center.x + _leng * cos(radi);
	//_jet->transform->position.y = _center.y + _leng * sin(radi);		// 下がマイナスになったのでマイナス処理
	//_list.emplace_back(_jet);

	//_rot = -80;
	//_jet = new GameObject();
	//_jet->AddComponent<Component::PlayerJatParts>();
	//_jet->transform->SetParent(owner->transform);
	//_jet->transform->RotateLocalZaxis(_rot);

	//_rot = 170;
	//radi = _rot * 3.141592653589793f / 180;
	//_jet->transform->position.x = _center.x + _leng * cos(radi);
	//_jet->transform->position.y = _center.y + _leng * sin(radi);		// 下がマイナスになったのでマイナス処理
	//_list.emplace_back(_jet);


	

	SceneManager::Instance()->AddList(&_list);
}

PlayerJatPartsParent::~PlayerJatPartsParent()
{

}

void PlayerJatPartsParent::Start()
{
	player = SceneManager::Instance()->GameObjectFindName("player");

	owner->transform->SetParent(player->transform);
}

void PlayerJatPartsParent::Update()
{
	RotUpdate();
	//_jet->transform->RotateLocalZaxis(1);
}

// 角度の設定
void PlayerJatPartsParent::SetRot(int _num)
{
	if (m_rot < m_maxrot && m_rot > -m_maxrot)
	{
		m_rot += _num;
		m_rotflg = true;
		//owner->transform->RotateLocalXaxis(_num);
	}
}

// 角度の更新
void PlayerJatPartsParent::RotUpdate()
{	//---------------------------
	// 全ての子の角度と位置設定
	//---------------------------

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


