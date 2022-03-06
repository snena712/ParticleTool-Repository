#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../MainGameManager.h"
#include "../../StartUIColorComponent.h"

namespace Component {
	class  SpeedUI : public ComponentBase
	{
	private:
		Transform* tra;
		Component::Quad2DRender* ren;

		XMFLOAT4 m_color;

		void PosUpdate();
		void CheckStart();
	public:
		SpeedUI(GameObject* _obj);
		~SpeedUI();

		void Start();
		void Update();

	};
}

