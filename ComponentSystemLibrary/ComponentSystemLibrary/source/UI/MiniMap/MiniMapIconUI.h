#pragma once
#include "../../../Component/GameObject.h"
#include "../../../Component/Quad2DRenderComponent.h"

namespace Component {

	class MiniMapIconUI :public ComponentBase
	{
	public:
		enum class ObjType
		{
			Player,
			Enemy
		};

	private:
		Component::Quad2DRender* ren;
		Transform* targetTra;

		ObjType m_objType;
		float m_distance = 0;
		float m_degree = 0;

	public:
		MiniMapIconUI(GameObject* _obj);

		void SetObjType(ObjType _objtype);
		void SetDistance(float _distance) { m_distance = _distance; }
		void SetDegree(float _degree) { m_degree = _degree; }
		void SetTransform(Transform* _tra) { targetTra = _tra; }
	};
}
