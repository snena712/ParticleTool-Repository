#pragma once
#include "../../../Component/QuadRenderComponent.h"
#include "../../line.h"


namespace Component {
	class ArrowUI :public ComponentBase
	{
	private:
		Component::QuadRender* ren;

		Line g_line;
		const float m_lineLength = 10;
		bool first = true;
	public:
		ArrowUI(GameObject* _obj);
		~ArrowUI();

		void DrawLine(XMFLOAT3 _pos);
	};
}

//-----------------------------
// create by Suehara
//-----------------------------