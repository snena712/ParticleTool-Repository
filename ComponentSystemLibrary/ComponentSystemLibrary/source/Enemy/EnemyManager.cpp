#include "EnemyManager.h"
#include "../../DX11System/Controller.h"
using namespace Component;

EnemyManager::EnemyManager(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "EnemyManager");

}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Start()
{
	player = SceneManager::Instance()->GameObjectFindName("player");
	EnemyUpdate();
	EnemyCameraLock();
}

void EnemyManager::Update()
{
	// 角度取得
	EnemyUpdate();

	// 生存数の比較
	CompareLiveCnt();

	// 死んだ敵はいるか
	DeadFlgCheck();
}

void EnemyManager::SetList(std::vector<Vector3*> _pos)
{
	for (auto hoge : _pos)
	{
		// リストに追加
		GameObject* ene = new GameObject();
		ene->AddComponent<Component::Enemy>();
		m_objlist.emplace_back(ene);

		// 状態
		ene->transform->position = *hoge;

		// 角度のリストの初期化
		m_degreelist.emplace_back(0);
		m_lengthlist.emplace_back(0,nullptr);

	}
	SceneManager::Instance()->AddList(&m_objlist);

	// サイズ取得
	m_enelistsize = m_objlist.size();
	m_startlivrcnt = m_enelistsize;
}

void EnemyManager::EnemyUpdate()
{
	_num = 0;
	m_liveCount = 0;

	// プレイヤーとエネミーを結ぶベクトルを出します
	for (auto hoge : m_objlist)
	{
		SerchDegree(hoge);
	}
	
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE))
	{
		m_inputflg = true;
	}

	
	if (Controller::Instance()->GetButtonXTrigger()) {
			m_inputflg = true;
	}
	

	if (m_inputflg) {
		m_inputflg = false;
		if (m_nearcnt < m_liveCount - 1) {
			m_nearcnt++;
		}
		else {
			m_nearcnt = 0;
		}

		// 入力があったときのみ実行
		EnemyCameraLock();
	}
}

void EnemyManager::SerchDegree(GameObject* _obj)
{
	if (_obj->GetComponent<Component::Enemy>()->GetStatus() == true)
	{
		// 生きているとき
		m_liveCount++;

		// ２点間の距離
		Vector3 pos2 = _obj->transform->position - player->transform->position;

		// プレイヤーの向いている角度
		double degreep = atan2(player->transform->forward().x, player->transform->forward().z);

		//ベクトルを正規化します
		pos2.y = 0;
		pos2.Normalize();
		//プレイヤーとエネミーを結ぶベクトルの角度を計算します
		double degree;

		degree = atan2(pos2.x, pos2.z);

		degree = degree - degreep;

		m_degreelist[_num] = degree * 180 / 3.141592653589793f + 180;


		// に転換の距離
		float pos = player->transform->position.Distance(_obj->transform->position);

		if (pos < m_lengthmax) {
			// 距離
			m_lengthlist[_num] = std::make_pair( pos,_obj);
		}
		else {
			m_lengthlist[_num] = std::make_pair( m_lengthmax,_obj);
		}

	}
	else {
		// 死んでいる
		m_lengthlist[_num] = std::make_pair(20000,_obj);

	}
	// リストに保存するための変数
	_num++;
}


void EnemyManager::EnemyCameraLock()
{
	std::sort(m_lengthlist.begin(), m_lengthlist.end());//昇順ソート

	for (int i = m_nearcnt; i >= 0; i--)
	{
		if (m_lengthlist[m_nearcnt].first < m_maxdis
			&& m_lengthlist[m_nearcnt].second->GetComponent<Component::Enemy>()->GetStatus())
		{
			m_target = m_lengthlist[m_nearcnt].second;
			break;
		}
	}
}


// 死んだ敵はいるか？
void EnemyManager::DeadFlgCheck()
{
	for (auto list : m_objlist)
	{
		if (list->GetExistState())
		{
			if (list->GetComponent<Component::Enemy>()->GetStatus()==false)
			{
				m_deadFlg = true;
				EnemyCameraLock();
				break;
			}
		}
	}	
}


void EnemyManager::CompareLiveCnt()
{
	// 倒した数の取得
	m_deadcount = m_startlivrcnt - m_liveCount;

	// 倒した敵と初期値が同じとき
	if(m_startlivrcnt==m_deadcount)
	{
		MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::CLEAR);
	}
}
