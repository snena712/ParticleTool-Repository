#pragma once
#include "../../Component/ModelComponent.h"

namespace Component {
	class  PlayerJatParts : public ComponentBase
	{
	private:

		GameObject* player;

		Transform* tra;
		Component::ModelRender* model;

		int m_rot;				// 現在の保持角度
		int m_maxrot = 10;		// 最大保持角度
		int m_rotreturn = 1;	// 戻る角度幅
		bool m_rotflg = false;

		void PosUpdate();
		void RotUpdate();		// 角度の更新
	public:
		PlayerJatParts(GameObject* _obj);
		~PlayerJatParts();

		void Start();
		void Update();

		void SetRot(int _num);	// 角度の設定

	};
}

