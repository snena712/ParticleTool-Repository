#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../../Component/AnimationComponent.h"
#include "../../../Component/SceneManager.h"
#include "BulletEffect.h"


namespace Component {

	class BulletEffectManager : public ComponentBase
	{

	private:

		std::vector<GameObject*> m_effectList;	// ホーミング配列
		const int m_effectnum = 3;

	public:
		BulletEffectManager(GameObject* _obj);
		~BulletEffectManager();

		void Start();
		void Update();

		void Fire(Vector3 _pos);
	};
}