#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../UI/Fade/FadeManager.h"

namespace Component {


	class OptionRetryUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Vector2 m_size;

		void PosUpdate();
		void SceneUpdate();
	public:
		OptionRetryUI(GameObject* _obj);
		~OptionRetryUI();

		void Start();
		void Update();
	};

}