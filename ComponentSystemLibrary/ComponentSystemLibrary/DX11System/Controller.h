#pragma once
#include <Windows.h>
#include <xinput.h>

#pragma comment (lib, "xinput.lib")

#define MAX_CONTROLLERS  4  //XInputが認識できるのは4つまで
#define Threshold   65535 //しきい値

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

	static int ControllerNum;						// コントローラー識別番号
	static bool init;

	// 振動用
	XINPUT_VIBRATION m_vibration;					// コントローラーの振動用変数
	int m_l_maxvibcnt = 0;							// 振動用カウント最大
	int m_l_nowvibcnt = 0;							// 現在の振動のカウント
	int m_r_maxvibcnt = 0;							// 振動用カウント最大
	int m_r_nowvibcnt = 0;							// 現在の振動のカウント
	float m_lpower = 0;
	float m_rpower = 0;

	// ボタン一覧
	ButtonTrigger ButtonA;
	ButtonTrigger ButtonB;
	ButtonTrigger ButtonX;
	ButtonTrigger ButtonY;
	ButtonTrigger ButtonRT;
	ButtonTrigger ButtonRS;
	ButtonTrigger ButtonLT;
	ButtonTrigger ButtonLS;

	void ShakeController();					// コントローラーの振動
	void CheckTriggerController();			// 入力の確認

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

	void SetControllerNum(int _num) { ControllerNum = _num; }	// コントローラーの番号割り振り
	int  GetControllerNum() { return ControllerNum; }			// コントローラ番号の取得

	bool CheckController();					// 接続確認

	void CheckConnect();

	void Init();
	void Update();
	void Exit();

	void ResetShakeLController();			// 左の振動の停止
	void ResetShakeRController();			// 右コントローラーの停止
	void ResetShakeAllController();			// 左コントローラーの停止

	void SetShakeController(float _lpower, int _lcnt, float _rpower, int _rcnt);	// 振動の設定（パワーの値は0～1の間に入れる）

	// トリガーの状態取得
	bool GetButtonATrigger() { return ButtonA.flg; };
	bool GetButtonBTrigger() { return ButtonB.flg; };
	bool GetButtonXTrigger() { return ButtonX.flg; };
	bool GetButtonYTrigger() { return ButtonY.flg; };
	bool GetButtonRTTrigger() { return ButtonRT.flg; };
	bool GetButtonRBTrigger() { return ButtonRS.flg; };
	bool GetButtonLTTrigger() { return ButtonLT.flg; };
	bool GetButtonLBTrigger() { return ButtonLS.flg; };
};


