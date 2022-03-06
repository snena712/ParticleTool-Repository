#pragma once
#include "../../../Component/GameObject.h"
#include "../../../Component/QuadRenderComponent.h"
#include "../../../DX11System\dx11mathutil.h"
#include "../../Enemy/EnemyManager.h"

namespace Component {

	class  EnemyCountUI :public ComponentBase
	{
	private:

		GameObject* m_camera;
		Component::EnemyManager* m_enemane;

		Transform* tra;
		Component::QuadRender* ren;

		void PosUpdate();
		void String();
	public:
		EnemyCountUI(GameObject* _obj);
		~EnemyCountUI();

		void Start();
		void Update();
		void DrawObject();

	};
}
