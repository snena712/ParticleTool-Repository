#pragma once
#include "../../../Component/GameObject.h"
#include "../../../Component/QuadRenderComponent.h"
#include "../../../DX11System\dx11mathutil.h"
#include "../../Enemy/EnemyManager.h"

// ミニマップの後ろの物
namespace Component {
	class  MiniMapBackUI :public ComponentBase
	{
	private:

		GameObject* m_camera;
		Component::EnemyManager* m_enemane;

		Transform* tra;
		Component::QuadRender* ren;


		XMFLOAT4 m_qt;
		float rot = 0;


		void PosUpdate();
	
	public:
		MiniMapBackUI(GameObject* _obj);
		~MiniMapBackUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

	};
}
