#pragma once
#include "../../Component/ModelComponent.h"

namespace Component {
	class  PlayerJatParts : public ComponentBase
	{
	private:

		GameObject* player;

		Transform* tra;
		Component::ModelRender* model;

		int m_rot;				// ���݂̕ێ��p�x
		int m_maxrot = 10;		// �ő�ێ��p�x
		int m_rotreturn = 1;	// �߂�p�x��
		bool m_rotflg = false;

		void PosUpdate();
		void RotUpdate();		// �p�x�̍X�V
	public:
		PlayerJatParts(GameObject* _obj);
		~PlayerJatParts();

		void Start();
		void Update();

		void SetRot(int _num);	// �p�x�̐ݒ�

	};
}

