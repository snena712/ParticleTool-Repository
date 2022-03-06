#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "../line.h"

namespace Component {

	class TileLine : public ComponentBase
	{
	public:
		// �������
		enum class Status
		{
			LIVE,
			DEAD
		};

	private:
		Component::QuadRender* ren;
		Transform* targetTra;

	public:
		TileLine(GameObject* _obj);
		~TileLine();
		void Start();
		void Update();
		void DrawObject();
	};
}