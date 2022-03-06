#include "SelectSceneCamera.h"

#define YPOS_OFFSET (3.0f)

using namespace Component;


SelectSceneCamera::SelectSceneCamera(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("MainCamera");

	tra = owner->transform;
	tra->position = Vector3{ 0.0f,0.0f,0.0f };
	tra->scale = Vector3{ 0.0f,0.0f,0.0f };

	cam = owner->AddComponent<Component::Camera>();
	cam->SetMainCamera(cam);

	cam->SetFar(10000);

	m_pos = Vector3(0, 0, 6.25f);

}

SelectSceneCamera::~SelectSceneCamera() {

}

void SelectSceneCamera::Update() {

	CameraUpdate();
}

void SelectSceneCamera::CameraUpdate()
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
void SelectSceneCamera::Start()
{
	// �J�����̈ʒu���v���C���[����擾
	player = SceneManager::Instance()->GameObjectFindName("player");
	playertra = player->transform;
	playercompo = player->GetComponent<Component::PlayerObj>();


	TPSRotateCamera(0.5f);
}

// �ʒu�̍X�V
void SelectSceneCamera::CameraPos()
{
	switch (camtype)
	{
	case SelectSceneCamera::CameraType::ROTATE:

		TPSRotateCamera(0.3f);
		break;

	case SelectSceneCamera::CameraType::STATIC:
		TPSCamera();
		break;

	default:
		break;
	}
}

// ����
void SelectSceneCamera::Input()
{

}

// �p�x�����W�A���ɕύX
inline float ToRad(float angle) {
	return (angle * DirectX::XM_PI) / 180.0f;
}

// ���ʑ����J����(�y����]�Ȃ��o�[�W����)
void SelectSceneCamera::UpdateSphereCamera(
	float radius,						// ���a(����)
	float elevation,					// �p�i���́j
	float azimuth,						// ���ʊp�i���́j
	const XMFLOAT3& lookat)	// �����_�i���́j
{
	//DirectX::XMFLOAT3 eye;				// ���_

	//// �p�A���ʊp�����W�A���ɕϊ�
	//elevation = ToRad(elevation);
	//azimuth = ToRad(azimuth);

	//// �p�ɉ��������a���v�Z
	//float r = radius * sinf(elevation);

	//// ���_���W���v�Z
	//eye.x = r * cosf(azimuth);
	//eye.y = radius * cosf(elevation);
	//eye.z = r * sinf(azimuth);

	//// ������x�N�g�����v�Z(�p�����X�O�x��]������Ƃł���)
	//DirectX::XMFLOAT3 up;
	//float elevationplus90 = elevation + ToRad(90.0f);			// �p���X�O�x����

	//r = radius * sinf(elevationplus90);

	//up.x = r * cosf(azimuth);
	//up.y = radius * cosf(elevationplus90);
	//up.z = r * sinf(azimuth);

	//// ���_�x�N�g���𒍎��_�����s�ړ�
	//eye.x += lookat.x;
	//eye.y += lookat.y;
	//eye.z += lookat.z;

	////tra->position = eye;
	//tra->position.x = eye.x + player->transform->position.x;
	//tra->position.y = eye.y + player->transform->position.y;
	//tra->position.z = eye.z + player->transform->position.z - 20;
	////cam->SetEye(tra->position);

	//this->transform->LookAt(lookat);
	//cam->CreateCameraMatrix();
}

// FPS�J����
void SelectSceneCamera::FPSCamera()
{
}

// TPS�J����
void SelectSceneCamera::TPSCamera()
{
	// �ꏊ
	//tra->position = player->transform->position - player->transform->forward() * 50.0f;

	tra->position = Vector3(0, 0, 6);
	// �N�I�[�^�j�I��������@
	cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());

	// lerp�g��
}

// �x��ĒǏ]����J����
void SelectSceneCamera::DelayCamera()
{
	static int svidx = 0;
	static bool firstflag = false;

	static std::array<XMFLOAT4, 100> qt;
	qt[svidx] = player->transform->qt;

	static std::array<Vector3, 100> forward;
	forward[svidx] = player->transform->forward();

	static std::array<Vector3, 100> up;
	up[svidx] = player->transform->up();

	static std::array<Vector3, 100> right;
	right[svidx] = player->transform->right();

	static std::array<Vector3, 100>pos;
	pos[svidx] = player->transform->position;


	svidx++;
	if (svidx > qt.size() - 1)
	{
		svidx = 0;
		firstflag = true;
	}
	// 30�t���[���O�̏����g�p���ăJ�������쐬
	int useidx = svidx - 2;
	if (useidx < 0)
	{
		if (!firstflag)
		{
			useidx = 0;
		}
		else
		{
			useidx = svidx + qt.size() - 2;
		}
	}


	// �ꏊ
	float length = 30;

	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;

	Vector3 inputVector;
	Vector3 _pos;

	// 
	inputVector.x = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRX) / 362.0f;
	inputVector.y = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY) / 362.0f;
	float yinputVector = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY) / 182.0f;

	//inputVector.x���C���Ƃɒ����Ċ|�����芄�����肵�Ă���
	float backx = inputVector.x;
	if (inputVector.x < 5 && -5 < inputVector.x) { inputVector.x = 0; }
	int xxx = inputVector.x;
	double radi = xxx * 3.141592653589793f / 360; _pos.x = length * sin(radi);

	if (inputVector.y < 5 && -5 < inputVector.y) { inputVector.y = 0; }
	// todo ���̒l���ȏ�̎��ɂ���
	radi = inputVector.y * 3.141592653589793f / 360;
	//if (radi < 0.75f)radi = 0.75f;

	_pos.y = length * sin(radi);

	float firstleng = 1;			// ��������
	const float confirstleng = 18;	// �Œ�̏�������

	if (inputVector.x == 0 && inputVector.y == 0)
	{
		firstleng = confirstleng;
	}

	float z = 0;
	float w = 0;
	// �N�I�[�^�j�I��������@
	cam->GetOwner()->transform->qt = qt[useidx];

	if ((Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{

		// ���E���͂ɑ΂��邚���W�̕␳�@
		if (_pos.x < 0)
		{
			z = _pos.x + 30;
		}
		else {
			z = 30 - _pos.x;
		}

		// ���͂��Ȃ��Ƃ��v�Z�̃o�O���Ȃ���
		if (_pos.x == 0)
		{
			_pos.x = 0.01f;
		}
		if (_pos.y == 0)
		{
			_pos.y = 0.01f;
		}

		//======== �J�����̉�]=========== //
		cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());

		// ���͂Ȃ��̏����l�̎�
		if (firstleng != confirstleng) {
			tra->position = pos[useidx]
				- forward[useidx] * _pos.y
				+ up[useidx] * 6
				- right[useidx] * _pos.x;
		}
		else {
			tra->position = pos[useidx]
				- forward[useidx] * firstleng
				+ up[useidx] * 6
				- right[useidx] * _pos.x;
		}
		Vector3 a = right[useidx] * _pos.x;
		//printf("%f %f %f \n",a.x,a.y,a.z );
	}
	else {

		if (playercompo->GetStreatFlg())
		{
			ShakeCamera();
		}


		Vector3 _pos = m_pos + m_randpos;

		// �ʏ펞
		tra->position = pos[useidx]
			- forward[useidx] * _pos.z + up[useidx] * (_pos.y) + tra->right() * _pos.x;
	}
}


// �X�^�[�g�̉��o�p
void SelectSceneCamera::TPSRotateCamera(float _pluscnt)
{
	Vector3 eye;				// ���_
	float radius = m_pos.z;					// ���a(����)
	float elevation = 75;					// �p�i���́j
	float _azimuth;						// ���ʊp�i���́j
	Vector3 lookat = player->transform->position;	// �����_�i���́j

	azimuth += _pluscnt;
	// �p�A���ʊp�����W�A���ɕϊ�
	elevation = ToRad(elevation);
	_azimuth = ToRad(azimuth);

	// �p�ɉ��������a���v�Z
	float r = radius * sinf(elevation);

	// ���_���W���v�Z
	eye.x = r * cosf(_azimuth);
	eye.y = radius * cosf(elevation);
	eye.z = r * sinf(_azimuth);

	// ���_�x�N�g���𒍎��_�����s�ړ�
	eye.x += lookat.x;
	eye.y += lookat.y;
	eye.z += lookat.z;

	tra->position = eye;

	cam->GetOwner()->transform->LookAt(lookat, player->transform->up());

}

// ��ʂ̃u��
void SelectSceneCamera::ShakeCamera()
{
	// �O�ɓ�������
	if (m_maxrandcnt > m_nowrandcnt)
	{
		m_nowrandcnt++;
		m_randpos.x = RandomClass::GetRandom(-0.1f, 0.1f);
		m_randpos.y = RandomClass::GetRandom(-0.1f, 0.1f);
	}
	else {
		m_nowrandcnt = 0;
		m_randpos = Vector3(0, 0, 0);
	}

}

// ���S���p
void  SelectSceneCamera::DeadCamera()
{
	cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());
}


//�@�X���[�v�ɂ���H�@���炩�ɂ�����

// �O������ϐ��ŊǗ�


//�p�x���݂�

// ������UI���h�ꂽ