#pragma once
#include "../../../Component/QuadRenderComponent.h"

namespace Component {


	class OptionSettingUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;

		Vector2 m_size;

		void PosUpdate();

	public:
		OptionSettingUI(GameObject* _obj);
		~OptionSettingUI();

		void Start();
		void Update();
	};

}