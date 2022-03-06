#include "TitleMainUI.h"
#include "../../../Component/SceneManager.h"
#include "../../../DX11System/mystring.h"

using namespace Component;

TitleMainUI::TitleMainUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName( "enecnt");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/Buttun/YButtun.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	ren->SetHeight(10);
	ren->SetWidth(10);
	ren->SetVertexColor(XMFLOAT4(1, 0, 0, 1));
}

TitleMainUI::~TitleMainUI()
{

}

void TitleMainUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
	player = SceneManager::Instance()->GameObjectFindName("player")->GetComponent<Component::TitlePlayer>();
}

void TitleMainUI::Update()
{
	PosUpdate();

	ButtonUpdate();
}

void TitleMainUI::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 10;
}

void TitleMainUI::ButtonUpdate()
{
	if (player->GetInputFlg())
	{
		ren->SetVertexColor(XMFLOAT4(1, 0, 0, 0));
	}
}

