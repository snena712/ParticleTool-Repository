#include "HitUI.h"
using namespace Component;

HitUI::HitUI(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("HitUI");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/HitUI.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	tra->position.x = 100000;

	ren->SetHeight(5);
	ren->SetWidth(8);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0.8f));
}

HitUI::~HitUI()
{

}

void HitUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
}

void HitUI::Update()
{
	StatusUpdate();
}

void HitUI::PosUpdate()
{
	tra->position =
		m_camera->transform->position + m_camera->transform->forward() * 50.1f + m_camera->transform->up() * 20;
}

void HitUI::StatusUpdate()
{
	if (m_maxinputcnt > m_nowinputcnt)
	{
		PosUpdate();
		m_nowinputcnt++;
	}
	else {
		m_nowinputcnt = 0;
		tra->position.x = 100000;
		owner->SetExistState(false);
	}
}