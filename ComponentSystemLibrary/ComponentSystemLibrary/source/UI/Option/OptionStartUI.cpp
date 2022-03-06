#include "OptionStartUI.h"
#include "../../../Component/SceneManager.h"

using namespace Component;

OptionStartUI::OptionStartUI(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("OptionStartUI");

	tra = owner->transform;

	m_size = Vector2(5, 5);
}

OptionStartUI::~OptionStartUI()
{

}

void OptionStartUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("player");
}

void OptionStartUI::Update()
{
	//MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::NORMAL);
	PosUpdate();
	owner->SetExistState(false);
}

void OptionStartUI::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 10 + m_camera->transform->up() * 5;
}



// todo ‰ñ“]‚µ‚½‚Æ‚«•Ï‚ÈêŠ‚És‚­