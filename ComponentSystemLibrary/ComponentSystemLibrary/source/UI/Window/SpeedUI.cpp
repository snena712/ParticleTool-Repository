#include "SpeedUI.h"

using namespace Component;

SpeedUI::SpeedUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "SpeedUI");

	tra = owner->transform;

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/SpeedUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(20);
	ren->SetWidth(60);

	owner->AddComponent<Component::StartUIColorComponent>();
	owner->GetComponent<Component::StartUIColorComponent>()->SetType(Component::StartUIColorComponent::RnderState::TWO);

	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;

	owner->transform->position = Vector3(wid / 2 - 132, -hei / 2, 1);
}

SpeedUI::~SpeedUI()
{
}

void SpeedUI::Start()
{
}

void SpeedUI::Update()
{
}

