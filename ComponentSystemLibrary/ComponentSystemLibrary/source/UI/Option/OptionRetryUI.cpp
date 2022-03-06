#include "OptionRetryUI.h"
#include "../../../Component/SceneManager.h"

using namespace Component;

OptionRetryUI::OptionRetryUI(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("OptionUI");

	m_size = Vector2(5, 5);
}

OptionRetryUI::~OptionRetryUI()
{

}

void OptionRetryUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("player");
	owner->SetExistState(false);
}

void OptionRetryUI::Update()
{
	SceneUpdate();
	PosUpdate();
}


void OptionRetryUI::PosUpdate()
{
}

void OptionRetryUI::SceneUpdate()
{
	auto _str = SceneManager::Instance()->GetActiveScene()->GetSceneName();

	SceneManager::Instance()->GameObjectFindName("FadeManager")
		->GetComponent<Component::FadeManager>()->SetFadeScene(60, _str);

	owner->SetExistState(false);
}

// todo ‰ñ“]‚µ‚½‚Æ‚«•Ï‚ÈêŠ‚És‚­