#include "MainCamera.h"
#include "../Component/SceneManager.h"
#include "../Component/Scene.h"
#include "../DX11System/CDirectInput.h"
#include <array>

#define YPOS_OFFSET (3.0f)

using namespace Component;

XMFLOAT4X4 g_mtx;

MainCamera::MainCamera(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("MainCamera");

	tra = owner->transform;
	tra->position = Vector3{ 100.0f,30.0f,-100.0f };
	cam = owner->AddComponent<Component::Camera>();
	cam->SetMainCamera(cam);

	camtype = CameraType::DELAY;	// �J�����̎��

	cam->SetFar(10000);

}

MainCamera::~MainCamera() {
}

void MainCamera::Update() {

	CameraUpdate();
}

void MainCamera::CameraUpdate()
{

	// ��ԕω�
	Input();

	// �ʒu�̍X�V
	CameraPos();

	// ���_�̍X�V
}

// ������
void MainCamera::Start()
{
	// �V�[���ɂ���Đ؂�ւ�
	ControllerNum = Controller::Instance()->GetControllerNum();
	tra->position = Vector3{ 0.0f,10.0f,-50.0f };
}

// �ʒu�̍X�V
void MainCamera::CameraPos()
{	
	TPSCamera();
}

// ����
void MainCamera::Input()
{
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_F11))
	{
	
	}
}


// �p�x�����W�A���ɕύX
inline float ToRad(float angle) {
	return (angle * DirectX::XM_PI) / 180.0f;
}





// TPS�J����
void MainCamera::TPSCamera()
{

	bool _shift = false;
	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LSHIFT) || CDirectInput::GetInstance().CheckKeyBuffer(DIK_RSHIFT))
	{
		_shift = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFTARROW))
	{
		tra->position -= tra->right();
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHTARROW))
	{
		tra->position+= tra->right();
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UPARROW))
	{
		if (!_shift)
			tra->position+= tra->up();
		else
			tra->position+= tra->forward();
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWNARROW))
	{
		if (!_shift)
			tra->position -= tra->up();
		else
			tra->position -= tra->forward();
	}
	
	if (CDirectInput::GetInstance().GetMouseRButtonCheck())
	{
		auto _x = CDirectInput::GetInstance().GetMousePosX();
		auto _y = CDirectInput::GetInstance().GetMousePosY();
	
		if (pressFlg) {
			// �V�����ʒu�ƌÂ��ʒu�̎Q��
			m_mouseRightPressOldPos = m_mouseRightPressPos;
			m_mouseRightPressPos = Vector3(_x, _y, 0);
			
			auto _rpt = m_mouseRightPressOldPos.Direction(m_mouseRightPressPos);
			//printf("X:%f  Y:%f Z:%f \n", _rpt.x, _rpt.y, _rpt.z);

			// �΂߃m�[�}���C�Y�H
			tra->RotateLocalXaxis(_rpt.y/2);
			tra->RotateLocalYaxis(_rpt.x/2);
			//tra->RotateZaxis(_rpt.z);
		}
		else {
			pressFlg = true;
			
			// �V�����ʒu�ƌÂ��ʒu�̎Q��
			m_mouseRightPressPos = Vector3(_x, _y, 0);
		}
	}
	else {
		pressFlg = false;
	}


}



