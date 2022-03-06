#pragma once

#include "ComponentBaseClass.h"
#include "RigidbodyComponent.h"
#include "../DX11System/Vector.h"

namespace Component {
	class ColliderBase :public ComponentBase {

	protected:
		bool HitCheck;						//当たり判定

		Vector3 Center;						//中心点のずれ

		bool Passive;						//動的に当たり判定を更新するかのフラグ

		bool AddListFlg;
	public:
		ColliderBase(GameObject* Owner) :ComponentBase(Owner), Center(0.0f, 0.0f, 0.0f), Passive(false), AddListFlg(false) {}
		virtual ~ColliderBase() {}

		Vector3		GetCenter() { return Center; }						//中心点のずれの取得
		void		SetCenter(Vector3 center) { Center = center; }		//中心点のずれを設定

		bool		GetHitCheck() { return HitCheck; }					//ヒットフラグを取得
		void		SetHitCheck(bool check) { HitCheck = check; }

		bool		GetPassive() { return Passive; }					// パッシブフラグの取得
		void		SetPassive(bool passive) { Passive = passive; }		// パッシブフラグの設定

		virtual void Draw() {}
		virtual void Update() {}
		virtual void PhysicsUpdate() {}

#ifdef DEBUG
		virtual void ImGuiDebug() {}
#endif

		std::string virtual GetType() { return "Base"; }
	};
}