#pragma once
#include "../../../../Component/QuadRenderComponent.h"
#include "../../../../Component/SceneManager.h"

namespace Component {
	class  DestroyUI :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;
		Component::QuadRender* ren;

		int m_nowinputcnt = 0;
		const int m_maxinputcnt = 20;

		void PosUpdate();
		void StatusUpdate();
	public:
		DestroyUI(GameObject* _obj);
		~DestroyUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void ResetCnt() { m_nowinputcnt = 0; }
	};
}
