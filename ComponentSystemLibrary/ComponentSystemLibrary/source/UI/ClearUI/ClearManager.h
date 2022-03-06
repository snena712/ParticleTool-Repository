#pragma once
#include "../../../DX11System/CDirectInput.h"
#include "../../../Component/SceneManager.h"
#include "../../MainGameManager.h"
#include "../Fade/FadeManager.h"
#include "../../../DX11System/mystring.h"
#include "../Fade/FadeComponent.h"
#include "../../Scene/SelectBackUI.h"

namespace Component {

	class ClearManager :public ComponentBase
	{
	private:
		int ControllerNum = -1;
		int m_optionNum;
		const int m_maxoptionNum = 5;

		bool m_inputflg = false;
		bool m_plessInputflg = false;
		bool m_yesflg = false;
		int m_nowinputcnt = 0;
		const int m_maxinputcnt = 20;


		Component::FadeManager* m_fade;
		GameObject* m_backui;

		void InputKeybord();
		void InputController();
		void EndUpdate();
	public:
		ClearManager(GameObject* _obj);
		~ClearManager();

		void Start();
		void Update();
	};

}