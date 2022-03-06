#include "OptionSettingUI.h"
#include "../../../Component/SceneManager.h"

using namespace Component;

OptionSettingUI::OptionSettingUI(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("OptionSettingUI");

	tra = owner->transform;
	m_size = Vector2(5, 5);
}

OptionSettingUI::~OptionSettingUI()
{

}

void OptionSettingUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("player");
}

void OptionSettingUI::Update()
{
	PosUpdate();
}

void OptionSettingUI::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 10 + m_camera->transform->up() * 5;
}



// todo ‰ñ“]‚µ‚½‚Æ‚«•Ï‚ÈêŠ‚És‚­