#include "StartUIColorComponent.h"
#include "../Component/SceneManager.h"

using namespace Component;

StartUIColorComponent::StartUIColorComponent(GameObject* _owner) : ComponentBase(_owner)
{

	m_color = XMFLOAT4(0.8f, 0.8f, 0.8f, 0);
	
}

StartUIColorComponent::~StartUIColorComponent()
{

}

void StartUIColorComponent::Start()
{
}
void StartUIColorComponent::SetType(RnderState _state)
{
	switch (_state)
	{
	case Component::StartUIColorComponent::RnderState::NORMAL:
		if (ren == nullptr)
		{
			ren = owner->GetComponent<Component::QuadRender>();
			ren->SetVertexColor(m_color);
		}
		
		break;
	case Component::StartUIColorComponent::RnderState::TWO:
		if (ren2d == nullptr)
		{
			ren2d = owner->GetComponent<Component::Quad2DRender>();
			ren2d->SetVertexColor(m_color);
		}
		
		break;
	default:
		break;
	}
}
void StartUIColorComponent::Update()
{
	CheckStart();
}

void StartUIColorComponent::CheckStart()
{
	if (ren != nullptr)
	{
		if (m_color.w != 0.7f) {
			if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL)
			{
				m_color.w += 0.1;
				ren->SetVertexColor(m_color);

				if (m_color.w > 0.7f)
				{
					m_color.w = 0.7f;
				}
			}
		}
		if (m_color.w != 0) {
			if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::NORMAL)
			{
				m_color.w -= 0.1;
				ren->SetVertexColor(m_color);

				if (m_color.w <= 0)
				{
					m_color.w = 0;
				}
			}
		}
	}
	else if (ren2d != nullptr)
	{
		if (m_color.w != 0.7f) {
			if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL)
			{
				m_color.w += 0.1;
				ren2d->SetVertexColor(m_color);
			}
			if (m_color.w > 0.7f)
			{
				m_color.w = 0.7f;
			}
		}

		if (m_color.w != 0) {
			if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::NORMAL)
			{
				m_color.w -= 0.1;
				ren2d->SetVertexColor(m_color);

				if (m_color.w <= 0)
				{
					m_color.w = 0;
				}
			}
		}
	}
}

