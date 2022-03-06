#pragma once
#include <Windows.h>
#include <xinput.h>

#pragma comment (lib, "xinput.lib")

#define MAX_CONTROLLERS  4  //XInput���F���ł���̂�4�܂�
#define Threshold   65535 //�������l

class Controller
{
private:
	typedef struct _CONTROLER_STATE
	{
		XINPUT_STATE state;
		bool bConnected;
	}CONTROLER_STATE;

	typedef struct ButtonTrigger
	{
		bool once;
		bool flg;
	};

	static int ControllerNum;						// �R���g���[���[���ʔԍ�
	static bool init;

	// �U���p
	XINPUT_VIBRATION m_vibration;					// �R���g���[���[�̐U���p�ϐ�
	int m_l_maxvibcnt = 0;							// �U���p�J�E���g�ő�
	int m_l_nowvibcnt = 0;							// ���݂̐U���̃J�E���g
	int m_r_maxvibcnt = 0;							// �U���p�J�E���g�ő�
	int m_r_nowvibcnt = 0;							// ���݂̐U���̃J�E���g
	float m_lpower = 0;
	float m_rpower = 0;

	// �{�^���ꗗ
	ButtonTrigger ButtonA;
	ButtonTrigger ButtonB;
	ButtonTrigger ButtonX;
	ButtonTrigger ButtonY;
	ButtonTrigger ButtonRT;
	ButtonTrigger ButtonRS;
	ButtonTrigger ButtonLT;
	ButtonTrigger ButtonLS;

	void ShakeController();					// �R���g���[���[�̐U��
	void CheckTriggerController();			// ���͂̊m�F

public:
	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;
	Controller(Controller&&) = delete;
	Controller& operator=(Controller&&) = delete;

	Controller() { }

	static Controller* Instance() {
		static Controller singleton;
		return &singleton;
	}

	static CONTROLER_STATE GAME_PAD[MAX_CONTROLLERS];

	void SetControllerNum(int _num) { ControllerNum = _num; }	// �R���g���[���[�̔ԍ�����U��
	int  GetControllerNum() { return ControllerNum; }			// �R���g���[���ԍ��̎擾

	bool CheckController();					// �ڑ��m�F

	void CheckConnect();

	void Init();
	void Update();
	void Exit();

	void ResetShakeLController();			// ���̐U���̒�~
	void ResetShakeRController();			// �E�R���g���[���[�̒�~
	void ResetShakeAllController();			// ���R���g���[���[�̒�~

	void SetShakeController(float _lpower, int _lcnt, float _rpower, int _rcnt);	// �U���̐ݒ�i�p���[�̒l��0�`1�̊Ԃɓ����j

	// �g���K�[�̏�Ԏ擾
	bool GetButtonATrigger() { return ButtonA.flg; };
	bool GetButtonBTrigger() { return ButtonB.flg; };
	bool GetButtonXTrigger() { return ButtonX.flg; };
	bool GetButtonYTrigger() { return ButtonY.flg; };
	bool GetButtonRTTrigger() { return ButtonRT.flg; };
	bool GetButtonRBTrigger() { return ButtonRS.flg; };
	bool GetButtonLTTrigger() { return ButtonLT.flg; };
	bool GetButtonLBTrigger() { return ButtonLS.flg; };
};


