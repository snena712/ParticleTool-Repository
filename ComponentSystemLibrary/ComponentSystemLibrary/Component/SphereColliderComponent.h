#pragma once

#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"

//�ݒ肷�锼�a�̎擾��
//�ݒ肷�锼�a�̎擾��
enum class SphereType {
	X,
	Y,
	Z,
	Auto
};


namespace Component {
	class SphereCollider :public ColliderBase {

	private:

		float Size;							//�{��
		SphereType Type;					//���a�ݒ��Type
		float Radius;						//���a

		void GetRadiusSize();
		void AddColliderList();
	public:
		SphereCollider(GameObject* Owner) :ColliderBase(Owner), Type(SphereType::Auto), Radius(-1.0f), Size(1.0f) {  }
		~SphereCollider() {}

		float		GetRadius() { return Radius; }						//���a�̎擾
		void		SetRadius(float radius) { Radius = radius; }		//���a��ݒ�

		float		GetSize() { return Size; }							//�{���̎擾
		void		SetSize(float size) { Size = size; }				//�{����ݒ�

		auto		GetCircleType() { return Type; }										//�����蔻��^�C�v�̎擾
		void		SetCircleType(SphereType type) { Type = type; GetRadiusSize(); }		//�����蔻��^�C�v�̐ݒ�

		void Update();
		void PhysicsUpdate();
		void Draw() {}

#ifdef DEBUG
		void ImGuiDebug();
#endif

		std::string GetType() { return "Sphere"; }
	};
}
