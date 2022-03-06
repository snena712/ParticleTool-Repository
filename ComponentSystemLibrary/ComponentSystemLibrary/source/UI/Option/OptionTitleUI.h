#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../UI/Fade/FadeManager.h"
namespace Component {

	class OptionTitleUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Vector2 m_size;

		void PosUpdate();
		void SceneUpdate();
	public:
		OptionTitleUI(GameObject* _obj);
		~OptionTitleUI();

		void Start();
		void Update();
	};

}