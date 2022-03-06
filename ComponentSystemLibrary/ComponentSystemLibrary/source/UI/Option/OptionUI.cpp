#include "OptionUI.h"
#include "../../../Component/SceneManager.h"

using namespace Component;

OptionUI::OptionUI(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("OptionUI");

	tra = owner->transform;

	m_size = Vector2(5, 5);

}

OptionUI::~OptionUI()
{

}

void OptionUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("player");
}

void OptionUI::Update()
{
	PosUpdate();
}

void OptionUI::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 10 + m_camera->transform->up() * 5;
}



// todo ‰ñ“]‚µ‚½‚Æ‚«•Ï‚ÈêŠ‚És‚­