#pragma once
#include "../Component/GameObject.h"
#include "../Component\CameraComponent.h"

class MainCamera :public GameObject
{
public:
	enum class CameraType{
		FPS,
		TPS,
		DELAY,
		STATIC
	};

private:
	 Transform* tra;
	 Component::Camera* cam;

	 CameraType camtype=CameraType::TPS;	// �J�����̎��

	 GameObject* player;

	 Transform*  playertra;
	 bool init = false;
	 bool once = false;	// �S�[���p
	 
	 int ControllerNum = -1; //�R���g���[���[

	 XMFLOAT3 pos;
	 XMFLOAT3 g_startpos;
	 int nowcnt = 0;	// ���݂̃J�E���g
	 int g_nowrotcnt = 0;
	 const int g_rotcnt = 45;	// �I���܂ł̃J�����ړ��J�E���g
	 bool GoalCamera();
	 void CameraPos();
	 void CameraLook();
	 void Input();

	 void UpdateSphereCamera(
		 float radius,					// ���a(����)
		 float elevation,				// �p�i���́j
		 float azimuth,					// ���ʊp�i���́j
		 const DirectX::XMFLOAT3& lookat);		// �����_�i���́j

	 void FPSCamera();
	 void TPSCamera();
	 void DelayCamera();		// �x���J����
	 void sssss();
public:
	MainCamera();
	~MainCamera();

	void CameraUpdate();

	void Update();
	void Draw();
	void Start();
};

