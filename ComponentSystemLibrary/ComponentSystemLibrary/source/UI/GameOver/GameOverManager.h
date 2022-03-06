#pragma once
#include "../../../DX11System/CDirectInput.h"
#include "../../../Component/SceneManager.h"
#include "../../MainGameManager.h"
#include "../Option/OptionEndUI.h"
#include "../Option/OptionRetryUI.h"
#include "../Option/OptionSettingUI.h"
#include "../Option/OptionStartUI.h"
#include "../Option/OptionTitleUI.h"
#include "../../Scene/SelectBackUI.h"
#include "../../../DX11System/mystring.h"
namespace Component {

	class GameOverManager :public ComponentBase
	{
	private:
		int ControllerNum = -1;
		int m_optionNum;
		const int m_maxoptionNum = 5;

		bool m_inputflg = false;
		bool m_plessInputflg = false;
		bool m_yesflg = false;
		int m_nowinputcnt = 0;
		const int m_maxinputcnt = 60;


		GameObject* m_end;
		GameObject* m_retry;
		GameObject* m_start;
		GameObject* m_title;


		Component::FadeManager* m_fade;
		GameObject* m_backui;
		void EndUpdate();

		void InputKeybord();
		void InputController();

	public:
		GameOverManager(GameObject* _obj);
		~GameOverManager();

		void Start();
		void Update();
	};

}