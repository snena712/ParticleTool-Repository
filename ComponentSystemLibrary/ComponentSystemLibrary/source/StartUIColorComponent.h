#pragma once
#include "../Component/QuadRenderComponent.h"
#include "../Component/Quad2DRenderComponent.h"
#include "MainGameManager.h"

namespace Component {
	class  StartUIColorComponent :public ComponentBase
	{
		public:
			enum class RnderState
			{
				NORMAL,
				TWO
			};
	private:
		Component::QuadRender* ren;
	Component::Quad2DRender *ren2d;

		XMFLOAT4 m_color;


		void CheckStart();
	public:
		StartUIColorComponent(GameObject* _obj);
		~StartUIColorComponent();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void SetType(RnderState _state);

	};
}


