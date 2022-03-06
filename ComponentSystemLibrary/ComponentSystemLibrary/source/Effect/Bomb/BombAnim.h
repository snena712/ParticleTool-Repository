#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/AnimationComponent.h"

namespace Component {
	class  BombAnim :public ComponentBase
	{
	private:

		Component::Animation* anim;
		Component::QuadRender* ren;

	public:
		BombAnim(GameObject* _obj);
		~BombAnim();

		void Update();

	};
}
