#pragma once
#include "../../../../Component/QuadRenderComponent.h"
#include "../../../../Component/SceneManager.h"
#include "../../../../GameObjects/MainCamera.h"

namespace Component {
	class  EnemyRoot :public ComponentBase
	{
	private:

		GameObject* m_camera;

		Transform* tra;
		Component::QuadRender* ren;

		void PosUpdate();
	public:
		EnemyRoot(GameObject* _obj);
		~EnemyRoot();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};
}
