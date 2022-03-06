#include "JetFire.h"
#include "../../Component/SceneManager.h"

Vector3 g_pos(0, 3, 0);

using namespace Component;

JetFire::JetFire(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("Bullet");

	tra = owner->transform;

	for (int i = 0; i < conenum; i++) {
		Cone* cone = new Cone();
		cone->Init(XMFLOAT4(1, 0, 0, 0.5f));
		conelist.emplace_back(cone);
	}
}

JetFire::~JetFire()
{

}

void JetFire::Start()
{
	player = SceneManager::Instance()->GameObjectFindName("player")->GetComponent<Component::PlayerObj>();
}

void JetFire::Update()
{
	if (player->GetStatus() == Status::LIVE) {
		
		FlickerFire();
	}
	else {
		owner->SetExistState(false);
	}

}

void JetFire::DrawObject()
{
	int forward = 10;
	XMFLOAT4 col = XMFLOAT4(1.0f, 1.0f, 0, 0.5f);
	// リスト描画

	float _speed = player->GetSpeed();
	float _maxspeed = player->GetMaxSpeed();

	float _rate = _speed / _maxspeed;

	// 1　黄
	col.x = 1;
	col.y = 1;
	g_pos.y = (2 + m_firesize) * _rate;
	conelist[0]->BufferUpdate(col, g_pos);
	//i++;
	conelist[0]->SetTransform(player->GetOwner()->transform, player->GetOwner()->transform->position - player->GetOwner()->transform->up() * 0.5f - player->GetOwner()->transform->forward() * forward, player->GetOwner()->transform->qt, 1, 1);
	conelist[0]->Draw();

	// 2　オレンジ
	col.x = 1.0f;
	col.y = 0.6f;
	g_pos.y = (4 + m_firesize) * _rate;
	conelist[1]->BufferUpdate(col, g_pos);
	//i++;
	conelist[1]->SetTransform(player->GetOwner()->transform, player->GetOwner()->transform->position - player->GetOwner()->transform->up() * 0.5f - player->GetOwner()->transform->forward() * forward, player->GetOwner()->transform->qt, 1, 1);
	conelist[1]->Draw();

	// 3 赤
	col.x = 1.0f;
	col.y = 0.0f;
	g_pos.y = (6 + m_firesize) * _rate;
	conelist[2]->BufferUpdate(col, g_pos);
	//i++;
	conelist[2]->SetTransform(player->GetOwner()->transform, player->GetOwner()->transform->position - player->GetOwner()->transform->up() * 0.5f - player->GetOwner()->transform->forward() * forward, player->GetOwner()->transform->qt, 1, 1);
	conelist[2]->Draw();

	g_pos.z -= 1;
}

// 火のちらつき
void JetFire::FlickerFire()
{
	// FPSが上がると間隔あった方がいいかも？

	/*if (m_nowFlickcnt > m_maxFlickcnt)
	{*/
	m_nowFlickcnt = 0;

	static std::mt19937_64 mt64(0);
	std::random_device rnd;     // 非決定的な乱数生成器を生成
	std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
	std::uniform_real_distribution<> rand12(0.0f, 1.0f);

	m_firesize = (float)rand12(mt);

	std::uniform_real_distribution<> randmax(0.0, 1.0);

	m_maxFlickcnt = (float)randmax(mt);
	/*}
	else {
		m_nowFlickcnt++;
	}*/
}