#include "EnemyRoot.h"

using namespace Component;

EnemyRoot::EnemyRoot(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("YButtunUI");

	tra = owner->transform;

	// 爆破アニメーション
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/arrow.png");

	ren->SetHeight(1);
	ren->SetWidth(1);

	owner->AddComponent<Component::StartUIColorComponent>();
	owner->GetComponent<Component::StartUIColorComponent>()->SetType(Component::StartUIColorComponent::RnderState::NORMAL);
}

EnemyRoot::~EnemyRoot()
{

}

void EnemyRoot::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");

}

void EnemyRoot::Update()
{
	PosUpdate();
}

void EnemyRoot::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 2 + m_camera->transform->up() * 1;
	
	tra->qt = m_camera->GetComponent<Component::MainCamera>()->GetLookRoot();
	tra->RotateLocalXaxis(90);
}


