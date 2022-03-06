#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/AnimationComponent.h"

namespace Component {
	class  BulletEffect :public ComponentBase
	{

	private:

		Component::Animation* anim;
		Component::QuadRender* ren;

	public:
		BulletEffect(GameObject* _obj);
		~BulletEffect();

		void Update();

	};
}
