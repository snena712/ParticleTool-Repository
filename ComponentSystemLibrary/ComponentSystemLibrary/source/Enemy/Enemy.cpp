#include "Enemy.h"


using namespace Component;

Enemy::Enemy(GameObject *_owner):ComponentBase(_owner)
{
	owner->SetName("enemy");

	tra = owner->transform;

	tra->scale = 2;

	// ���f���̐ݒ�
	model = owner->AddComponent<Component::ModelRender>(
		"assets/f1/f1.x",			// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/ps.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/f1/");				// �e�N�X�`���̑��݃t�H���_

	//�摜�̓��b�N�I���Ɣ��j
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/target.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetVertexColor(XMFLOAT4(0, 1, 0, 1));

	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();

	col = owner->AddComponent<Component::SphereCollider>();
	col->SetRadius(10);

	status = Status::LIVE;

	// �ړ�����
	RandMove();

	// �����̃t���[���ǉ�
	GameObject* _frame = new GameObject();
	frame=_frame->AddComponent<Component::EnemyPosFrame>();  //�R���|�[�l���g�ł��̃G�l�~�[�n��
	_frame->GetComponent< Component::EnemyPosFrame>()->SetParent(owner);
	SceneManager::Instance()->AddObject(_frame, false);

	bomb = new GameObject();
	bomb->AddComponent<Component::BombAnim>();  //�R���|�[�l���g�ł��̃G�l�~�[�n��
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
		////�ۑ����ꂽ		
		frame->CheckScreen();
	}
	StatusUpdate();

}

void Enemy::DrawObject()
{
	// ���b�N�I����
	LockOnUpdate();

	if (status == Status::LIVE) {
		//GameObject::Draw();
	}
}

// �ړ������̍X�V
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

// �ړ��X�V
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

	//��������@�@�����≜�s�s���߂����Ƃ�

	// �ړ��̍X�V
	if (m_movecnt < m_nowmovecnt) {
		RandMove();
	}
	else {
		m_nowmovecnt++;
	}

	AutoGo(tra->forward());
}

// �����蔻��
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

// ��Ԃ̍X�V
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

// ���S�̏���
void Enemy::DeadUpdate()
{
	if (m_maxdeadcnt > m_nowdeadcnt)
	{
		if (m_nowdeadcnt == 0) {
			// �t���[���폜�@���������@
			frame->GetOwner()->SetExistState(false);
			bomb->SetExistState(true);
		}
		m_nowdeadcnt++;
		DeadFly(tra->forward());
	}
	else {

		owner->SetExistState(false);
	}
	// �����̏ꏊ�̍X�V
	bomb->transform->position = owner->transform->position;
}

// �����ړ�
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

// �O�Ƃ̗͔̑�r
void Enemy::LifeCheck()
{
	if (life != oldlife) {

		oldlife = life;

		/*TestComputeObj* test = new TestComputeObj(this->transform->position);
		SceneManager::Instance()->AddObject(test, false);
		test->GetComponent<Component::ComputeComponent>()->SetStatus(StatusCompute::Live);
	*/}
}

// ���b�N�I���̍X�V
void Enemy::LockOnUpdate()
{
	// ���b�N�I����
	if (m_lockoncnt != 0)
	{
		// �T�C�Y�ύX
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
			// ������
			ren->SetHeight(1);
			ren->SetWidth(1);
			ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
			m_lockoncnt = 0;
		}
	}
}

void Enemy::DeadFly(XMFLOAT4 _axisZ)
{
	// �����Ă�����ɗ�����
	straightSpeed = 1;		// ���x�͌��݂̃X�s�[�h
	tra->position.x += _axisZ.x * straightSpeed;
	tra->position.y += _axisZ.y * straightSpeed;
	tra->position.z += _axisZ.z * straightSpeed;

	tra->RotateXaxis(0.5f);

	const int hoge = 10;
	for (int i = 0; i < hoge; i++)
	{// �傫��5�ŉ��ۂ�����
		smogmana->SetEnemyDeadSmog(tra->position + tra->forward() * straightSpeed * i / hoge, Vector3(5, 5, 5), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.8f), Vector3(0.6f, 0.6f, 0.6f), 0.03f);
		smogmana->SetEnemyDeadSmog(tra->position + tra->forward() * straightSpeed * i / hoge, Vector3(5, 5, 5), XMFLOAT4(0.1f, 0.1f, 0.1f, 0.8f), Vector3(0.6f, 0.6f, 0.6f), 0.03f);
	}
}

void Enemy::SmogFly()
{
	if (m_maxsmogcnt < m_nowsmogcnt)
	{
		//���J�E���g���Ƃɐ���
		// �傫��5�ŉ��ۂ�����
		m_nowsmogcnt = 0;

		smogmana->SetEnemySmog(tra->position + tra->right() * 7 + tra->forward(), tra->qt, Vector3(0.5f, 4.5f, 2), XMFLOAT4(1, 1, 1, 1), Vector3(0, 0, 0), 0.05f);
		smogmana->SetEnemySmog(tra->position - tra->right() * 7 + tra->forward(), tra->qt, Vector3(0.5f, 4.5f, 2), XMFLOAT4(1, 1, 1, 1), Vector3(0, 0, 0), 0.05f);
	}
	else {
		m_nowsmogcnt++;
	}
}