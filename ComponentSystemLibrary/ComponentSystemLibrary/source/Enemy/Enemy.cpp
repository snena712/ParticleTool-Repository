#include "Enemy.h"


using namespace Component;

Enemy::Enemy(GameObject *_owner):ComponentBase(_owner)
{
	owner->SetName("enemy");

	tra = owner->transform;

	tra->scale = 2;

	// モデルの設定
	model = owner->AddComponent<Component::ModelRender>(
		"assets/f1/f1.x",			// モデルデータ			// カレントファイルからの読み込み
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/ps.hlsl",			// ピクセルシェーダー
		"assets/f1/");				// テクスチャの存在フォルダ

	//画像はロックオンと爆破
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/target.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetVertexColor(XMFLOAT4(0, 1, 0, 1));

	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();

	col = owner->AddComponent<Component::SphereCollider>();
	col->SetRadius(10);

	status = Status::LIVE;

	// 移動方向
	RandMove();

	// 自分のフレーム追加
	GameObject* _frame = new GameObject();
	frame=_frame->AddComponent<Component::EnemyPosFrame>();  //コンポーネントでこのエネミー渡す
	_frame->GetComponent< Component::EnemyPosFrame>()->SetParent(owner);
	SceneManager::Instance()->AddObject(_frame, false);

	bomb = new GameObject();
	bomb->AddComponent<Component::BombAnim>();  //コンポーネントでこのエネミー渡す
	SceneManager::Instance()->AddObject(bomb, false);
	bomb->SetExistState(false);
}

Enemy::~Enemy()
{
}

void Enemy::Start()
{
	smogmana= SceneManager::Instance()->GameObjectFindName("SmogManager")->GetComponent<Component::SmogManager>();
	//smog
}

void Enemy::Update()
{
	if (status == Status::LIVE
		&&MainGameManager::GetInstance()->GetSceneState()==MainGameManager::SceneState::NORMAL
		||MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::START) {
		//GameObject::Update();

		MoveUpdate();
		////保存された		
		frame->CheckScreen();
	}
	StatusUpdate();

}

void Enemy::DrawObject()
{
	// ロックオン時
	LockOnUpdate();

	if (status == Status::LIVE) {
		//GameObject::Draw();
	}
}

// 移動方向の更新
void Enemy::RandMove()
{

	int num = RandomClass::GetRandom(0,5);

	switch (num)
	{
	case 0:
		movedir = MoveDir::UP;
		break;
	case 1:
		movedir = MoveDir::DOWN;
		break;
	case 2:
		movedir = MoveDir::UP;
		break;
	case 5:
		movedir = MoveDir::DOWN;
		break;
	case 3:
		movedir = MoveDir::LSPIN;
		break;
	case 4:
		movedir = MoveDir::RSPIN;
		break;
	default:
		break;
	}

	m_nowmovecnt = 0;
	movedir = MoveDir::UP;

}

// 移動更新
void Enemy::MoveUpdate()
{
	switch (movedir)
	{
	case Enemy::MoveDir::UP:
		tra->RotateLocalXaxis(-0.1f);
		break;
	case Enemy::MoveDir::DOWN:
		tra->RotateLocalXaxis(0.1f);
		break;
	case Enemy::MoveDir::RIGHT:
		//tra->RotateYaxis(1.0f);
		break;
	case Enemy::MoveDir::LEFT:
		//tra->RotateLocalYaxis(-1.0f);
		break;
	case Enemy::MoveDir::STRREAT:
		straightSpeed += speed;
		break;
	case Enemy::MoveDir::SLOW:
		straightSpeed -= speed;
		break;
	case Enemy::MoveDir::LSPIN:
		tra->RotateLocalZaxis(-1.0f);
		break;
	case Enemy::MoveDir::RSPIN:
		tra->RotateLocalZaxis(1.0f);
		break;

	default:
		break;
	}

	//条件つける　　高さや奥行行き過ぎたとき

	// 移動の更新
	if (m_movecnt < m_nowmovecnt) {
		RandMove();
	}
	else {
		m_nowmovecnt++;
	}

	AutoGo(tra->forward());
}

// 当たり判定
void Enemy::Collision()
{
	bool _flg = false;
	for (auto& _rigidlist : *rb->GetCollisionObject())
	{
		if (_rigidlist.obj->GetName() == "player") {

			//_rigidlist.obj->GetComponent<Component::PlayerObj>()->SetStatus(Component::PlayerObj::Status::DEAD);
		}
	}
	if (!_flg)
	{
		//col->SetColor(XMFLOAT4(1, 1, 1, 0.5f));
	}

	//Component::EnemyManager* ene;
}

// 状態の更新
void Enemy::StatusUpdate()
{
	switch (status)
	{
	case Enemy::LIVE:
		Collision();
		LifeCheck();
		SmogFly();
		if (life <= 0)
		{
			status = Status::DEAD; 
			frame->GetOwner()->SetExistState(false);
		}
		
		break;

	case Enemy::DEAD:
		DeadUpdate();
		break;
	default:
		break;
	}
}

// 死亡の処理
void Enemy::DeadUpdate()
{
	if (m_maxdeadcnt > m_nowdeadcnt)
	{
		if (m_nowdeadcnt == 0) {
			// フレーム削除　爆発発生　
			frame->GetOwner()->SetExistState(false);
			bomb->SetExistState(true);
		}
		m_nowdeadcnt++;
		DeadFly(tra->forward());
	}
	else {

		owner->SetExistState(false);
	}
	// 爆発の場所の更新
	bomb->transform->position = owner->transform->position;
}

// 自動移動
void Enemy::AutoGo(XMFLOAT4 _axisZ)
{
	if (straightSpeed < m_minSpeed)
	{
		straightSpeed = m_minSpeed;
	}
	else if (straightSpeed > m_maxSpeed) {
		straightSpeed = m_maxSpeed;
	}
	if (0 < straightSpeed)
	{
		tra->position.x += _axisZ.x * straightSpeed;
		tra->position.y += _axisZ.y * straightSpeed;
		tra->position.z += _axisZ.z * straightSpeed;
	}
	else
	{
		tra->position.x += _axisZ.x * straightSpeed / 5;
		tra->position.y += _axisZ.y * straightSpeed / 5;
		tra->position.z += _axisZ.z * straightSpeed / 5;
	}
}

// 前との体力比較
void Enemy::LifeCheck()
{
	if (life != oldlife) {

		oldlife = life;

		/*TestComputeObj* test = new TestComputeObj(this->transform->position);
		SceneManager::Instance()->AddObject(test, false);
		test->GetComponent<Component::ComputeComponent>()->SetStatus(StatusCompute::Live);
	*/}
}

// ロックオンの更新
void Enemy::LockOnUpdate()
{
	// ロックオン時
	if (m_lockoncnt != 0)
	{
		// サイズ変更
		ren->SetHeight(20 - (float)m_lockoncnt / 3);
		ren->SetWidth(20 - (float)m_lockoncnt / 3);

		if (lockonFlg)
		{
			ren->SetVertexColor(XMFLOAT4(1, 0, 0, 1));
		}
		lockonFlg = false;
	}
	else if (m_lockoncnt == 0) {
		
		if (!lockonFlg) 
		{
			// 初期化
			ren->SetHeight(1);
			ren->SetWidth(1);
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
			m_lockoncnt = 0;
		}
	}
}

void Enemy::DeadFly(XMFLOAT4 _axisZ)
{
	// 向いてる向きに落ちる
	straightSpeed = 1;		// 速度は現在のスピード
	tra->position.x += _axisZ.x * straightSpeed;
	tra->position.y += _axisZ.y * straightSpeed;
	tra->position.z += _axisZ.z * straightSpeed;

	tra->RotateXaxis(0.5f);

	const int hoge = 10;
	for (int i = 0; i < hoge; i++)
	{// 大きさ5で煙ぽかった
		smogmana->SetEnemyDeadSmog(tra->position + tra->forward() * straightSpeed * i / hoge, Vector3(5, 5, 5), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.8f), Vector3(0.6f, 0.6f, 0.6f), 0.03f);
		smogmana->SetEnemyDeadSmog(tra->position + tra->forward() * straightSpeed * i / hoge, Vector3(5, 5, 5), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.8f), Vector3(0.6f, 0.6f, 0.6f), 0.03f);
	}
}

void Enemy::SmogFly()
{
	if (m_maxsmogcnt < m_nowsmogcnt)
	{
		//一定カウントごとに生成
		// 大きさ5で煙ぽかった
		m_nowsmogcnt = 0;

		smogmana->SetEnemySmog(tra->position + tra->right() * 7 + tra->forward(), tra->qt, Vector3(0.5f, 4.5f, 2), XMFLOAT4(1, 1, 1, 1), Vector3(0, 0, 0), 0.05f);
		smogmana->SetEnemySmog(tra->position - tra->right() * 7 + tra->forward(), tra->qt, Vector3(0.5f, 4.5f, 2), XMFLOAT4(1, 1, 1, 1), Vector3(0, 0, 0), 0.05f);
	}
	else {
		m_nowsmogcnt++;
	}
}