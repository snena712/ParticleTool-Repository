#pragma once
#include "../../../Component/Quad2DRenderComponent.h"
#include "../../../Component/SceneManager.h"
#include "../../StartUIColorComponent.h"

namespace Component {

	class RightRodUI : public ComponentBase
	{
	private:

		GameObject* m_camera;
		Component::Quad2DRender* ren;

	public:
		RightRodUI(GameObject* _obj);
		~RightRodUI();

		void Start();
		void Update();
	};
}
