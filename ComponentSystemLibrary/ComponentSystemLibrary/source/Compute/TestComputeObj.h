#pragma once
#include "../../Component/GameObject.h"
#include "../../Component/ComputeComponent.h"
#include "../../DX11System\CDirectxGraphics.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../DX11System/RandomClass.h"

namespace Component {
	class TestComputeObj :public ComponentBase
	{
	private:
		//--コンポーネント
		Component::ComputeComponent* comCompo;
		Component::QuadRender* ren;

	public:
		TestComputeObj(GameObject* _obj);
		~TestComputeObj();

		void DrawObject();
		void Update();
		//void Start();


	};
}