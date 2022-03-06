#include "EnemyWingSmogBase.h"
#include "SmogPlayer.h"

using namespace Component;

EnemyWingSmogBase::EnemyWingSmogBase(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("EnemyWingSmogBase");

	ren = owner->AddComponent<Component::QuadRender>("assets/white.bmp");
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0.9f));
}

EnemyWingSmogBase::~EnemyWingSmogBase()
{
}

void EnemyWingSmogBase::Start()
{

}

void EnemyWingSmogBase::Update()
{
}


