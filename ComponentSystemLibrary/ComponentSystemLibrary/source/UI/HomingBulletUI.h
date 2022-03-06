#pragma once
#include "../../Component/Quad2DRenderComponent.h"
#include "../StartUIColorComponent.h"

namespace Component {
	class HomingBulletUI :public ComponentBase
	{
	private:

		Transform* tra;
		Component::Quad2DRender* ren;

	public:
		HomingBulletUI(GameObject* _obj);
		~HomingBulletUI();

		void Start();


		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};

}