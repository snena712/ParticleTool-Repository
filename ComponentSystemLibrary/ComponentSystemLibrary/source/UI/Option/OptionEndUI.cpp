#include "OptionEndUI.h"
#include "../../../Component/SceneManager.h"

using namespace Component;

OptionEndUI::OptionEndUI(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("OptionEndUI");

	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/CeneterUI.png");

	m_size = Vector2(5, 5);

}

OptionEndUI::~OptionEndUI()
{

}

void OptionEndUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("player");
	owner->SetExistState(false);
}

void OptionEndUI::Update()
{
	PosUpdate();
	
	PostQuitMessage(0);
}

void OptionEndUI::PosUpdate()
{
	owner->transform->position = m_camera->transform->position + m_camera->transform->forward() * 10 + m_camera->transform->up() * 5;
}



// todo ‰ñ“]‚µ‚½‚Æ‚«•Ï‚ÈêŠ‚És‚­