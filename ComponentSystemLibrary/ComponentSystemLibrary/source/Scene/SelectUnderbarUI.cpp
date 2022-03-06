#include "SelectUnderbarUI.h"

using namespace Component;

SelectUnderbarUI::SelectUnderbarUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("SelectUnderbarUI");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/Select/Underbar.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	ren->SetHeight(0.08f);
	ren->SetWidth(0.7);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
}

SelectUnderbarUI::~SelectUnderbarUI()
{

}

void SelectUnderbarUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
}

void SelectUnderbarUI::Update()
{
	PosUpdate();
}

void SelectUnderbarUI::PosUpdate()
{
	tra->position = m_camera->transform->position +
		m_camera->transform->right() * m_pos.x
		+ m_camera->transform->up() * m_pos.y
		+ m_camera->transform->forward() * m_pos.z;
}

