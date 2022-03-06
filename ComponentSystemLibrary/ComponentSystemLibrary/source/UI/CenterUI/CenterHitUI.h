#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/SceneManager.h"

namespace Component {
	class  CenterHitUI :public ComponentBase
	{
	public:
		enum class State {
			UP_RIGHT,
			UP_LEFT,
			DOWN_RIGHT,
			DOWN_LEFT,
		};
	private:

		GameObject* m_camera;

		Component::QuadRender* ren;

		int m_nowinputcnt = 0;
		const int m_maxinputcnt = 6;
		State m_state;
		Vector2 m_plus;
		Vector3 m_base;
		

		void PosUpdate();
		bool StatusUpdate();
	public:
		CenterHitUI(GameObject* _obj);
		~CenterHitUI();

		void Start();
		void Update();
	
		void SetState(State _state);
	};
}
