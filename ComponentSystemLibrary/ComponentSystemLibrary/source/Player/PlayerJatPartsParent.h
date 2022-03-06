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

		//----�W�F�b�g�̃��X�g
		std::vector<Component::PlayerJatParts> m_jetlist;	// �W�F�b�g���X�g
		int m_jetnum = 4;

		int m_rot;				// ���݂̕ێ��p�x
		int m_maxrot = 10;		// �ő�ێ��p�x
		int m_rotreturn = 1;	// �߂�p�x��
		bool m_rotflg = false;

		void RotUpdate();		// �p�x�̍X�V
	public:
		PlayerJatPartsParent(GameObject* _obj);
		~PlayerJatPartsParent();

		void Start();
		void Update();

		void SetInitialPosition(XMFLOAT3 _pos) { owner->transform->position = _pos; }
		void SetDirection(XMFLOAT4 _qt) { owner->transform->qt = _qt; }

		void SetRot(int _num);	// �p�x�̐ݒ�
	};
}


