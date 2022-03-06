#include "CenterHitUI.h"
using namespace Component;

CenterHitUI::CenterHitUI(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("CenterHitUI");

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/CenterHItUI.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	owner->transform->position.x = 100000;

	ren->SetHeight(0.25f);
	ren->SetWidth(0.25f);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 1));
}

CenterHitUI::~CenterHitUI()
{

}

void CenterHitUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
}

void CenterHitUI::Update()
{
	if (StatusUpdate())
	{
		PosUpdate();
	}
}

void CenterHitUI::SetState(State _state)
{
	m_state = _state;

	switch (_state)
	{
	case State::UP_LEFT:
		ren->SetUVState(UV_STATE::TURN_XY);
		m_plus.x = +1;
		m_plus.y = -1;
		break;

	case State::UP_RIGHT:
		ren->SetUVState(UV_STATE::TURN_Y);
		m_plus.x = -1;
		m_plus.y = -1;
		break;

	case State::DOWN_LEFT:
		ren->SetUVState(UV_STATE::TURN_X);
		m_plus.x = +1;
		m_plus.y = +1;
		break;

	case State::DOWN_RIGHT:
		ren->SetUVState(UV_STATE::DEFAULT);
		m_plus.x = -1;
		m_plus.y = +1;
		break;
	default:
		break;
	}
	
	m_base.x = -m_plus.x/2;
	m_base.y = -m_plus.y/2;
}

void CenterHitUI::PosUpdate()
{
	owner->transform->position = m_camera->transform->position + m_camera->transform->forward() * 10 + m_camera->transform->up() * m_base.y
		+ m_camera->transform->right() * m_base.x;
}

bool CenterHitUI::StatusUpdate()
{
	ren->SetUVState(UV_STATE::DEFAULT);

	// 中央に近づく
	if (m_maxinputcnt > m_nowinputcnt)
	{
		m_nowinputcnt++;
		m_base.x += m_plus.x / m_maxinputcnt/2;
		m_base.y += m_plus.y / m_maxinputcnt/2;
	}
	// 中央から離れる
	//else if (m_maxinputcnt > m_nowinputcnt
	//	&&m_nowinputcnt > m_maxinputcnt / 2)
	//{
	//	m_nowinputcnt++;
	//	m_base.x -= m_plus.x/4;
	//	m_base.y -= m_plus.y/4;
	//}
	else
	{
		m_nowinputcnt = 0;
		m_base.x = -m_plus.x/2;
		m_base.y = -m_plus.y/2;
		owner->transform->position.x = 100000;
		owner->SetExistState(false);

		return false;
	}
	return true;
}
