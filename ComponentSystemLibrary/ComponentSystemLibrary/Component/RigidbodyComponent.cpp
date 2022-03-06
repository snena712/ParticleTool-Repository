#include "RigidbodyComponent.h"
#include "Transform.h"
#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"
#include "ImGuiComponent.h"

#define GRAVITY	(-9.80665)		//�d�͉����x
#define DT		(1.0f/60.0f)	//�f���^�^�C��

using namespace Component;

std::vector<GameObject*> Rigidbody::RigidbodyList;

Rigidbody::~Rigidbody() {

	if (RigidbodyList.size() > ID) {
		//���X�g���玩�g���폜
		RigidbodyList.erase(RigidbodyList.begin() + ID);

		//�폜���ID���Ď擾����
		for (int i = ID; i < RigidbodyList.size(); i++) {
			RigidbodyList[i]->GetComponent<Rigidbody>()->SetID();
		}
	}

}

bool Rigidbody::GetHitCheck() {


	for (auto collider : Collider) {
		if (collider->GetHitCheck()) {
			return true;
		}
	}

	return false;

}

void Rigidbody::SetID() {
	ID = std::find(RigidbodyList.begin(),
		RigidbodyList.end(), GetOwner())
		- RigidbodyList.begin();
}


void Rigidbody::PhysicsUpdate() {

	if (!AddListFlg) {
		AddList();
		SetID();
	}

	//�Փˈꗗ���N���A
	CollisionObject.clear();

	Transform *tra = owner->transform;

	Velocity += ADDForce * (1.0f / Mass);


	Velocity += Acceleration * DT;
	ADDForce *= 0.0f;

	//tra->GetPosition + (Velocity * DT);
	Vector3 nextpos = { tra->position.x + Velocity.x * DT ,
						tra->position.y + Velocity.y * DT ,
						tra->position.z + Velocity.z * DT };

	if (UseGravity) {
		Acceleration += Vector3(0.0f, GRAVITY * GravityPower, 0.0f);
	}

	tra->position = nextpos;


}

#ifdef DEBUG

void Rigidbody::ImGuiDebug()
{
	//�v���X���Ă��Q������
	std::string name = owner->GetName() + "G";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 120), true);
	ImGui::Text(u8"*RigidBody");
	// 2����
	ImGui::Text(u8"�d��");
	ImGui::SameLine();

	ImGui::Checkbox("�d��", (bool*)&UseGravity);

	ImGui::Text(u8"���x");
	ImGui::InputFloat3(u8"", (float*)&Velocity);

	ImGui::EndChild();
}

#endif
