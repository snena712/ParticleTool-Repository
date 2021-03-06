#pragma once

#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"

//Ýè·é¼aÌæ¾æ
//Ýè·é¼aÌæ¾æ
enum class SphereType {
	X,
	Y,
	Z,
	Auto
};


namespace Component {
	class SphereCollider :public ColliderBase {

	private:

		float Size;							//{¦
		SphereType Type;					//¼aÝèÌType
		float Radius;						//¼a

		void GetRadiusSize();
		void AddColliderList();
	public:
		SphereCollider(GameObject* Owner) :ColliderBase(Owner), Type(SphereType::Auto), Radius(-1.0f), Size(1.0f) {  }
		~SphereCollider() {}

		float		GetRadius() { return Radius; }						//¼aÌæ¾
		void		SetRadius(float radius) { Radius = radius; }		//¼aðÝè

		float		GetSize() { return Size; }							//{¦Ìæ¾
		void		SetSize(float size) { Size = size; }				//{¦ðÝè

		auto		GetCircleType() { return Type; }										//½è»è^CvÌæ¾
		void		SetCircleType(SphereType type) { Type = type; GetRadiusSize(); }		//½è»è^CvÌÝè

		void Update();
		void PhysicsUpdate();
		void Draw() {}

#ifdef DEBUG
		void ImGuiDebug();
#endif

		std::string GetType() { return "Sphere"; }
	};
}
