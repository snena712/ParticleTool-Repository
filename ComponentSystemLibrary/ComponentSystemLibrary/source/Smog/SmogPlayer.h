#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SceneManager.h"
namespace Component
{
	class SmogPlayer :public ComponentBase
	{
	public:
		// ê∂ë∂èÛë‘
		enum class Status
		{
			LIVE,
			DEAD
		};

	public:
		enum class ObjType
		{
			Player,
			Enemy
		};

	private:
		Component::QuadRender* ren;
		Transform* targetTra;

		XMFLOAT4 m_color ;
		float m_height;
		float m_width;
		float m_reduce = 0;
		Vector3 m_plussize;
		Status status = Status::DEAD;
		ObjType m_Type;

		void AlphaReduce();
	public:
		SmogPlayer(GameObject* _obj);
		~SmogPlayer();
		void Start();
		void Update();

		Status GetStatus() { return status; }
		void SetStatus(Status _sta) { status = _sta; };
		void SetAlpha(XMFLOAT4 _hoge) { m_color = _hoge; }
		void SetSize(float _width, float _height) { m_width = _width; m_height = _height; }
		void SetReduce(float _num) { m_reduce = _num; }
		void SetPlusSize(Vector3 _plussize) { m_plussize = _plussize; }
		void SetType(ObjType _type);
	};

}