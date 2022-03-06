#pragma once
#include "../../../../Component/QuadRenderComponent.h"
#include "../../../../Component/SceneManager.h"

namespace Component {
	class  HitUI :public ComponentBase
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
		HitUI(GameObject* _obj);
		~HitUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void ResetCnt() { m_nowinputcnt = 0; owner->transform->position.x = 10000; }
	};
}
