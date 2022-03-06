#include "ArrowUI.h"

using namespace Component;

ArrowUI::ArrowUI(GameObject* _owner) : ComponentBase(_owner)
{	
}

ArrowUI::~ArrowUI()
{}

void ArrowUI::DrawLine(XMFLOAT3 _pos)
{
	std::vector<Line::MyVertex> v;
	v.resize(2);

	
	Vector3 pos = Vector3(_pos.x, _pos.y, _pos.z);
	
	float length = 20;

	if (first) {
		// èâä˙âª
		v[0].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);
		v[1].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);

		v[0].color = v[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);


		g_line.Init(v);

		first = false;
	}
	// 1
	v[0].vertex.x = pos.x ;
	v[0].vertex.y = pos.y + 0 * -length / 2.0f;
	v[0].vertex.z = pos.z + 0 * -length / 2.0f;

	v[1].vertex.x = pos.x + 1 * length / 2.0f;
	v[1].vertex.y = pos.y + 0 * length / 2.0f;
	v[1].vertex.z = pos.z + 0 * length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	g_line.SetVertex(v);
	g_line.Draw();

	// 2
	v[0].vertex.x = pos.x + 0 * -length / 2.0f;
	v[0].vertex.y = pos.y ;
	v[0].vertex.z = pos.z + 0 * -length / 2.0f;

	v[1].vertex.x = pos.x + 0 * length / 2.0f;
	v[1].vertex.y = pos.y + 1 * length / 2.0f;
	v[1].vertex.z = pos.z + 0 * length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(0.0f, 0.8f, 0.2f, 0.9f);
	g_line.SetVertex(v);
	g_line.Draw();

	// 3
	v[0].vertex.x = pos.x + 0 * -length / 2.0f;
	v[0].vertex.y = pos.y + 0 * -length / 2.0f;
	v[0].vertex.z = pos.z ;

	v[1].vertex.x = pos.x + 0 * length / 2.0f;
	v[1].vertex.y = pos.y + 0 * length / 2.0f;
	v[1].vertex.z = pos.z + 1 * length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(0.0f, 0.0f, 0.8f, 0.9f);
	g_line.SetVertex(v);
	g_line.Draw();
}



//-----------------------------
// create by Suehara
//-----------------------------