#include "BoxColliderComponent.h"
#include "SphereColliderComponent.h"
#include "Transform.h"
#include "Collision.h"
#include "ImGuiComponent.h"

using namespace Component;

void SphereCollider::GetRadiusSize() {

	Vector3 rad = owner->transform->scale;

	//Typeによって取得する半径を変える
	switch (Type)
	{
	case SphereType::Auto:	//自動　短い距離を取得する	

		if (rad.x < rad.y) {
			rad.x < rad.z ? Radius = rad.x / 2.0f : Radius = rad.z / 2.0f;
		}
		else {
			rad.y < rad.z ? Radius = rad.y / 2.0f : Radius = rad.y / 2.0f;
		}
		break;
	case SphereType::X:		//Xを取得
		Radius = owner->transform->scale.x / 2.0f;
		break;
	case SphereType::Y:		//Yを取得
		Radius = owner->transform->scale.y / 2.0f;
		break;
	case SphereType::Z:
		Radius = owner->transform->scale.z / 2.0f;
	default:
		break;
	}
}

void SphereCollider::AddColliderList() {
	AddListFlg = true;
	owner->GetComponent<Rigidbody>()->SetColliderList(this);
	if (Radius <= 0.0f) {
		GetRadiusSize();
	}
}

void SphereCollider::Update() {
	if (Passive) {
		HitCheck = false;
	}
}

void SphereCollider::PhysicsUpdate() {

	if (!AddListFlg) {
		AddColliderList();
	}

	static Vector3 ObjPos;
	static Vector3 ObjScale;


	if (!Passive) {

		Rigidbody* rigid = owner->GetComponent<Rigidbody>();

		int loop = (int)Rigidbody::RigidbodyList.size();
		int ID = rigid->GetID();


		//位置と半径の取得
		Vector3 thisPos = owner->transform->position + Center;
		float thisRadius = Radius * Size;

		GameObject* obj;


		BoxCollider* box = nullptr;
		SphereCollider* circle = nullptr;

		for (int i = ID + 1; i < loop; i++) {

			obj = Rigidbody::RigidbodyList[i];

			ObjPos = obj->transform->position;

			for (auto collider : *obj->GetComponent<Rigidbody>()->GetColliderList()) {

				bool hit = false;

				std::string col = collider->GetType();

				// typeid ⇒ 基底クラスポインタを本来のポインタに変換して比較する
				if (col == std::string{ "Box" }) {
					ObjPos += obj->GetComponent<Component::BoxCollider>()->GetCenter();

					box = dynamic_cast <BoxCollider*>(collider);
					ObjScale = { box->GetScale().x * box->GetSize().x,box->GetScale().y * box->GetSize().y,box->GetScale().z * box->GetSize().z };

					if (HitCheckBox_Sphere(ObjPos + box->GetCenter(), ObjScale, thisPos, thisRadius)) {
						hit = true;
						box->SetHitCheck(true);
					}
					box = nullptr;
				}
				else if (col == std::string{ "Sphere" }) {
					ObjPos += obj->GetComponent<Component::SphereCollider>()->GetCenter();
					circle = dynamic_cast <SphereCollider*>(collider);

					Vector3 distance = thisPos - (ObjPos + circle->GetCenter());	//当たり判定2点間の距離
					float circleRadius = circle->GetRadius() * circle->GetSize();		//circleの半径

					if (HitCheckSphere_Sphere(distance, thisRadius, circleRadius)) {
						hit = true;
						circle->SetHitCheck(true);
					}
					circle = nullptr;
				}


				if (hit) {

					HitCheck = true;

					bool flg = true;

					//自身の<Rigidbody>内の"CollisionObject"に
					//	　当たったオブジェクトがなければリストに追加する
					for (auto& col : *rigid->GetCollisionObject()) {
						if (col.obj == obj) {
							flg = false;
							break;
						}
					}

					if (flg) {
						rigid->GetCollisionObject()->emplace_back(
							ColliderObject{ obj,ColliderDirection::enumEnd });
						obj->GetComponent<Rigidbody>()->GetCollisionObject()->emplace_back(
							ColliderObject{ owner,ColliderDirection::enumEnd });

					}

				}

			}

		}
	}
}

#ifdef DEBUG

void SphereCollider::ImGuiDebug()
{
	std::string name = owner->GetName() + "C";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 80), true);
	ImGui::Text(u8"*SphereCollider");

	ImGui::Text(u8"コライダーのタイプ ： CircleCollider");
	ImGui::Text(u8"中心点");
	ImGui::SameLine();

	ImGui::Text(u8"%f%f%f", Center);

	ImGui::EndChild();
}

#endif
