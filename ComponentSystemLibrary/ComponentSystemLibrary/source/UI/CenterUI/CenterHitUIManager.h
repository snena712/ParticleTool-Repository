#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "CenterHitUI.h"
namespace Component {
	class  CenterHitUIManager :public ComponentBase
	{
	public:
		enum class State {
			UP_RIGHT,
			UP_LEFT,
			DOWN_RIGHT,
			DOWN_LEFT,
		};
	private:


		Component::QuadRender* ren;

		std::vector<GameObject*>m_centerList;

	public:
		CenterHitUIManager(GameObject* _obj);
		~CenterHitUIManager();

		void Start();
		void Update();

	};
}
