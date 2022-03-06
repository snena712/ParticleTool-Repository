#pragma once

#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"

namespace Component {
	class BoxCollider :public ColliderBase {

	private:
		Vector3 Size;		//”{—¦
		Vector3 Scale;		//‘å‚«‚³

		bool Type2D;		//‚QDƒtƒ‰ƒO@// true‚É‚·‚é‚ÆBox“¯‚Ì“–‚½‚è”»’è‚ª‚QDã‚Å”»’è‚³‚ê‚Ü‚·

		void AddColliderList();

	public:
		BoxCollider(GameObject* Owner) :ColliderBase(Owner), Size(1.0f, 1.0f, 1.0f), Scale(-1.0f, -1.0f, 1.0f), Type2D(false) {}
		~BoxCollider() {}


		Vector3		GetSize() { return Size; }					//”{—¦‚Ìæ“¾
		void		SetSize(Vector3 size) { Size = size; }		//”{—¦‚ğİ’è

		bool		Get2DFlg() { return Type2D; }				//‚QDƒtƒ‰ƒO‚Ìæ“¾
		void		Set2Dflg(bool flg) { Type2D = flg; }		//‚QD ƒtƒ‰ƒO‚Ìİ’è

		Vector3		GetScale() { return Scale; }				//‘å‚«‚³‚Ìæ“¾
		void		SetScale(Vector3 scale) { Scale = scale; }	//‘å‚«‚³‚ğİ’è

		void		Update();
		void		PhysicsUpdate();
		void		Draw() {}

#ifdef DEBUG
		void		ImGuiDebug();
#endif

		std::string GetType() { return "Box"; }
	};
}