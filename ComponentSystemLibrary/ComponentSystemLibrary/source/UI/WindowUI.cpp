#include "../UI/WindowUI.h"
#include "../../Component/SceneManager.h"

using namespace Component;

WindowUI::WindowUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "WindowUI");

	tra = owner->transform;

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/CeneterUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(50);
	ren->SetWidth(50);

	owner->AddComponent<Component::StartUIColorComponent>();
	owner->GetComponent<Component::StartUIColorComponent>()->SetType(Component::StartUIColorComponent::RnderState::TWO);

	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;

	owner->transform->position = Vector3(wid / 2 , -hei / 2 , 1);
}

WindowUI::~WindowUI()
{

}

void WindowUI::Start()
{
}

void WindowUI::Update()
{
	HitUpdate();
}


void WindowUI::HitUpdate()
{
	//if (m_hitflg)
	//{
	//	m_size.x++;
	//	m_size.y++;
	//	ren->SetHeight(m_size.x);
	//	ren->SetWidth(m_size.y);
	//	m_hitflg = false;
	//}
}

// todo ‰ñ“]‚µ‚½‚Æ‚«•Ï‚ÈêŠ‚És‚­