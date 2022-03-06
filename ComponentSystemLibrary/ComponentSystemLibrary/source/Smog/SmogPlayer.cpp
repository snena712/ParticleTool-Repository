#include "SmogPlayer.h"

using namespace Component;

SmogPlayer::SmogPlayer(GameObject *_owner) : ComponentBase(_owner)
{
	 owner->SetName( "smog");

	ren = owner->AddComponent<Component::QuadRender>("assets/Image/Smog/RealSmog2.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0.9f));
	ren->SetWidth(1);
	ren->SetHeight(1);	
}

SmogPlayer::~SmogPlayer()
{
}

void SmogPlayer::Start()
{

}

void SmogPlayer::Update()
{
	if (status == Status::LIVE) {
		
		AlphaReduce();
	}
}

void SmogPlayer::AlphaReduce() {
	if (owner->GetExistState()) 
	{
		if (status == Status::LIVE) {
			ren->SetVertexColor(m_color);
			ren->SetHeight(m_height);
			ren->SetWidth(m_width);

			m_color.w -= m_reduce;
			m_width += m_plussize.x;
			m_height += m_plussize.y;
		}

		if (m_color.w < 0)
		{
			ren->SetHeight(0);
			ren->SetWidth(0);
			status = Status::DEAD;
			owner->SetExistState(false);
		}
	}
}


void SmogPlayer::SetType(ObjType _type)
{
	m_Type = _type;

	switch (m_Type)
	{
	case ObjType::Player:
		ren->ChangeTexture("assets/Image/Smog/RealSmog2.png");
		break;
	case ObjType::Enemy:
		ren->ChangeTexture("assets/Image/Smog/RealSmog2.png");
		break;
	default:
		break;
	}
}
