#include "Transform.h"

#ifdef DEBUG

void Transform::ImGuiDebug()
{
	//�v���X���Ă��Q������
	std::string name = gameObject->GetName() + "T";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 65), true);
	ImGui::Text(u8"*Transform");
	// 3�������W
	ImGui::InputFloat3(u8"�v���C���[���W", (float*)&this->position);
	ImGui::EndChild();
}

#endif