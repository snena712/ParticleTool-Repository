#include "HomingBulletUI.h"
#include "../../Component/SceneManager.h"

using namespace Component;

// ミサイルの残段数
HomingBulletUI::HomingBulletUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "HomingBulletUI");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/HomingBulletUI.png");
	ren->SetAnchorType(AnchorType::LeftTop);

	ren->SetHeight(40);
	ren->SetWidth(60);

	owner->AddComponent<Component::StartUIColorComponent>();
	owner->GetComponent<Component::StartUIColorComponent>()->SetType(Component::StartUIColorComponent::RnderState::TWO);

	float wid = Application::CLIENT_WIDTH;
	float hei = Application::CLIENT_HEIGHT;

	owner->transform->position = Vector3(wid / 2 + 450, -hei / 2 - 215, 1);
}

HomingBulletUI::~HomingBulletUI()
{
}

void HomingBulletUI::Start()
{
}


