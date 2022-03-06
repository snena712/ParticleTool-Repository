#include "EnemyCountUI.h"
#include "../../../Component/SceneManager.h"
#include "../../../DX11System/mystring.h"

using namespace Component;

EnemyCountUI::EnemyCountUI(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "enecnt");

	tra = owner->transform;

	/*ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/Airport.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	ren->SetHeight(5);
	ren->SetWidth(5);
	ren->SetVertexColor(XMFLOAT4(1, 0, 0, 1));*/
}

EnemyCountUI::~EnemyCountUI()
{

}

void EnemyCountUI::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
	m_enemane = SceneManager::Instance()->GameObjectFindName("EnemyManager")->GetComponent<Component::EnemyManager>();
}

void EnemyCountUI::Update()
{
	PosUpdate();
}

void EnemyCountUI::DrawObject()
{
	String();
}

void EnemyCountUI::PosUpdate()
{
	tra->position = m_camera->transform->position + m_camera->transform->forward() * 51 - m_camera->transform->right() * 72
		+ m_camera->transform->up() * 48;
}

void EnemyCountUI::String()
{
	int s = m_enemane->GetLiveCount();
	std::string ui("ENEMY:"+std::to_string(s));

	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2-535,
		Application::Instance()->CLIENT_HEIGHT / 2-340 ,
		20, 20, XMFLOAT4(0, 1.0f, 0.0f, 0.7f), ui.c_str());
}