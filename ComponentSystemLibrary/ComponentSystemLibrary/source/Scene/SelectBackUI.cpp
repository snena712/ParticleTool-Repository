#include "SelectBackUI.h"

using namespace Component;

SelectBackUI::SelectBackUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("SelectBackUI");

	tra = owner->transform;


	ren = owner->AddComponent<Component::QuadRender>("assets/Image/Select/SelectBackUI.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	ren->SetHeight(2.5f);
	ren->SetWidth(3.5f);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));

	owner->AddComponent<Component::FadeComponent>();

}

SelectBackUI::~SelectBackUI()
{

}

void SelectBackUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
}

void SelectBackUI::Update()
{
	PosUpdate();
}

void SelectBackUI::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 1.1f;
}
