#pragma once
#include "../../../Component/GameObject.h"
#include "../../../Component/QuadRenderComponent.h"
#include "../../../DX11System\dx11mathutil.h"
#include "../..//BulletManager.h"

namespace Component {
	class  WindowBullet : public ComponentBase
	{
	private:

		GameObject* m_camera;
		BulletManager* m_bulletmana;

		Transform* tra;
		Component::QuadRender* ren;

		void PosUpdate();
	public:
		WindowBullet(GameObject* _obj);
		~WindowBullet();

		void Start();
		void Update();
		void DrawObject();

		void DrawString();		// •¶Žš‚Ì•`‰æ

	};
}

