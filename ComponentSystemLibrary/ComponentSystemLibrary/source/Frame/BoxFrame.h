#pragma once
#include "../../Component/ModelComponent.h"

namespace Component {
	class BoxFrame :public ComponentBase
	{
	private:
		//--コンポーネント
		Component::ModelRender* model;

	public:
		BoxFrame(GameObject* _obj);
		~BoxFrame();

		void DrawObject();
		void Update();

	};
}