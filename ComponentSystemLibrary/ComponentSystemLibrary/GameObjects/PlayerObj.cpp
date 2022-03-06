#include "PlayerObj.h"
#include "../Component/QuadRenderComponent.h"
#include "../Component/ModelComponent.h"

PlayerObj::PlayerObj()
{
	this->SetName("player");

	transform->position = Vector3(0.0f, 0.0f, 0.0f);
	transform->scale = Vector3(1.0f, 1.0f, 1.0f);

	this->AddComponent<Component::ModelComponent>(
		"assets/fbx/Star.fbx",	// モデルデータ			// カレントファイルからの読み込み
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/ps.hlsl",			// ピクセルシェーダー
		"assets/fbx/tex/");			// テクスチャの存在フォルダ

	this->AddComponent<Component::QuadRender>("assets/dice/RED_SCISSOR.png");
	this->GetComponent<Component::QuadRender>()->SetHeight(30.0f);
	this->GetComponent<Component::QuadRender>()->SetWidth(30.0f);
}

PlayerObj::~PlayerObj()
{
	for (auto& comList : componentList) {

		if (comList != nullptr) {
			delete comList;
		}
	}
}

void  PlayerObj::Update()
{
	Start();

	//コンポーネント更新
	GameObject::Update();
}

void  PlayerObj::Draw()
{
	//コンポーネント描画
	GameObject::Draw();
}

void PlayerObj::Start()
{

}
