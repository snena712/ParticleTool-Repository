#pragma once
#include "../../Component/GameObject.h"
#include "../../Component/ModelRenderInstance.h"

namespace Component {
	class SphereModel :public ComponentBase
	{
	public:
		SphereModel(GameObject* _obj);
		~SphereModel();

	private:
		Component::ModelRenderInstance* model;

	};
}

//-----------------------------
// create by Suehara
//-----------------------------