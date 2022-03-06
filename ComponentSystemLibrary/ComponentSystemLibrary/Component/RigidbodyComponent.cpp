#include "RigidbodyComponent.h"
#include "Transform.h"
#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"
#include "ImGuiComponent.h"

#define GRAVITY	(-9.80665)		//重力加速度
#define DT		(1.0f/60.0f)	//デルタタイム

using namespace Component;

std::vector<GameObject*> Rigidbody::RigidbodyList;

Rigidbody::~Rigidbody() {

	if (RigidbodyList.size() > ID) {
		//リストから自身を削除
		RigidbodyList.erase(RigidbodyList.begin() + ID);

		//削除後のIDを再取得する
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

	//衝突一覧をクリア
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
	//プラスしてｃ＿ｓｔｒ
	std::string name = owner->GetName() + "G";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 120), true);
	ImGui::Text(u8"*RigidBody");
	// 2次元
	ImGui::Text(u8"重力");
	ImGui::SameLine();

	ImGui::Checkbox("重力", (bool*)&UseGravity);

	ImGui::Text(u8"速度");
	ImGui::InputFloat3(u8"", (float*)&Velocity);

	ImGui::EndChild();
}

#endif
