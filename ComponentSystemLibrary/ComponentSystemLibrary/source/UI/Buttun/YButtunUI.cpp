#include "YButtunUI.h"

using namespace Component;

YButtunUI::YButtunUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("YButtunUI");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/Buttun/YButtun.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	ren->SetHeight(5);
	ren->SetWidth(5);
	ren->SetVertexColor(XMFLOAT4(1, 0, 0, 1));
}

YButtunUI::~YButtunUI()
{

}

void YButtunUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
}

void YButtunUI::Update()
{
	PosUpdate();
}

void YButtunUI::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 51 + m_camera->transform->right() * 72
		- m_camera->transform->up() * 30;
}

