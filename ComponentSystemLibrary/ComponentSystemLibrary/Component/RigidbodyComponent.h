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

		int ID;							//Rigidbody��ID

		Vector3 ADDForce;				//�^����͂̐ݒ�
		Vector3 Velocity;				//���x
		Vector3 Acceleration;			//�����x
		float Mass;						//���ʁ@�@(ADDForce�̂ݔ��f�BAcceleration�͔��f���܂���)
		bool UseGravity;				//�d�̓t���O
		float GravityPower;				//�d�͌W��

		std::vector<ColliderBase*> Collider;				//�R���C�_�[���X�g

		std::vector<ColliderObject> CollisionObject;			//�Փ˂����I�u�W�F�N�g�ꗗ��ۑ�����vector  Update���ɍX�V�����

		void AddList() { RigidbodyList.emplace_back(owner); AddListFlg = true; }	//RigidbodyList�ւ̒ǉ��@(Rigidbody��ǉ������I�u�W�F�N�g���Ǘ�����vector)
		bool AddListFlg;

	public:
		static std::vector<GameObject*> RigidbodyList;

		Rigidbody(GameObject* Owner) : ComponentBase(Owner), ADDForce(0, 0, 0), Mass(1), UseGravity(false), GravityPower(1.0f), AddListFlg(false), Velocity(0, 0, 0), Acceleration(0, 0, 0) { }
		Rigidbody(GameObject* Owner, Vector3 addForce) : ComponentBase(Owner), ADDForce(addForce), Mass(1), UseGravity(false), GravityPower(1.0f), AddListFlg(false), Velocity(0, 0, 0), Acceleration(0, 0, 0) { }
		Rigidbody(GameObject* Owner, Vector3 addForce, float mass) : ComponentBase(Owner), ADDForce(addForce), Mass(mass), UseGravity(false), GravityPower(1.0f), AddListFlg(false), Velocity(0, 0, 0), Acceleration(0, 0, 0) { }
		~Rigidbody();


		void			SetForce(Vector3 Force) { ADDForce += Force; }				//�͂�������(����)
		Vector3			GetVelocity() { return Velocity; }							//���ݑ��x���擾
		void			SetVelocity(Vector3 velocity) { Velocity = velocity; }		//���x�̐ݒ�
		void			SetVelocityX(float velocity) { Velocity.x = velocity; }		//���x�̐ݒ�( X )
		void			SetVelocityY(float velocity) { Velocity.y = velocity; }		//���x�̐ݒ�( Y )
		void			SetVelocityZ(float velocity) { Velocity.z = velocity; }		//���x�̐ݒ�( Z )

		Vector3			GetAcceleration() { return Acceleration; }					//�����x���擾
		void			AddAcceleration(Vector3 accel) { Acceleration += accel; }	//�����x�̒ǉ�(����)
		void			SetAcceleration(Vector3 accel) { Acceleration = accel; }	//�����x�̐ݒ�

		void			SetAccelerationX(float accel) { Acceleration.x = accel; }	//�����x�̐ݒ�( X )
		void			SetAccelerationY(float accel) { Acceleration.y = accel; }	//�����x�̐ݒ�( Y )
		void			SetAccelerationZ(float accel) { Acceleration.z = accel; }	//�����x�̐ݒ�( Z )


		float			GetMass() { return Mass; }									//���ʂ��擾
		void			SetMass(float mass) { Mass = mass; }						//���ʂ̐ݒ�

		bool			GetGravity() { return UseGravity; }							//�d�̓t���O���擾
		void			SetGravity(bool Gravity) { UseGravity = Gravity; }			//�d�̓t���O��ݒ�

		float			GetGravityPower() { return GravityPower; }					//�d�͂̑傫�����擾
		void			SetGravityPower(float gravity) { GravityPower = gravity; }	//�d�͂̑傫����ݒ�

		auto			GetCollisionObject() { return &CollisionObject; }								//��������Object�̈ꗗ���擾

		auto			GetColliderList() { return &Collider; }												//�g��Ȃ��ł�������
		void			SetColliderList(ColliderBase* collider) { Collider.emplace_back(collider); }		//�g��Ȃ��ł�������	//��������Object�̐ݒ�

		void			StopMove() { Velocity = { 0,0,0 }; Acceleration = { 0,0,0 }; UseGravity = false; }		//����̒�~

		int				GetID() { return ID; }
		void			SetID();

		bool			GetHitCheck();												//�Փ˔���̎擾

		void			PhysicsUpdate();

#ifdef DEBUG
		void			ImGuiDebug();
#endif

	};
}