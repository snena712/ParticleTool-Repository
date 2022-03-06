#pragma once
#include "../../DX11System/DX11util.h"
#include  "../../Application.h"
#include <string>
#include "../../DX11System\shaderhashmap.h"
#include "../../DX11System\quad.h"
#include "../../Component\GameObject.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/PostEffectComponent.h"

namespace Component {

	class Shimmer :public ComponentBase
	{
	private:
		Component::PostEffectComponent* post;
		GameObject* player;
		std::vector<Vector3>poslist;		// •`‰æˆÊ’u

		void State();
	public:
		Shimmer(GameObject* _obj);
		~Shimmer();

		void Start();
		void DrawObject();
		void Update();
	};
}