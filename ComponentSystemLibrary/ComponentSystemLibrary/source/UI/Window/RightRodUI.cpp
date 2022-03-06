#include "RightRodUI.h"

using namespace Component;

RightRodUI::RightRodUI(GameObject* _owner) :ComponentBase(_owner)
{
	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/RightUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(175);
	ren->SetWidth(50);

	owner->AddComponent<Component::StartUIColorComponent>();
	owner->GetComponent<Component::StartUIColorComponent>()->SetType(Component::StartUIColorComponent::RnderState::TWO);

	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;
	
	owner->transform->position = Vector3(wid / 2 + 100, -hei / 2, 1);
}

RightRodUI::~RightRodUI()
{

}

void RightRodUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("player");
}

void RightRodUI::Update()
{
}

