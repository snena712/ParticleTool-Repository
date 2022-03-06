#pragma once
#include "../../Component/QuadRenderComponent.h"
#include "../../Component/SceneManager.h"
namespace Component
{
	class EnemyWingSmogBase :public ComponentBase
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


	public:
		EnemyWingSmogBase(GameObject* _obj);
		~EnemyWingSmogBase();
		void Start();
		void Update();

	};

}