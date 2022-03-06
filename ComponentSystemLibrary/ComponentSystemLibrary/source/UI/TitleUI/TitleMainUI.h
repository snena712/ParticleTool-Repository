#pragma once
#include "../../../Component/GameObject.h"
#include "../../../Component/QuadRenderComponent.h"
#include "../../../DX11System\dx11mathutil.h"
#include "../../Title/TitlePlayer.h"

namespace Component {

	class  TitleMainUI :public ComponentBase
	{
	private:

		GameObject* m_camera;
		TitlePlayer* player;

		Transform* tra;
		Component::QuadRender* ren;


		XMFLOAT4 m_qt;

		void PosUpdate();

	public:
		TitleMainUI(GameObject* _obj);
		~TitleMainUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void ButtonUpdate();
	};
}

