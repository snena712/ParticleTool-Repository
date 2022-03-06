#pragma once
#include "../../Component/ModelComponent.h"
#include "PlayerWing.h"

namespace Component {

	class  PlayerWingParent :public ComponentBase
	{
	private:
		GameObject* player;

		int m_rot;				// 現在の保持角度
		int m_maxrot = 15;		// 最大保持角度
		int m_rotreturn = 3;	// 戻る角度幅
		bool m_rotflg = false;

		void RotUpdate();		// 角度の更新
	public:
		PlayerWingParent(GameObject* _obj);
		~PlayerWingParent();

		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void SetRot(int _num);	// 角度の設定
	};
}


