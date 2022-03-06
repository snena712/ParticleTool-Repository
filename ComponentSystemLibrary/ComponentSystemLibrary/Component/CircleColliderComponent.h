#pragma once

#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"

//�ݒ肷�锼�a�̎擾��
enum class CircleType {
	X,
	Y,
	Auto
};


namespace Component {
	class CircleCollider :public ColliderBase {

	private:

		float Size;							//�{��
		CircleType Type;					//���a�ݒ��Type
		float Radius;						//���a

		void GetRadiusSize();
		void AddColliderList();
	public:
		CircleCollider(GameObject* Owner) :ColliderBase(Owner), Type(CircleType::Auto), Radius(-1.0f), Size(1.0f) {  }
		~CircleCollider() {}

		float		GetRadius() { return Radius; }						//���a�̎擾
		void		SetRadius(float radius) { Radius = radius; }		//���a��ݒ�

		float		GetSize() { return Size; }							//�{���̎擾
		void		SetSize(float size) { Size = size; }				//�{����ݒ�

		auto		GetCircleType() { return Type; }										//�����蔻��^�C�v�̎擾
		void		SetCircleType(CircleType type) { Type = type; GetRadiusSize(); }		//�����蔻��^�C�v�̐ݒ�

		void Update();
		void PhysicsUpdate();

#ifdef DEBUG
		void ImGuiDebug();
#endif

		std::string GetType() { return "Circle"; }
	};
}
