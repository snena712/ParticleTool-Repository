#include "LeftRodUI.h"

using namespace Component;

LeftRodUI::LeftRodUI(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName( "WindowUI");

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/LeftUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(175);
	ren->SetWidth(50);

	owner->AddComponent<Component::StartUIColorComponent>();
	owner->GetComponent<Component::StartUIColorComponent>()->SetType(Component::StartUIColorComponent::RnderState::TWO);
	
	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;

	owner->transform->position = Vector3(wid / 2-100, -hei/2, 1);
}

LeftRodUI::~LeftRodUI()
{

}


