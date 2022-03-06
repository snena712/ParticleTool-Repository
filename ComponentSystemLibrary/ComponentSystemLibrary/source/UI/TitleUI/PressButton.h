#pragma once
#include "../../../Component/GameObject.h"
#include "../../../Component/QuadRenderComponent.h"
#include "../../../DX11System\dx11mathutil.h"
#include "../../Title/TitlePlayer.h"

namespace Component {

	class  PressButton :public ComponentBase
	{
	private:

		GameObject* m_camera;
		Component::TitlePlayer* player;

		Transform* tra;
		Component::QuadRender* ren;

		void PosUpdate();

		int m_cnt = 0;

	public:
		PressButton(GameObject* _obj);
		~PressButton();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void ButtonUpdate();
	};
}


