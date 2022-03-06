#include "FadeManager.h"

using namespace Component;

// gameに置く　instanceする
FadeManager::FadeManager(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("FadeManager");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/white.bmp");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetVertexColor(XMFLOAT4( 0, 0, 0, m_alpha));

	ren->SetHeight(0);
	ren->SetWidth(0);
}

FadeManager::~FadeManager()
{

}

void FadeManager::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
	PosUpdate();
}

void FadeManager::Update()
{
	switch (m_state)
	{
	case Component::FadeManager::FadeState::NONE:
		
		break;
	case Component::FadeManager::FadeState::FADEIN:
		StartFade();
		break;
	case Component::FadeManager::FadeState::FADEOUT:
		EndFade();
		break;
	default:
		break;
	}
	
	PosUpdate();
}

void FadeManager::DrawUI()
{
	ren->SetVertexColor(XMFLOAT4(0, 0, 0, m_alpha));
}

void FadeManager::PosUpdate()
{

	tra->position = m_camera->transform->position + m_camera->transform->forward() * 1;

	//tra->position = Vector3(0, 0, -15);
}

bool FadeManager::StartFade()
{
	if (m_alpha < 1)
	{
		m_alpha += 1.0f / m_fadecnt;
	}
	else {
		m_alpha = 1;
		m_state = FadeState::NONE;
		owner->SetExistState(false);

		// 黒くなるとシーン読み込み
		if (!SceneManager::Instance()->ActiveScene(m_scenename)) {
			exit(EXIT_FAILURE);
		}
	}
	ren->SetVertexColor(XMFLOAT4(0, 0, 0, m_alpha));
}


bool FadeManager::EndFade()
{
	if (m_alpha >0)
	{
		m_alpha -= 1.0f / m_fadecnt;
		ren->SetVertexColor(XMFLOAT4(0, 0, 0, m_alpha));
		return false;
	}
	else {
		m_alpha = 0;
		ren->SetVertexColor(XMFLOAT4(0, 0, 0, m_alpha));
		owner->SetExistState(false);

		m_state = FadeState::NONE;
		return true;
	}
}

