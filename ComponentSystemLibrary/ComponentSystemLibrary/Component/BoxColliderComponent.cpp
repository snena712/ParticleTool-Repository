#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "SphereColliderComponent.h"
#include "Transform.h"
#include "Collision.h"
#include "ImGuiComponent.h"

using namespace Component;

void BoxCollider::AddColliderList() {
	AddListFlg = true;
	owner->GetComponent<Rigidbody>()->SetColliderList(this);

	if (Scale.x <= 0.0f) {
		Scale = owner->transform->scale;
	}

}

void BoxCollider::Update() {
	if (Passive) {
		HitCheck = false;
	}
}
void BoxCollider::PhysicsUpdate() {

	if (!AddListFlg) {
		AddColliderList();
	}


	if (!Passive) {
		Rigidbody* rigid = owner->GetComponent<Rigidbody>();
		Transform* tra = owner->transform;

		static Vector3 distance;
		static Vector3 ObjScale;

		int loop = (int)Rigidbody::RigidbodyList.size();
		int ID = owner->GetComponent<Rigidbody>()->GetID();

		//�ʒu�ƃT�C�Y�̎擾
		Vector3 thisPos = tra->position + Center;
		Vector3 thisScale = tra->scale;
		thisScale.x *= Size.x;
		thisScale.y *= Size.y;
		thisScale.z *= Size.z;

		GameObject* obj;



		BoxCollider* box;
		SphereCollider* Sphere;
		CircleCollider* Circle;

		ColliderDirection HitDir = ColliderDirection::enumEnd;

		for (int i = ID + 1; i < loop; i++) {

			obj = Rigidbody::RigidbodyList[i];

			Vector3 ObjPos = obj->transform->position;




			for (auto collider : *obj->GetComponent<Rigidbody>()->GetColliderList()) {

				volatile bool hit = false;

				std::string col = collider->GetType();



				// typeid �� ���N���X�|�C���^��{���̃|�C���^�ɕϊ����Ĕ�r����
				if (col == std::string{ "Box" }) {

					box = dynamic_cast<BoxCollider*>(collider);
					ObjPos += obj->GetComponent<Component::BoxCollider>()->GetCenter();


					// �����蔻��2�_�Ԃ̋���
					distance = thisPos - ObjPos;

					// box ��Scale * size ��ݒ�
					ObjScale.x = box->GetScale().x * box->GetSize().x;
					ObjScale.y = box->GetScale().y * box->GetSize().y;
					ObjScale.z = box->GetScale().z * box->GetSize().z;

					// �{�b�N�X�̓����蔻����QD��Ŏ��s
					if (Type2D || box->Get2DFlg()) {
						if (HitCheckBox_Box(Vector3{ distance.x,distance.y,0 },
							Vector3{ ObjScale.x,ObjScale.y,0 },
							Vector3{ thisScale.x,thisScale.y,0 })) {
							hit = true;
							box->SetHitCheck(true);

							if (thisPos.x == ObjPos.x) {
								if (thisPos.y < ObjPos.y) {
									HitDir = ColliderDirection::Up;
								}
								else {
									HitDir = ColliderDirection::Down;
								}
							}
							else if (thisPos.y == ObjPos.y) {
								if (thisPos.x < ObjPos.x) {
									HitDir = ColliderDirection::Right;
								}
								else {
									HitDir = ColliderDirection::Left;
								}
							}
							else {

								volatile int X = ((int)((thisPos.x < ObjPos.x) & 1) * 2) - 1;
								volatile int Y = ((int)((thisPos.y < ObjPos.y) & 1) * 2) - 1;

								Vector3 oldPos = tra->OldPosition() + Center;
								Vector3 oldObjPos = obj->transform->OldPosition() +
									obj->GetComponent<Component::BoxCollider>()->GetCenter();

								if (HitCheckLine_Line(
									Vector3{ thisPos.x + (thisScale.x / 2.0f * (float)X),
											 thisPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldPos.x + (thisScale.x / 2.0f * (float)X),
											 oldPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y + (ObjScale.y / 2.0f * (float)Y),0 }
								)) {
									X < 0 ? HitDir = ColliderDirection::Left : HitDir = ColliderDirection::Right;
								}
								else if (HitCheckLine_Line(
									Vector3{ thisPos.x + (thisScale.x / 2.0f * (float)X),
											 thisPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldPos.x + (thisScale.x / 2.0f * (float)X),
											 oldPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ ObjPos.x + (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 }
								)) {
									Y < 0 ? HitDir = ColliderDirection::Down : HitDir = ColliderDirection::Up;
								}


								else if (HitCheckLine_Line(
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 oldObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ thisPos.x + (thisScale.x / 2.0f * (float)X),
											 thisPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ thisPos.x + (thisScale.x / 2.0f * (float)X),
											 thisPos.y - (thisScale.y / 2.0f * (float)Y),0 })) {
									X < 0 ? HitDir = ColliderDirection::Left : HitDir = ColliderDirection::Right;
								}
								else if (HitCheckLine_Line(
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 oldObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ thisPos.x + (thisScale.x / 2.0f * (float)X),
											 thisPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ thisPos.x - (thisScale.x / 2.0f * (float)X),
											 thisPos.y + (thisScale.y / 2.0f * (float)Y),0 })) {
									Y < 0 ? HitDir = ColliderDirection::Down : HitDir = ColliderDirection::Up;

								}

								if (HitCheckLine_Line(
									Vector3{ thisPos.x + (thisScale.x / 2.0f * (float)X),
											 thisPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldPos.x + (thisScale.x / 2.0f * (float)X),
											 oldPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 oldObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 oldObjPos.y + (ObjScale.y / 2.0f * (float)Y),0 }
								)) {
									X < 0 ? HitDir = ColliderDirection::Left : HitDir = ColliderDirection::Right;
								}
								else if (HitCheckLine_Line(
									Vector3{ thisPos.x + (thisScale.x / 2.0f * (float)X),
											 thisPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldPos.x + (thisScale.x / 2.0f * (float)X),
											 oldPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 oldObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldObjPos.x + (ObjScale.x / 2.0f * (float)X),
											 oldObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 }
								)) {
									Y < 0 ? HitDir = ColliderDirection::Down : HitDir = ColliderDirection::Up;
								}

								else if (HitCheckLine_Line(
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 oldObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldPos.x + (thisScale.x / 2.0f * (float)X),
											 oldPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldPos.x + (thisScale.x / 2.0f * (float)X),
											 oldPos.y - (thisScale.y / 2.0f * (float)Y),0 })) {
									X < 0 ? HitDir = ColliderDirection::Left : HitDir = ColliderDirection::Right;
								}
								else if (HitCheckLine_Line(
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 oldObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldPos.x + (thisScale.x / 2.0f * (float)X),
											 oldPos.y + (thisScale.y / 2.0f * (float)Y),0 },
									Vector3{ oldPos.x - (thisScale.x / 2.0f * (float)X),
											 oldPos.y + (thisScale.y / 2.0f * (float)Y),0 })) {
									Y < 0 ? HitDir = ColliderDirection::Down : HitDir = ColliderDirection::Up;

								}

								else if (HitCheckLine_Line(thisPos, ObjPos,
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y + (ObjScale.y / 2.0f * (float)Y),0 })) {
									X < 0 ? HitDir = ColliderDirection::Left : HitDir = ColliderDirection::Right;
								}
								else if (HitCheckLine_Line(thisPos, ObjPos,
									Vector3{ ObjPos.x - (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 },
									Vector3{ ObjPos.x + (ObjScale.x / 2.0f * (float)X),
											 ObjPos.y - (ObjScale.y / 2.0f * (float)Y),0 })) {
									Y < 0 ? HitDir = ColliderDirection::Down : HitDir = ColliderDirection::Up;
								}
							}
						}
						// �{�b�N�X�̓����蔻����RD�Ŏ��s
						else {

							if (HitCheckBox_Box(distance, Vector3{ ObjScale.x,ObjScale.y, ObjScale.z }, thisScale)) {
								hit = true;
								box->SetHitCheck(true);
							}
						}
						box = nullptr;
					}
				}
				else if (col == std::string{ "Spehre" }) {
					ObjPos += obj->GetComponent<Component::SphereCollider>()->GetCenter();
					Sphere = dynamic_cast<SphereCollider*>(collider);

					Vector3 circlePos = ObjPos + Sphere->GetCenter();	//�����蔻��2�_�Ԃ̋���
					float circleRadius = Sphere->GetRadius() * Sphere->GetSize();		//circle�̔��a


					if (HitCheckBox_Sphere(thisPos, thisScale, circlePos, circleRadius)) {
						hit = true;
						Sphere->SetHitCheck(true);
					}
					Sphere = nullptr;

				}
				else if (col == std::string{ "Circle" }) {
					ObjPos += obj->GetComponent<Component::CircleCollider>()->GetCenter();
					Circle = dynamic_cast<CircleCollider*>(collider);

					Vector3 circlePos = ObjPos + Circle->GetCenter();	//�����蔻��2�_�Ԃ̋���
					float circleRadius = Circle->GetRadius() * Circle->GetSize();		//circle�̔��a


					if (HitCheckBox_Circle(thisPos, thisScale, circlePos, circleRadius)) {
						hit = true;
						Circle->SetHitCheck(true);
					}
					Circle = nullptr;
				}




				if (hit) {

					HitCheck = true;
					bool flg = true;
					ColliderObject hoge = { nullptr,ColliderDirection::enumEnd };

					//���g��<Rigidbody>����"CollisionObject"��
					//	�@���������I�u�W�F�N�g���Ȃ���΃��X�g�ɒǉ�����
					for (auto& col : *rigid->GetCollisionObject()) {
						if (col.obj == obj) {
							flg = false;

							break;
						}
					}

					if (flg) {
						hoge.obj = obj;
						hoge.dir = HitDir;
						rigid->GetCollisionObject()->emplace_back(hoge);


						hoge.obj = owner;

						HitDir & 1 ?
							hoge.dir = (ColliderDirection)((int)HitDir - 1) :
							hoge.dir = (ColliderDirection)((int)HitDir + 1);

						obj->GetComponent<Rigidbody>()->GetCollisionObject()->emplace_back(hoge);

					}

				}

			}
		}

	}
}

#ifdef DEBUG

void BoxCollider::ImGuiDebug()
{
	std::string name = owner->GetName() + "C";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 80), true);
	ImGui::Text(u8"*Collider");
	// 2����
	ImGui::Text(u8"�R���C�_�[�̃^�C�v �F BoxCollider");

	ImGui::Text(u8"���S�_");
	ImGui::SameLine();

	ImGui::Text(u8"%f%f%f", Center);

	ImGui::EndChild();
}

#endif
