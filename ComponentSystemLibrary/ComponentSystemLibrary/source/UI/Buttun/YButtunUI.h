#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/SceneManager.h"

namespace Component {
	class  YButtunUI :public ComponentBase
	{
	private:

		GameObject* m_camera;
		
		Transform* tra;
		Component::QuadRender* ren;

		void PosUpdate();
	public:
		YButtunUI(GameObject* _obj);
		~YButtunUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};
}
