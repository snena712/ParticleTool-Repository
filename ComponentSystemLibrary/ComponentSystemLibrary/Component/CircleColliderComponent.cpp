#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "Transform.h"
#include "Collision.h"
#include "ImGuiComponent.h"

using namespace Component;


void CircleCollider::GetRadiusSize() {

	Vector3 rad = owner->transform->scale;

	//Typeによって取得する半径を変える
	switch (Type)
	{
	case CircleType::Auto:	//自動　短い距離を取得する	

		rad.x < rad.y ? Radius = rad.x / 2.0f : Radius = rad.y / 2.0f;
		break;
	case CircleType::X:		//Xを取得
		Radius = owner->transform->scale.x / 2.0f;
		break;
	case CircleType::Y:		//Yを取得
		Radius = owner->transform->scale.y / 2.0f;
		break;
	default:
		break;
	}
}

void CircleCollider::AddColliderList() {
	AddListFlg = true;
	owner->GetComponent<Rigidbody>()->SetColliderList(this);
	if (Radius <= 0.0f) {
		GetRadiusSize();
	}
}

void CircleCollider::Update() {
	if (Passive) {
		HitCheck = false;
	}
}

void CircleCollider::PhysicsUpdate() {

	if (!AddListFlg) {
		AddColliderList();
	}

	static Vector3 ObjPos;
	static Vector3 ObjScale;
	static Vector3 distance;


	if (!Passive) {

		Rigidbody* rigid = owner->GetComponent<Rigidbody>();

		int loop = (int)Rigidbody::RigidbodyList.size();
		int ID = rigid->GetID();


		//位置と半径の取得
		Vector3 thisPos = owner->transform->position + Center;
		float thisRadius = Radius * Size;

		GameObject* obj;


		BoxCollider* box = nullptr;
		CircleCollider* circle = nullptr;

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
					ObjScale = { box->GetScale().x * box->GetSize().x,box->GetScale().y * box->GetSize().y , 0 };

					if (HitCheckBox_Circle(ObjPos + box->GetCenter(), ObjScale, thisPos, thisRadius)) {
						hit = true;
						box->SetHitCheck(true);
					}
					box = nullptr;
				}
				else if (col == std::string{ "Circle" }) {
					ObjPos += obj->GetComponent<Component::CircleCollider>()->GetCenter();
					circle = dynamic_cast <CircleCollider*>(collider);

					distance = thisPos - (ObjPos + circle->GetCenter());	//当たり判定2点間の距離
					float circleRadius = circle->GetRadius() * circle->GetSize();		//circleの半径

					if (HitCheckCircle_Circle(distance, thisRadius, circleRadius)) {
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

void CircleCollider::ImGuiDebug()
{
	std::string name = owner->GetName() + "C";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 80), true);
	ImGui::Text(u8"*Collider");

	ImGui::Text(u8"コライダーのタイプ ： CircleCollider");
	ImGui::Text(u8"中心点");
	ImGui::SameLine();

	ImGui::Text(u8"%f%f%f", Center);

	ImGui::EndChild();
}

#endif
