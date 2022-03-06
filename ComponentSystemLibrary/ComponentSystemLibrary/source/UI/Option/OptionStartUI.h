#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../MainGameManager.h"
namespace Component {


	class OptionStartUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;

		Vector2 m_size;

		void PosUpdate();

	public:
		OptionStartUI(GameObject* _obj);
		~OptionStartUI();

		void Start();
		void Update();
	};

}