#include "WindowBullet.h"
#include "../../../Component/SceneManager.h"
#include "../../../DX11System/mystring.h"

using namespace Component;

WindowBullet::WindowBullet(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName( "WindowBullet");

	tra = owner->transform;

	ren = owner->AddComponent<Component::QuadRender>("assets/Image/UI/WindowBullet.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);

	ren->SetHeight(15);
	ren->SetWidth(25);
	ren->SetVertexColor(XMFLOAT4(1, 1, 1, 0.5f));
}

WindowBullet::~WindowBullet()
{

}

void WindowBullet::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
	m_bulletmana = SceneManager::Instance()->GameObjectFindName("BulletManager")->GetComponent<Component::BulletManager>();
}

void WindowBullet::Update()
{
	PosUpdate();
}

void WindowBullet::DrawObject()
{
	DrawString();
}

// 位置更新
void WindowBullet::PosUpdate()
{
	tra->position = m_camera->transform->position - m_camera->transform->up() * 41.5f+ m_camera->transform->forward() * 50 + m_camera->transform->right() * 61;
}

// 文字描画
void WindowBullet::DrawString()
{
	std::string num("1/" + std::to_string(m_bulletmana->GetHomigNum()));

	switch (m_bulletmana->GetBulletType())
	{
	case BulletManager::BulletType::Bullet:
		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 370,
			Application::Instance()->CLIENT_HEIGHT / 2 + 270,
			50, 50, XMFLOAT4(0, 1.0f, 0.2f, 1.0f), "-/- ");
		break;

	case BulletManager::BulletType::Homing:
		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 370,
			Application::Instance()->CLIENT_HEIGHT / 2 + 270,
			50, 50, XMFLOAT4(0, 1.0f, 0.2f, 1.0f), num.c_str());

		break;
	default:
		break;
	}
}

// newとデリーとしたくないから最初から初期化しとく