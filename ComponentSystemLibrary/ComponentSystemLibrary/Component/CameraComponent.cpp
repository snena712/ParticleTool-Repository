#include "CameraComponent.h"
#include "ImGuiComponent.h"

using namespace Component;

Camera* Camera::m_MainCamera;

void Camera::LateUpdate() {

	CreateProjectionMatrix();
	CreateCameraMatrix();
}

void Camera::Draw() {

}

#ifdef DEBUG

void Camera::ImGuiDebug()
{
	//�v���X���Ă��Q������
	std::string name = owner->GetName() + "C";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 220), true);
	ImGui::Text(u8"*Camera");

	Vector3 eye = owner->transform->position;
	Vector3 look = owner->transform->forward() + owner->transform->position;
	Vector3 up = owner->transform->up();

	// const�̈׏����s��
	ImGui::Text(u8"�J�����̈ʒu");
	ImGui::Text(u8"x���W:%f y���W:%f�@z���W:%f", eye.x, eye.y, eye.z);

	ImGui::Text(u8"�J�����̎��_");
	ImGui::Text(u8"%f%f%f", look);

	ImGui::Text(u8"�J�����̌���");
	ImGui::Text(u8"%f%f%f", up);

	ImGui::Text(u8"�J�����̎���p");
	ImGui::Text(u8"%f", m_Fov);

	ImGui::EndChild();
}

#endif