#pragma once

#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"

//İ’è‚·‚é”¼Œa‚Ìæ“¾æ
enum class CircleType {
	X,
	Y,
	Auto
};


namespace Component {
	class CircleCollider :public ColliderBase {

	private:

		float Size;							//”{—¦
		CircleType Type;					//”¼Œaİ’è‚ÌType
		float Radius;						//”¼Œa

		void GetRadiusSize();
		void AddColliderList();
	public:
		CircleCollider(GameObject* Owner) :ColliderBase(Owner), Type(CircleType::Auto), Radius(-1.0f), Size(1.0f) {  }
		~CircleCollider() {}

		float		GetRadius() { return Radius; }						//”¼Œa‚Ìæ“¾
		void		SetRadius(float radius) { Radius = radius; }		//”¼Œa‚ğİ’è

		float		GetSize() { return Size; }							//”{—¦‚Ìæ“¾
		void		SetSize(float size) { Size = size; }				//”{—¦‚ğİ’è

		auto		GetCircleType() { return Type; }										//“–‚½‚è”»’èƒ^ƒCƒv‚Ìæ“¾
		void		SetCircleType(CircleType type) { Type = type; GetRadiusSize(); }		//“–‚½‚è”»’èƒ^ƒCƒv‚Ìİ’è

		void Update();
		void PhysicsUpdate();

#ifdef DEBUG
		void ImGuiDebug();
#endif

		std::string GetType() { return "Circle"; }
	};
}
