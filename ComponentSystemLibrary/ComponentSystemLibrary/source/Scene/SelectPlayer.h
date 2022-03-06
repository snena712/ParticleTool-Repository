#pragma once
#include "../../Component/GameObject.h"
#include "../../Component\RigidbodyComponent.h"
#include "../../Component/ModelComponent.h"
#include "../../DX11System/dx11mathutil.h"
#include "../../DX11System/DX11util.h"
#include "../StageManagerClass.h"
#include "../BulletManager.h"
#include "../../source/ObjectShape/Cone.h"
#include "../Smog/SmogManager.h"
#include "../../DX11System/RandomClass.h"
#include "../UI/Fade/FadeManager.h"
#include "SelectUnderbarUI.h"

namespace Component {

	class SelectPlayer :public ComponentBase
	{
	public :
		enum class Select
		{
			NONE,
			GAMESTART,
			STAGESELECT,
			OPTION,
			SCORE,
			WAIT,
			END
		};

		enum class InputType
		{
			VERTEX,
			WIDOS
		};

		enum ColorMODE
		{
			NONE,
			UP,
			DOWN
		};

	private:
		//----�R���|�[�l���g------//
		Transform* tra;
		Component::Rigidbody* rb;
		Component::ModelRender* model;

		Select m_select= Select::NONE;				// ���߂̑I��

		const XMFLOAT4 m_maxcolor = XMFLOAT4(0, 1.0f, 0.0f, 0.7f);
		XMFLOAT4 m_color;
		ColorMODE colorMode= ColorMODE::NONE;

		// �ʒu
		DirectX::XMFLOAT3 g_startpos;	// �L�[���͂��ꂽ�ۂ̊J�n�ʒu

		// �R���g���[���[
		int ControllerNum = -1;
		bool m_inputflg = false;		
		bool m_plessInputflg = false;// �A�����͒�~
		
		// ���͂ɑ΂���ϐ�
		int m_nowinputcnt = 0;
		const int m_maxinputcnt = 20;
		bool m_button = false;
		bool m_cancelflg=false;
		int m_inputcnt = 0;				// ���͂̃J�E���g
		int m_inputnum = 0;				// ���݂̓���

		// �t�h�ړ��p
		bool m_slideflg = false;
		float m_slidepos = 0;
		int m_nowsldcnt = 0;				//  
		const int m_sldcntmax = 30;			// �J�E���g
		
		// ��ʐ؂�ւ���
		bool m_chanflg = false;				// �ؑփt���O
		int m_nowchancnt = 0;				//  
		const int m_chancntmax = 15;		// �J�E���g

		// �V�[���ǂݍ��ݗp
		std::string m_loadscnestr;

		// �������p
		int m_strlen=0;						// ���݂̉����������̒���

		// �e���ڂ̕�����
		const std::string select=("SELECT");
		const std::string start=("START");
		const std::string tutorial=("TUTORIAL");
		const std::string setting=("SETTING");
		const std::string backtitle=("BACK TO TITLE");
		const std::string end=("END");


		const std::string stage=(": STAGE");
		const std::string stage1=("STAGE-1");
		const std::string stage2=("STAGE-2");
		const std::string stage3=("STAGE-3");
		const std::string back=("BACK");

		InputType m_type=InputType::VERTEX;

		// �Q�[���X�^�[�g�p�ϐ�
		int m_gamestartnum = 0;		

		GameObject* fade;
		GameObject* line;

		//-------�O���N���X�̕ϐ�--------//

		//-------�֐�--------//

		void KeyBord();							// �L�[�{�[�h

		void Controller();
		void InputCnt();

		void SelectUpdate();
		void GameStart();
		void GameEnd();
		void StageSelect();

		void String();
		void LinePos();
		bool Slider();
		bool ColorChange();

		void StrLength();

		void InputLimit();
		bool GetChangeFlg() { return m_chanflg; }

		void SetChangeFlg(ColorMODE _mode) { colorMode = _mode; }

	public:
		SelectPlayer(GameObject* _obj);
		~SelectPlayer();

		//----��{�֐�----//
		void Start();
		void Update();
		void DrawObject();
		void Input();

		bool GetInputFlg() { return m_inputflg; }
	};

}