#include "PressButton.h"
#include "../../../Component/SceneManager.h"
#include "../../../DX11System/mystring.h"

using namespace Component;

PressButton::PressButton(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName( "PressButton");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/Buttun/YButtun.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	ren->SetHeight(3);
	ren->SetWidth(3);
	ren->SetVertexColor(XMFLOAT4(1, 0, 0, 1));
}

PressButton::~PressButton()
{

}

void PressButton::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
	player = SceneManager::Instance()->GameObjectFindName("player")->GetComponent<Component::TitlePlayer>();
}

void PressButton::Update()
{
	PosUpdate();

	ButtonUpdate();
}

void PressButton::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 10 + m_camera->transform->right() * 10
		- m_camera->transform->up() * 10;
}

void PressButton::ButtonUpdate()
{

}

