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
	//プラスしてｃ＿ｓｔｒ
	std::string name = owner->GetName() + "C";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 220), true);
	ImGui::Text(u8"*Camera");

	Vector3 eye = owner->transform->position;
	Vector3 look = owner->transform->forward() + owner->transform->position;
	Vector3 up = owner->transform->up();

	// constの為書換不可
	ImGui::Text(u8"カメラの位置");
	ImGui::Text(u8"x座標:%f y座標:%f　z座標:%f", eye.x, eye.y, eye.z);

	ImGui::Text(u8"カメラの視点");
	ImGui::Text(u8"%f%f%f", look);

	ImGui::Text(u8"カメラの向き");
	ImGui::Text(u8"%f%f%f", up);

	ImGui::Text(u8"カメラの視野角");
	ImGui::Text(u8"%f", m_Fov);

	ImGui::EndChild();
}

#endif