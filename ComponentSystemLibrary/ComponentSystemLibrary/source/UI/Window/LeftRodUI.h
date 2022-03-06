#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../StartUIColorComponent.h"

namespace Component {

	class LeftRodUI : public ComponentBase
	{
	private:

		Component::Quad2DRender* ren;

	public:
		LeftRodUI(GameObject* _obj);
		~LeftRodUI();

	};

}