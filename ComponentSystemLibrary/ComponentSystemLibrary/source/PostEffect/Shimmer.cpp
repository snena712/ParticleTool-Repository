#include "Shimmer.h"
#include "../../DX11System\ImageResourceManager.h"
#include "../../DX11System\DX11Settransform.h"
#include "../../Component\CameraComponent.h"
#include "DirectGraphicsTexture.h"
#include "../../Component/SceneManager.h"

using namespace Component;

Shimmer::Shimmer(GameObject * _owner) :ComponentBase(_owner)
{
	owner->SetName( "noise");

	owner->transform->position = Vector3(0, 5, 0);

	post=owner->AddComponent<Component::PostEffectComponent>(
		"assets/Image/Noise/noise.jpg","assets/Image/Noise/alpha.jpg","shader/Shimmervs.hlsl", "shader/Shimmerps.hlsl");
	
	
	for (int i = 0; i < 2; i++) 
	{
		poslist.emplace_back();
		poslist[i] = Vector3(0, 0, 0);
	}
}

Shimmer::~Shimmer()
{

}
void Shimmer::Update()
{
}

void Shimmer::Start()
{
	player = SceneManager::Instance()->GameObjectFindName("player");
}

void Shimmer::DrawObject()
{
	// 描画
	owner->transform->position = player->transform->position - player->transform->forward() * 18- player->transform->up() * 2;

	State();

	// 場所  テストコメント中
	post->SetPosList(&poslist);
}

void Shimmer::State()
{
	poslist[0] = player->transform->position - player->transform->forward() * 12 - player->transform->right() * 2.2f;
	poslist[1] = player->transform->position - player->transform->forward() * 12 + player->transform->right() * 2.2f;

	post->SetHeight(5);
	post->SetWidth(5);
}

