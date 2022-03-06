#include "OptionTitleUI.h"
#include "../../../Component/SceneManager.h"

using namespace Component;

OptionTitleUI::OptionTitleUI(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("OptionUI");

	m_size = Vector2(5, 5);
}

OptionTitleUI::~OptionTitleUI()
{

}

void OptionTitleUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("player");
	owner->SetExistState(false);
}

void OptionTitleUI::Update()
{
	SceneUpdate();

	PosUpdate();
}

void OptionTitleUI::PosUpdate()
{
	owner->transform->position = m_camera->transform->position + m_camera->transform->forward() * 10 + m_camera->transform->up() * 5;
}

void OptionTitleUI::SceneUpdate()
{
	SceneManager::Instance()->GameObjectFindName("FadeManager")
		->GetComponent<Component::FadeManager>()->SetFadeScene(60, "TitleScene");

	owner->SetExistState(false);
}

// todo ‰ñ“]‚µ‚½‚Æ‚«•Ï‚ÈêŠ‚És‚­