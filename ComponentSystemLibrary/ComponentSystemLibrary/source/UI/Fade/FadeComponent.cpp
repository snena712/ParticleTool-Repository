#include "FadeComponent.h"

using namespace Component;

// gameに置く　instanceする
FadeComponent::FadeComponent(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("FadeComponent");

	// 爆破アニメーション
	ren = owner->GetComponent<Component::QuadRender>();
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, m_alpha));
}

FadeComponent::~FadeComponent()
{

}


void FadeComponent::Update()
{
	switch (m_state)
	{
	case Component::FadeComponent::FadeState::NONE:

		break;
	case Component::FadeComponent::FadeState::FADEIN:
		StartFade();
		break;
	case Component::FadeComponent::FadeState::FADEOUT:
		EndFade();
		break;
	default:
		break;
	}
}

void FadeComponent::DrawObject()
{
	if (m_alpha != m_oldalpha) {
		ren->SetVertexColor(XMFLOAT4(1, 1, 1, m_alpha));
		m_oldalpha = m_alpha;
	}
}


bool FadeComponent::StartFade()
{
	if (m_alpha < 1)
	{
		m_alpha += 1.0f / m_fadecnt;
		ren->SetVertexColor(XMFLOAT4(1, 1, 1, m_alpha));
		return false;
	}
	else {
		m_alpha = 1;
		m_state = FadeState::NONE;
		ren->SetVertexColor(XMFLOAT4(1, 1, 1, m_alpha));		

		return true;
	}
	
}


bool FadeComponent::EndFade()
{
	if (m_alpha > 0)
	{
		m_alpha -= 1.0f / m_fadecnt;
		ren->SetVertexColor(XMFLOAT4(1, 1, 1, m_alpha));
		return false;
	}
	else {
		m_alpha = 0;
		ren->SetVertexColor(XMFLOAT4(1, 1, 1, m_alpha));
		owner->SetExistState(false);

		m_state = FadeState::NONE;
		return true;
	}
}

