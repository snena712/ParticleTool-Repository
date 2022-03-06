#include "MiniMapBackUI.h"
#include "../../../Component/SceneManager.h"
#include "../../../DX11System/mystring.h"

using namespace Component;

MiniMapBackUI::MiniMapBackUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("enecnt");

	tra = owner->transform;

	// ”š”jƒAƒjƒ[ƒVƒ‡ƒ“
	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/raderback.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	ren->SetHeight(20);
	ren->SetWidth(20);
	
}

MiniMapBackUI ::~MiniMapBackUI()
{

}

void MiniMapBackUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
	m_enemane = SceneManager::Instance()->GameObjectFindName("EnemyManager")->GetComponent<Component::EnemyManager>();
}

void MiniMapBackUI::Update()
{
	PosUpdate();
	//‰ñ“]
	owner->transform->RotateYaxis(rot);
	rot++;
}

void MiniMapBackUI::PosUpdate()
{
	tra->position =
		m_camera->transform->position + m_camera->transform->forward() * 50.1f - m_camera->transform->up() * 37 - m_camera->transform->right() * 100;
	//tra->position =
	//	m_camera->transform->position + m_camera->transform->forward() * 50.1f - m_camera->transform->up() * 37 - m_camera->transform->right() * 65;
}

