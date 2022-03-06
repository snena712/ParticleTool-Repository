#pragma once
#include "../../Component/ModelComponent.h"
#include "PlayerWing.h"

namespace Component {

	class  PlayerWingParent :public ComponentBase
	{
	private:
		GameObject* player;

		int m_rot;				// ���݂̕ێ��p�x
		int m_maxrot = 15;		// �ő�ێ��p�x
		int m_rotreturn = 3;	// �߂�p�x��
		bool m_rotflg = false;

		void RotUpdate();		// �p�x�̍X�V
	public:
		PlayerWingParent(GameObject* _obj);
		~PlayerWingParent();

		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void SetRot(int _num);	// �p�x�̐ݒ�
	};
}


