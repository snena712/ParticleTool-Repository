#include "TileLine.h"

using namespace Component;

#define Tile (40)
#define TilePosMax (200)

Line g_line2;

TileLine::TileLine(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("TileLine");

	ren = owner->AddComponent<Component::QuadRender>("assets/Image/Smog/RealSmog.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetWidth(0);
	ren->SetHeight(0);

}

TileLine::~TileLine()
{
}

void TileLine::Start()
{

}

void TileLine::Update()
{
}

void TileLine::DrawObject()
{
//XMFLOAT4X4 mtx;
	//DX11MakeWorldMatrix(mtx,Vector3(1,1,1),)

	// ü‰Šú‰»
	std::vector<Line::MyVertex> v;
	v.resize(2);

	static bool first = true;	
	Vector3 pos;
	float length = 400;
	int tileLength = TilePosMax;

	if (first) {
		// ‰Šú‰»
		v[0].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);
		v[1].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);

		v[0].color = v[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


		g_line2.Init(v);

		first = false;
	}


	pos.z = -TilePosMax;
	for (int i = 0; i < Tile; i++)
	{
		pos.z += TilePosMax * 2 / Tile;

		v[0].vertex.x = pos.x + 1 * -length / 2.0f;
		v[0].vertex.y = pos.y + 0 * -length / 2.0f;
		v[0].vertex.z = pos.z + 0 * -length / 2.0f;

		v[1].vertex.x = pos.x + 1 * length / 2.0f;
		v[1].vertex.y = pos.y + 0 * length / 2.0f;
		v[1].vertex.z = pos.z + 0 * length / 2.0f;

		if (Tile / 2 == i-1) {
			v[0].color = v[1].color = XMFLOAT4(0.8f, 0.0f, 0.0f, 0.12f);
		}
		else {
			v[0].color = v[1].color = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.12f);
		}

		g_line2.SetVertex(v);
		g_line2.Draw();
	}

	//// Y²
	//v[0].vertex.x = pos.x + 0 * -length / 2.0f;
	//v[0].vertex.y = pos.y + 1 * -length / 2.0f;
	//v[0].vertex.z = pos.z + 0 * -length / 2.0f;

	//v[1].vertex.x = pos.x + 0 * length / 2.0f;
	//v[1].vertex.y = pos.y + 1 * length / 2.0f;
	//v[1].vertex.z = pos.z + 0 * length / 2.0f;

	//v[0].color = v[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	//g_line2.SetVertex(v);
	//g_line2.Draw();

	
	pos.x = -TilePosMax;
	pos.z = 0;
	
	for (int i = 0; i < Tile; i++)
	{
		pos.x += TilePosMax * 2 / Tile;
		// Z²
		v[0].vertex.x = pos.x + 0 * -length / 2.0f;
		v[0].vertex.y = pos.y + 0 * -length / 2.0f;
		v[0].vertex.z = pos.z + 1 * -length / 2.0f;

		v[1].vertex.x = pos.x + 0 * length / 2.0f;
		v[1].vertex.y = pos.y + 0 * length / 2.0f;
		v[1].vertex.z = pos.z + 1 * length / 2.0f;

		if (Tile / 2 == i+1) {
			v[0].color = v[1].color = XMFLOAT4(0.0f, 0.8f, 0.0f, 0.12f);
		}
		else {
			v[0].color = v[1].color = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.12f);
		}

		g_line2.SetVertex(v);
		g_line2.Draw();
	}
}