#pragma once
#include "../../Component/Quad2DRenderComponent.h"
#include "../StartUIColorComponent.h"
#include "../BulletManager.h"


namespace Component {


	class WindowUI :public ComponentBase
	{
	private:

		GameObject* m_camera;
		GameObject* m_bullet;

		Transform* tra;
		Component::Quad2DRender* ren;

		Vector2 m_size;

		bool m_hitflg = false;

		void HitUpdate();

	public:
		WindowUI(GameObject* _obj);
		~WindowUI();

		void Start();
		void Update();

		void SetHitFlg(bool _flg) { m_hitflg = _flg; }
		bool GetHitFlg() { return m_hitflg; }
	};

}