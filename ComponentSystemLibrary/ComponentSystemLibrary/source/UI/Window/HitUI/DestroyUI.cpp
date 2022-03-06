#include "DestroyUI.h"
using namespace Component;

DestroyUI::DestroyUI(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("DestroyUI");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/DestroyUI.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	tra->position.x = 100000;

	ren->SetHeight(5);
	ren->SetWidth(8);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0.8f));
}

DestroyUI::~DestroyUI()
{

}

void DestroyUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
}

void DestroyUI::Update()
{
	StatusUpdate();
	PosUpdate();
}

void DestroyUI::PosUpdate()
{
	tra->position =
		m_camera->transform->position + m_camera->transform->forward() * 50.1f + m_camera->transform->up() * 20;
}

void DestroyUI::StatusUpdate()
{
	if (m_maxinputcnt > m_nowinputcnt)
	{
		m_nowinputcnt++;
	}
	else {
		m_nowinputcnt = 0;
		tra->position.x = 100000;
		owner->SetExistState(false);
	}
}
