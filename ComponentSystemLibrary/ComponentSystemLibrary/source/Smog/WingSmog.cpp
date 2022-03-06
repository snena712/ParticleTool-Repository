#include "WingSmog.h"

using namespace Component;

WingSmog::WingSmog(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "WingSmog");

	ren = owner->AddComponent<Component::QuadRender>("assets/Image/Smog/RealSmog.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0.9f));
	ren->SetWidth(1);
	ren->SetHeight(1);

}

WingSmog::~WingSmog()
{
}

void WingSmog::Start()
{

}

void WingSmog::Update()
{
	if (status == Status::LIVE) {

		AlphaReduce();

		ren->SetVertexColor(m_color);
		ren->SetHeight(m_height);
		ren->SetWidth(m_width);
	}
}

void WingSmog::DrawObject()
{

}
