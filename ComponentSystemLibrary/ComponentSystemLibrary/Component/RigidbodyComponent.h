#pragma once

#include "ComponentBaseClass.h"
#include "../DX11System/Vector.h"
#include "ColliderBase.h"


enum ColliderDirection {
	Down = 0,
	Up,
	Right,
	Left,
	front,
	back,

	enumEnd = 15,
};


struct ColliderObject {
	GameObject* obj;
	ColliderDirection dir;
};


namespace Component {
	class Rigidbody : public ComponentBase {

	private:

		int ID;							//RigidbodyのID

		Vector3 ADDForce;				//与える力の設定
		Vector3 Velocity;				//速度
		Vector3 Acceleration;			//加速度
		float Mass;						//質量　　(ADDForceのみ反映。Accelerationは反映しません)
		bool UseGravity;				//重力フラグ
		float GravityPower;				//重力係数

		std::vector<ColliderBase*> Collider;				//コライダーリスト

		std::vector<ColliderObject> CollisionObject;			//衝突したオブジェクト一覧を保存するvector  Update毎に更新される

		void AddList() { RigidbodyList.emplace_back(owner); AddListFlg = true; }	//RigidbodyListへの追加　(Rigidbodyを追加したオブジェクトを管理するvector)
		bool AddListFlg;

	public:
		static std::vector<GameObject*> RigidbodyList;

		Rigidbody(GameObject* Owner) : ComponentBase(Owner), ADDForce(0, 0, 0), Mass(1), UseGravity(false), GravityPower(1.0f), AddListFlg(false), Velocity(0, 0, 0), Acceleration(0, 0, 0) { }
		Rigidbody(GameObject* Owner, Vector3 addForce) : ComponentBase(Owner), ADDForce(addForce), Mass(1), UseGravity(false), GravityPower(1.0f), AddListFlg(false), Velocity(0, 0, 0), Acceleration(0, 0, 0) { }
		Rigidbody(GameObject* Owner, Vector3 addForce, float mass) : ComponentBase(Owner), ADDForce(addForce), Mass(mass), UseGravity(false), GravityPower(1.0f), AddListFlg(false), Velocity(0, 0, 0), Acceleration(0, 0, 0) { }
		~Rigidbody();


		void			SetForce(Vector3 Force) { ADDForce += Force; }				//力を加える(等速)
		Vector3			GetVelocity() { return Velocity; }							//現在速度を取得
		void			SetVelocity(Vector3 velocity) { Velocity = velocity; }		//速度の設定
		void			SetVelocityX(float velocity) { Velocity.x = velocity; }		//速度の設定( X )
		void			SetVelocityY(float velocity) { Velocity.y = velocity; }		//速度の設定( Y )
		void			SetVelocityZ(float velocity) { Velocity.z = velocity; }		//速度の設定( Z )

		Vector3			GetAcceleration() { return Acceleration; }					//加速度を取得
		void			AddAcceleration(Vector3 accel) { Acceleration += accel; }	//加速度の追加(加速)
		void			SetAcceleration(Vector3 accel) { Acceleration = accel; }	//加速度の設定

		void			SetAccelerationX(float accel) { Acceleration.x = accel; }	//加速度の設定( X )
		void			SetAccelerationY(float accel) { Acceleration.y = accel; }	//加速度の設定( Y )
		void			SetAccelerationZ(float accel) { Acceleration.z = accel; }	//加速度の設定( Z )


		float			GetMass() { return Mass; }									//質量を取得
		void			SetMass(float mass) { Mass = mass; }						//質量の設定

		bool			GetGravity() { return UseGravity; }							//重力フラグを取得
		void			SetGravity(bool Gravity) { UseGravity = Gravity; }			//重力フラグを設定

		float			GetGravityPower() { return GravityPower; }					//重力の大きさを取得
		void			SetGravityPower(float gravity) { GravityPower = gravity; }	//重力の大きさを設定

		auto			GetCollisionObject() { return &CollisionObject; }								//当たったObjectの一覧を取得

		auto			GetColliderList() { return &Collider; }												//使わないでくだちい
		void			SetColliderList(ColliderBase* collider) { Collider.emplace_back(collider); }		//使わないでくだちい	//当たったObjectの設定

		void			StopMove() { Velocity = { 0,0,0 }; Acceleration = { 0,0,0 }; UseGravity = false; }		//動作の停止

		int				GetID() { return ID; }
		void			SetID();

		bool			GetHitCheck();												//衝突判定の取得

		void			PhysicsUpdate();

#ifdef DEBUG
		void			ImGuiDebug();
#endif

	};
}