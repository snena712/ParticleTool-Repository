#include "Transform.h"

#ifdef DEBUG

void Transform::ImGuiDebug()
{
	//プラスしてｃ＿ｓｔｒ
	std::string name = gameObject->GetName() + "T";
	auto* name2 = name.c_str();

	ImGui::BeginChild(name2, ImVec2(0, 65), true);
	ImGui::Text(u8"*Transform");
	// 3次元座標
	ImGui::InputFloat3(u8"プレイヤー座標", (float*)&this->position);
	ImGui::EndChild();
}

#endif