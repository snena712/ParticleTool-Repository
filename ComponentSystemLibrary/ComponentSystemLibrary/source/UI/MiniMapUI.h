#pragma once
#include "../../Component\GameObject.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/Quad2DMultiRenderComponent.h"
#include "../../DX11System\dx11mathutil.h"
#include "../Enemy/EnemyManager.h"
#include "MiniMap\MiniMapIconUI.h"

namespace Component {

	class MiniMapUI : public ComponentBase
	{
	private:
		// コンポ―ネント
		Transform* tra;
		Component::QuadRender* ren;
		Component::Quad2DRender* ren2d;

		// 変数
		GameObject* m_camera;
		GameObject* player;
		Component::EnemyManager* enemana;

		Vector3 center;			// ミニマップの中心点
		Vector3 m_pos = Vector3(75, -648, 1);

		// リスト
		std::vector< GameObject*> miniiconlist;	// アイコン

		void PosUpdate();		// 場所の更新
		void RoundCheck();		// 角度から位置指定


		bool init = false;
	public:
		MiniMapUI(GameObject* _obj);
		~MiniMapUI();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }// 位置の設定
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }				// qtの設定
	};
}