#pragma once
#include "../../../../Component/SceneManager.h"
#include "../../../../DX11System/mystring.h"
#include "../../../../DX11System/Time.h"
#include "../../../MainGameManager.h"

namespace Component {
	class  TimerUI :public ComponentBase
	{
	private:

		float m_starttime = 0;		// ƒQ[ƒ€ŠJŽnŽžŽžŠÔ
		float m_limitTime = 5;
		float m_stocktime = 0;
		float m_optionstock=NULL;

		float _time = 0;

		bool m_endflg = false;

		void PosUpdate();
		void StartTimer();
		void String();

		void ReduceTime();
		void LimitCheck();
	public:
		TimerUI(GameObject* _obj);
		~TimerUI();

		void Start();
		void Update();
		void DrawObject();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void SetTimer(float _timer) { m_limitTime = _timer; }
	};
}
