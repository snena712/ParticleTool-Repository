#pragma once

#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"

namespace Component {
	class BoxCollider :public ColliderBase {

	private:
		Vector3 Size;		//�{��
		Vector3 Scale;		//�傫��

		bool Type2D;		//�QD�t���O�@// true�ɂ����Box�����̓����蔻�肪�QD��Ŕ��肳��܂�

		void AddColliderList();

	public:
		BoxCollider(GameObject* Owner) :ColliderBase(Owner), Size(1.0f, 1.0f, 1.0f), Scale(-1.0f, -1.0f, 1.0f), Type2D(false) {}
		~BoxCollider() {}


		Vector3		GetSize() { return Size; }					//�{���̎擾
		void		SetSize(Vector3 size) { Size = size; }		//�{����ݒ�

		bool		Get2DFlg() { return Type2D; }				//�QD�t���O�̎擾
		void		Set2Dflg(bool flg) { Type2D = flg; }		//�QD �t���O�̐ݒ�

		Vector3		GetScale() { return Scale; }				//�傫���̎擾
		void		SetScale(Vector3 scale) { Scale = scale; }	//�傫����ݒ�

		void		Update();
		void		PhysicsUpdate();
		void		Draw() {}

#ifdef DEBUG
		void		ImGuiDebug();
#endif

		std::string GetType() { return "Box"; }
	};
}