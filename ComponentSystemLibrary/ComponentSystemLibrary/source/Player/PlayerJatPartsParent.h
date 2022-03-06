#pragma once
#include "../../Component/ModelComponent.h"
#include "../Player/PlayerJatParts.h"

namespace Component {

	class  PlayerJatPartsParent :public ComponentBase
	{
	private:
		Component::ModelRender* model;

		Transform* tra;
		GameObject* player;

		//----ジェットのリスト
		std::vector<Component::PlayerJatParts> m_jetlist;	// ジェットリスト
		int m_jetnum = 4;

		int m_rot;				// 現在の保持角度
		int m_maxrot = 10;		// 最大保持角度
		int m_rotreturn = 1;	// 戻る角度幅
		bool m_rotflg = false;

		void RotUpdate();		// 角度の更新
	public:
		PlayerJatPartsParent(GameObject* _obj);
		~PlayerJatPartsParent();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void SetRot(int _num);	// 角度の設定
	};
}


