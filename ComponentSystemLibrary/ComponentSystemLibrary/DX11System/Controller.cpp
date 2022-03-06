#include "Controller.h"

Controller::CONTROLER_STATE Controller::GAME_PAD[MAX_CONTROLLERS];
int Controller::ControllerNum = -1;
bool Controller::init = false;

void Controller::Init()
{
	ButtonA.flg = false;
	ButtonA.once = false;
		   
	ButtonB.flg = false;
	ButtonB.once = false;
		   
	ButtonX.flg = false;
	ButtonX.once = false;
		   
	ButtonY.flg = false;
	ButtonY.once = false;

	ButtonRT.flg = false;
	ButtonRT.once = false;
			
	ButtonRS.flg = false;
	ButtonRS.once = false;
			
	ButtonLT.flg = false;
	ButtonLT.once = false;
			
	ButtonLS.flg = false;
	ButtonLS.once = false;
}

void Controller::CheckConnect()
{

	DWORD dwResult;
	for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
	{
		dwResult = XInputGetState(i, &GAME_PAD[i].state);

		if (dwResult == ERROR_SUCCESS)
		{
			GAME_PAD[i].bConnected = true;
			if (!init)
			{
				SetControllerNum(i);
				init = true;
			}
		}
		else
			GAME_PAD[i].bConnected = false;
	}

}

// コントローラーチェック 番号取得面倒な時に使用
bool Controller::CheckController()
{

	// 指定番号に接続があるか？
	if (Controller::GAME_PAD[ControllerNum].bConnected)
	{
		return true;
	}

	return false;
}

void Controller::Update()
{
	// 入力の確認
	CheckTriggerController();

	// カメラの振動
	ShakeController();
}

void Controller::Exit()
{
	ResetShakeAllController();
	XInputSetState(0, &m_vibration);
}

// 入力の確認
void Controller::CheckTriggerController()
{
	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;

	// Aボタン
	if (Buttons & XINPUT_GAMEPAD_A)
	{
		if (!ButtonA.once)
		{
			ButtonA.once = true;
			ButtonA.flg = true;
		}
		else {
			ButtonA.flg = false;
		}
	}
	else if (!(Buttons & XINPUT_GAMEPAD_A))
	{
		ButtonA.once = false;
		ButtonA.flg = false;
	}		   

	// Bボタン
	if (Buttons & XINPUT_GAMEPAD_B)
	{
		if (!ButtonB.once)
		{
			ButtonB.once = true;
			ButtonB.flg = true;
		}
		else {
			ButtonB.flg = false;
		}
	}
	else if (!(Buttons & XINPUT_GAMEPAD_B))
	{
		ButtonB.once = false;
		ButtonB.flg = false;
	}

	// Xボタン
	if (Buttons & XINPUT_GAMEPAD_X)
	{
		if (!ButtonX.once)
		{
			ButtonX.once = true;
			ButtonX.flg = true;
		}
		else {
			ButtonX.flg = false;
		}
	}
	else if (!(Buttons & XINPUT_GAMEPAD_X))
	{
		ButtonX.once = false;
		ButtonX.flg = false;
	}

	// Yボタン
	if (Buttons & XINPUT_GAMEPAD_Y)
	{
		if (!ButtonY.once)
		{
			ButtonY.once = true;
			ButtonY.flg = true;
		}
		else {
			ButtonY.flg = false;
		}
	}
	else if (!(Buttons & XINPUT_GAMEPAD_Y))
	{
		ButtonY.once = false;
		ButtonY.flg = false;
	}

	// RTボタン
	if (Buttons & XINPUT_GAMEPAD_RIGHT_THUMB)
	{
		if (!ButtonRT.once)
		{
			ButtonRT.once = true;
			ButtonRT.flg = true;
		}
		else {
			ButtonRT.flg = false;
		}
	}
	else if (!(Buttons & XINPUT_GAMEPAD_RIGHT_THUMB))
	{
		ButtonRT.once = false;
		ButtonRT.flg = false;
	}

	// RSボタン
	if (Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
	{
		if (!ButtonRS.once)
		{
			ButtonRS.once = true;
			ButtonRS.flg = true;
		}
		else {
			ButtonRS.flg = false;
		}
	}
	else if (!(Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		ButtonRS.once = false;
		ButtonRS.flg = false;
	}


	// LTボタン
	if (Buttons & XINPUT_GAMEPAD_LEFT_THUMB)
	{
		if (!ButtonLT.once)
		{
			ButtonLT.once = true;
			ButtonLT.flg = true;
		}
		else {
			ButtonLT.flg = false;
		}
	}
	else if (!(Buttons & XINPUT_GAMEPAD_LEFT_THUMB))
	{
		ButtonLT.once = false;
		ButtonLT.flg = false;
	}

	// LSボタン
	if (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER)
	{
		if (!ButtonLS.once)
		{
			ButtonLS.once = true;
			ButtonLS.flg = true;
		}
		else {
			ButtonLS.flg = false;
		}
	}
	else if (!(Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{
		ButtonLS.once = false;
		ButtonLS.flg = false;
	}
}

// 振動
void Controller::ShakeController()
{
	bool _shakeflg;
	
	// 右の振動
	if (m_r_maxvibcnt > m_r_nowvibcnt)
	{
		m_r_nowvibcnt++;
	}
	else {
		ResetShakeRController();
	}

	// 左の振動
	if (m_l_maxvibcnt > m_l_nowvibcnt)
	{
		m_l_nowvibcnt++;
	}
	else {
		ResetShakeLController();
	}
	
	// 振動のチェック
	if (m_l_maxvibcnt != 0 && m_r_maxvibcnt != 0)
	{
		XInputSetState(0, &m_vibration);
	}
	else {
		
			ResetShakeAllController();
			XInputSetState(0, &m_vibration);
		
	}
	
}

void Controller::ResetShakeLController()
{
	m_l_maxvibcnt = 0;
	m_l_nowvibcnt = 0;

	m_lpower = 0;

	m_vibration.wLeftMotorSpeed = m_lpower;
}

void Controller::ResetShakeRController()
{
	m_r_maxvibcnt = 0;
	m_r_nowvibcnt = 0;

	m_rpower = 0;

	m_vibration.wRightMotorSpeed = m_rpower;
}

void Controller::ResetShakeAllController()
{
	m_r_maxvibcnt = 0;
	m_r_nowvibcnt = 0;

	m_l_maxvibcnt = 0;
	m_l_nowvibcnt = 0;

	m_rpower = 0;
	m_lpower = 0;

	m_vibration.wLeftMotorSpeed = m_lpower;
	m_vibration.wRightMotorSpeed = m_rpower;
}

void Controller::SetShakeController(float _lpower, int _lcnt, float _rpower, int _rcnt)
{
	if (_lpower > 1)_lpower = 1;
	if (_lpower < 0)_lpower = 0;

	if (_rpower > 1)_rpower = 1;
	if (_rpower < 0)_rpower = 0;

	m_vibration.wLeftMotorSpeed = 65535 * _lpower;
	m_vibration.wRightMotorSpeed = 65535 * _rpower;

	m_l_maxvibcnt = _lcnt;
	m_r_maxvibcnt = _rcnt;

	m_l_nowvibcnt = 0;
	m_r_nowvibcnt = 0;
}