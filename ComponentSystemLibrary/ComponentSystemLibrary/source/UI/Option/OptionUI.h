#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../BulletManager.h"


namespace Component {


	class OptionUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;

		Vector2 m_size;

		void PosUpdate();

	public:
		OptionUI(GameObject* _obj);
		~OptionUI();

		void Start();
		void Update();
	};

}