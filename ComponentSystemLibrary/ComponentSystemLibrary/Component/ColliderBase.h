#pragma once

#include "ComponentBaseClass.h"
#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"

namespace Component {
	class ColliderBase :public ComponentBase {

	protected:
		bool HitCheck;						//�����蔻��

		Vector3 Center;						//���S�_�̂���

		bool Passive;						//���I�ɓ����蔻����X�V���邩�̃t���O

		bool AddListFlg;
	public:
		ColliderBase(GameObject* Owner) :ComponentBase(Owner), Center(0.0f, 0.0f, 0.0f), Passive(false), AddListFlg(false) {}
		virtual ~ColliderBase() {}

		Vector3		GetCenter() { return Center; }						//���S�_�̂���̎擾
		void		SetCenter(Vector3 center) { Center = center; }		//���S�_�̂����ݒ�

		bool		GetHitCheck() { return HitCheck; }					//�q�b�g�t���O���擾
		void		SetHitCheck(bool check) { HitCheck = check; }

		bool		GetPassive() { return Passive; }					// �p�b�V�u�t���O�̎擾
		void		SetPassive(bool passive) { Passive = passive; }		// �p�b�V�u�t���O�̐ݒ�

		virtual void Draw() {}
		virtual void Update() {}
		virtual void PhysicsUpdate() {}

#ifdef DEBUG
		virtual void ImGuiDebug() {}
#endif

		std::string virtual GetType() { return "Base"; }
	};
}