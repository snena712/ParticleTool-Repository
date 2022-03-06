#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SceneManager.h"
#include "../UI/Fade/FadeComponent.h"
namespace Component {
	class  SelectBackUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;
		Component::QuadRender* ren;

		void PosUpdate();
	public:
		SelectBackUI(GameObject* _obj);
		~SelectBackUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};
}
