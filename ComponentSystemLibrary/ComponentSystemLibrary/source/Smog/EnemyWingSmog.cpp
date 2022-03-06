#include "EnemyWingSmog.h"

using namespace Component;

EnemyWingSmog::EnemyWingSmog(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("EnemyWingSmog");

	// 
	lwing= new GameObject();
	lwing->AddComponent<Component::EnemyWingSmogBase>();
	l_ren =lwing->GetComponent<Component::QuadRender>();
	lwing->transform->RotateXaxis(90);
	lwing->transform->SetParent(owner->transform);
	lwing->SetExistState(false);

	rwing = new GameObject();
	rwing->AddComponent<Component::EnemyWingSmogBase>();
	r_ren = rwing->GetComponent<Component::QuadRender>();
	rwing->transform->RotateXaxis(90);
	rwing->transform->RotateZaxis(90);
	rwing->transform->SetParent(owner->transform);
	rwing->SetExistState(false);


	// ƒŠƒXƒg‚É’Ç‰Á
	SceneManager::Instance()->AddObject(lwing, false);
	SceneManager::Instance()->AddObject(rwing, false);
}

EnemyWingSmog::~EnemyWingSmog()
{
}

void EnemyWingSmog::Start()
{

}

void EnemyWingSmog::Update()
{
	if (status == Status::LIVE) {

		AlphaReduce();
	}
}

void EnemyWingSmog::AlphaReduce() {
	
	if (owner->GetExistState())
	{
		if (status == Status::LIVE) {
			r_ren->SetVertexColor(m_color);
			r_ren->SetHeight(m_height);
			r_ren->SetWidth(m_width);

			l_ren->SetVertexColor(m_color);
			l_ren->SetHeight(m_height);
			l_ren->SetWidth(m_width);

			m_color.w -= m_reduce;
			m_width += m_plussize.x;
			m_height += m_plussize.y;

			rwing->SetExistState(true);
			lwing->SetExistState(true);
		}

		if (m_color.w < 0)
		{
			r_ren->SetHeight(0);
			r_ren->SetWidth(0);

			l_ren->SetHeight(0);
			l_ren->SetWidth(0);

			status = Status::DEAD;
			owner->SetExistState(false);
		}
	}
}

