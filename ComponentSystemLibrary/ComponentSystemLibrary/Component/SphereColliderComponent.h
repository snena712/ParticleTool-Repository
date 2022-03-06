#pragma once

#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"

//İ’è‚·‚é”¼Œa‚Ìæ“¾æ
//İ’è‚·‚é”¼Œa‚Ìæ“¾æ
enum class SphereType {
	X,
	Y,
	Z,
	Auto
};


namespace Component {
	class SphereCollider :public ColliderBase {

	private:

		float Size;							//”{—¦
		SphereType Type;					//”¼Œaİ’è‚ÌType
		float Radius;						//”¼Œa

		void GetRadiusSize();
		void AddColliderList();
	public:
		SphereCollider(GameObject* Owner) :ColliderBase(Owner), Type(SphereType::Auto), Radius(-1.0f), Size(1.0f) {  }
		~SphereCollider() {}

		float		GetRadius() { return Radius; }						//”¼Œa‚Ìæ“¾
		void		SetRadius(float radius) { Radius = radius; }		//”¼Œa‚ğİ’è

		float		GetSize() { return Size; }							//”{—¦‚Ìæ“¾
		void		SetSize(float size) { Size = size; }				//”{—¦‚ğİ’è

		auto		GetCircleType() { return Type; }										//“–‚½‚è”»’èƒ^ƒCƒv‚Ìæ“¾
		void		SetCircleType(SphereType type) { Type = type; GetRadiusSize(); }		//“–‚½‚è”»’èƒ^ƒCƒv‚Ìİ’è

		void Update();
		void PhysicsUpdate();
		void Draw() {}

#ifdef DEBUG
		void ImGuiDebug();
#endif

		std::string GetType() { return "Sphere"; }
	};
}
