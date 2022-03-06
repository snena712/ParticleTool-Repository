#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SceneManager.h"

namespace Component {
	class  SelectUnderbarUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;
		Component::QuadRender* ren;

		Vector3 m_pos;

		void PosUpdate();
	public:
		SelectUnderbarUI(GameObject* _obj);
		~SelectUnderbarUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void SetPos(Vector3 _pos) { m_pos = _pos; }
	};
}
