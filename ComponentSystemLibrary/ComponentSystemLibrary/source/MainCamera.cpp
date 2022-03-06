#include "MainCamera.h"
#include "Component/SceneManager.h"
#include "Component/Scene.h"
#include "GoalClass.h"
#include "DX11System\input.h"
#include <array>
#define YPOS_OFFSET (3.0f)

XMFLOAT4X4 g_mtx;

MainCamera::MainCamera() {

	this->SetName("MainCamera");

	tra = this->transform;
	tra->position=Vector3{ 0.0f,0.0f,0.0f };
	tra->scale=Vector3{ 0.0f,0.0f,0.0f };
		
	cam = this->AddComponent<Component::Camera>();
	cam->SetMainCamera(cam);

	camtype = CameraType::TPS;	// �J�����̎��
}

MainCamera::~MainCamera() {

}

void MainCamera::Update() {
		
	CameraUpdate();
	
	//�R���|�[�l���g�X�V
	GameObject::Update();

}

void MainCamera::Draw() {

	//�R���|�[�l���g�`��
	GameObject::Draw();

}

void MainCamera::CameraUpdate()
{

	// �v���C���[�����݂��邩�H
	if (player != nullptr)
	{
		// �v���C���[�X�V
		playertra = player->transform;
	}

	// ��ԕω�
	Input();

	// �ʒu�̍X�V
	CameraPos();	
	
	// ���_�̍X�V
}

// ������
void MainCamera::Start()
{
	if (!init) 
	{
		Component::Camera* cam = this->AddComponent<Component::Camera>();

		switch (camtype)
		{
		case MainCamera::CameraType::FPS:
			// �J�����̈ʒu���v���C���[����擾
			player = GameObject::Find("player");
			playertra = player->transform;

			// �X�e�[�W�擾
			stage = dynamic_cast<StageManagerClass*>(GameObject::Find("StageManger"));
			goalUi = dynamic_cast<GoalUIClass*>(GameObject::Find("GoalUI"));
			
			// FPS�J�����̐ݒ�

			break;
		
		case MainCamera::CameraType::TPS:
			
			// �J�����̈ʒu���v���C���[����擾
			player = GameObject::Find("player");
			playertra = player->transform;

			// �X�e�[�W�擾
			stage = dynamic_cast<StageManagerClass*>(GameObject::Find("StageManger"));
			goalUi = dynamic_cast<GoalUIClass*>(GameObject::Find("GoalUI"));
			
			break;
		case MainCamera::CameraType::DELAY:
			// �J�����̈ʒu���v���C���[����擾
			player = GameObject::Find("player");
			playertra = player->transform;

			// �X�e�[�W�擾
			stage = dynamic_cast<StageManagerClass*>(GameObject::Find("StageManger"));
			goalUi = dynamic_cast<GoalUIClass*>(GameObject::Find("GoalUI"));
			break;

		case MainCamera::CameraType::STATIC:
			// �J�����̈ʒu���v���C���[����擾
			player = GameObject::Find("player");
			playertra = player->transform;

			// �X�e�[�W�擾
			stage = dynamic_cast<StageManagerClass*>(GameObject::Find("StageManger"));
			goalUi = dynamic_cast<GoalUIClass*>(GameObject::Find("GoalUI"));
			break;
		default:
			break;
		}
		init = true;
		
	}
}

// �ʒu�̍X�V
void MainCamera::CameraPos()
{
	auto scene = SceneManager::Instance()->GetActiveScene()->GetSceneName();

	switch (camtype)
	{
	case MainCamera::CameraType::FPS:
		if (scene == "Title") {
			FPSCamera();
		}
		break;

	case MainCamera::CameraType::TPS:

		if (scene == "Title") {
		//	tra->position = Vector3(playertra->position.x, 10, playertra->position.z - 8);
			TPSCamera();
		}

		/*if (scene == "Map1_1Class") {
			tra->position = Vector3(playertra->position.x, 10, playertra->position.z - 8);
		}

		if (scene == "Map1_2Class") {
			tra->position = Vector3(playertra->position.x, 10, playertra->position.z - 8);
		}

		if (scene == "Map1_3Class") {
			tra->position = Vector3(playertra->position.x, 10, playertra->position.z - 8);
		}*/
		break;

	case MainCamera::CameraType::DELAY:
		DelayCamera();
		break;

	case MainCamera::CameraType::STATIC:
		UpdateSphereCamera(10, 10, 10, (DirectX::XMFLOAT3(playertra->position.x, playertra->position.y, playertra->position.z)));
		break;


	default:
		break;
	}
}

// ����
void MainCamera::Input()
{
	if (GetKeyboardTrigger(DIK_F11))
	{
		// ��ԕω�
		switch (camtype)
		{
		case MainCamera::CameraType::FPS:
			camtype = CameraType::TPS;
			break;
		case MainCamera::CameraType::TPS:
			camtype = CameraType::DELAY;
			break;
		case MainCamera::CameraType::DELAY:
			camtype = CameraType::FPS;
			break;
		}
	}
}


// ���_�̍X�V�i���ݎg�p���ĂȂ��j
void MainCamera::CameraLook()
{
	DirectX::XMFLOAT3 c_lookat{0,0,0};

	switch (camtype)
	{
	case MainCamera::CameraType::FPS:
		c_lookat = DirectX::XMFLOAT3(playertra->position.x, playertra->position.y , playertra->position.z+2);

		printf("%f %f %f",c_lookat.x, c_lookat.y, c_lookat.z);
		system("CLS");

		// �S�[���̍X�V
		if (GoalCamera())
		{
			goalUi->Goal();
		}
		break;

	case MainCamera::CameraType::TPS:
		break;

	default:
		break;
	}

	// ���_�����܂��Ă��Ȃ����̉�����
	if (c_lookat.x == 0 && c_lookat.y == 0 && c_lookat.z == 0) {
		c_lookat.z = 1;
	}

	//�Z�b�g����
	cam->SetLookat(c_lookat);
}

// �S�[���̍X�V����
bool MainCamera::GoalCamera()
{	
	if (stage->GetGoalFlg()&&!once) 
	{
		once = true;

		// �J�n�ʒu�̕ۑ�
		g_startpos.x = tra->position.x;			// X���W�ۑ� 
		g_startpos.y = tra->position.y;			// Y���W�ۑ�
		g_startpos.z = tra->position.z;			// Z���W�ۑ�
	}
	
	float t;
	if (once)
	{	
		if (g_nowrotcnt < g_rotcnt) {

			pos.x = g_startpos.x;
			pos.y = g_startpos.y;
			pos.z = g_startpos.z;

			XMFLOAT3 endpos;
			t = static_cast<float>(g_nowrotcnt + 1) / static_cast<float>(g_rotcnt);

			// �I���ʒu���v�Z
			endpos.y = g_startpos.y - 3 * 2;
			endpos.z = g_startpos.z + 1 * 2;

			// ���`��Ԃ̎���X���W���v�Z
			pos.y = g_startpos.y * (1.0f - t) + endpos.y * t;
			pos.z = g_startpos.z * (1.0f - t) + endpos.z * t;

			// ��]�����J�E���g�A�b�v
			g_nowrotcnt++;
			tra->position.y = pos.y;
			tra->position.z = pos.z;
		}
		else {
			tra->position.y = pos.y;
			tra->position.z = pos.z;
			return true;
		}
		
	}
	return false;
}


// �p�x�����W�A���ɕύX
inline float ToRad(float angle) {
	return (angle * DirectX::XM_PI) / 180.0f;
}
  
// ���ʑ����J����(�y����]�Ȃ��o�[�W����)
void MainCamera::UpdateSphereCamera(
	float radius,						// ���a(����)
	float elevation,					// �p�i���́j
	float azimuth,						// ���ʊp�i���́j
	const DirectX::XMFLOAT3& lookat)	// �����_�i���́j
{

	DirectX::XMFLOAT3 eye;				// ���_

	// �p�A���ʊp�����W�A���ɕϊ�
	elevation = ToRad(elevation);
	azimuth = ToRad(azimuth);

	// �p�ɉ��������a���v�Z
	float r = radius * sinf(elevation);

	// ���_���W���v�Z
	eye.x = r * cosf(azimuth);
	eye.y = radius * cosf(elevation);
	eye.z = r * sinf(azimuth);

	// ������x�N�g�����v�Z(�p�����X�O�x��]������Ƃł���)
	DirectX::XMFLOAT3 up;
	float elevationplus90 = elevation + ToRad(90.0f);			// �p���X�O�x����

	r = radius * sinf(elevationplus90);

	up.x = r * cosf(azimuth);
	up.y = radius * cosf(elevationplus90);
	up.z = r * sinf(azimuth);

	// ���_�x�N�g���𒍎��_�����s�ړ�
	eye.x += lookat.x;
	eye.y += lookat.y;
	eye.z += lookat.z;
	

	// �r���[�ϊ��s����쐬����
	cam->SetEye(eye);
	//tra->position = eye;
	tra->position.x = eye.x + player->transform->position.x;
	tra->position.y = eye.y + player->transform->position.y;
	tra->position.z = eye.z + player->transform->position.z-20;
	//cam->SetEye(tra->position);

	cam->SetLookat(lookat);
	cam->SetUp(up);
	cam->CreateCameraMatrix();
}

// FPS�J����
void MainCamera::FPSCamera()
{
	// �v���C���[�̍s��擾
	XMFLOAT4X4 mtx;
	mtx = player->transform->GetMtx();
	// �r���[�ϊ��s���ύX
	static int svidx = 0;
	static bool firstflag = false;
	static std::array<XMFLOAT3, 100> zaxis;
	// Z��
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;
	static std::array<XMFLOAT3, 100> yaxis;
	// Y��
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;
	static std::array<XMFLOAT3, 100> pos;
	// ���ݒn
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;


	XMFLOAT3 eye;
	// ���_
	XMFLOAT3 lookat;
	// �����_
	XMFLOAT3 up;
	// ������x�N�g��
	lookat = pos[svidx];
	eye.x = pos[svidx].x - 1 * zaxis[svidx].x;
	eye.y = pos[svidx].y - 1 * zaxis[svidx].y;
	eye.z = pos[svidx].z - 1 * zaxis[svidx].z;

	up = yaxis[svidx];

	cam->SetEye(eye);
	tra->position = eye;
	tra->position.x = eye.x;
	tra->position.y = eye.y;
	tra->position.z = eye.z;
	cam->SetLookat(lookat);
	cam->SetUp(up);

	printf("%f %f %f\n", mtx._41, mtx._42, mtx._43);
	cam->CreateCameraMatrix();
}

// TPS�J����
void MainCamera::TPSCamera()
{
	// �v���C���[�̍s��擾
	XMFLOAT4X4 mtx;
	mtx = player->transform->GetMtx();
	// �r���[�ϊ��s���ύX
	static int svidx = 0;
	static bool firstflag = false;
	static std::array<XMFLOAT3, 100> zaxis;
	// Z��
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;
	static std::array<XMFLOAT3, 100> yaxis;
	// Y��
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;
	static std::array<XMFLOAT3, 100> pos;
	// ���ݒn
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;


	XMFLOAT3 eye;
	// ���_
	XMFLOAT3 lookat;
	// �����_
	XMFLOAT3 up;
	// ������x�N�g��
	lookat = pos[svidx];
	eye.x = pos[svidx].x - 30 * zaxis[svidx].x;
	eye.y = pos[svidx].y - 30 * zaxis[svidx].y;
	eye.z = pos[svidx].z - 30 * zaxis[svidx].z;

	up = yaxis[svidx];

	cam->SetEye(eye);
	tra->position.x = eye.x;
	tra->position.y = eye.y;
	tra->position.z = eye.z;
	cam->SetLookat(lookat);
	cam->SetUp(up);

	printf("%f %f %f\n", mtx._41, mtx._42, mtx._43);
	cam->CreateCameraMatrix();
}

// �x��ĒǏ]����J����
void MainCamera::DelayCamera()
{
	// �v���C���[�̍s��擾
	XMFLOAT4X4 mtx;
	mtx = player->transform->GetMtx();
	// �r���[�ϊ��s���ύX
	static int svidx = 0;
	static bool firstflag = false;
	static std::array<XMFLOAT3, 100> zaxis;
	// Z��
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;
	static std::array<XMFLOAT3, 100> yaxis;
	// Y��
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;
	static std::array<XMFLOAT3, 100> pos;
	// ���ݒn
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;
	svidx++;
	if (svidx > zaxis.size() - 1)
	{
		svidx = 0;
		firstflag = true;
	}
	// 30�t���[���O�̏����g�p���ăJ�������쐬
	int useidx = svidx - 30;
	if (useidx < 0)
	{
		if (!firstflag)
		{
			useidx = 0;
		}
		else
		{
			useidx = svidx + zaxis.size() - 30;
		}
	}
	XMFLOAT3 eye;
	// ���_
	XMFLOAT3 lookat;
	// �����_
	XMFLOAT3 up;
	// ������x�N�g��
	lookat = pos[useidx];

	eye.x = pos[useidx].x - 30 * zaxis[useidx].x;
	eye.y = pos[useidx].y - 30 * zaxis[useidx].y;
	eye.z = pos[useidx].z - 30 * zaxis[useidx].z;

	up = yaxis[useidx];

	cam->SetEye(eye);
	tra->position.x = eye.x;
	tra->position.y = eye.y;
	tra->position.z = eye.z;
	cam->SetLookat(lookat);
	cam->SetUp(up);

	printf("%f %f %f\n", mtx._41, mtx._42, mtx._43);
	cam->CreateCameraMatrix();
}


// �e�J�����̈ʒu�ɑ΂��鏉�����@�X�V
// �Œ�J�����̓���

