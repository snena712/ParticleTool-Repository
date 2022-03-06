#pragma once
#include "../../../Component/QuadRenderComponent.h"

namespace Component {


	class OptionEndUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;
		Component::QuadRender* ren;

		Vector2 m_size;

		void PosUpdate();



	public:
		OptionEndUI(GameObject* _obj);
		~OptionEndUI();

		void Start();
		void Update();
	};

}