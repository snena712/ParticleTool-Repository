#include "PlayerWing.h"


using namespace Component;

PlayerWing::PlayerWing(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("PlayerWing");

	

	// 爆破アニメーション
	 owner->AddComponent<Component::ModelRender>(
		"assets/F2_v2/f2a_hane.fbx",	// モデルデータ			// カレントファイルからの読み込み
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/ps.hlsl",			// ピクセルシェーダー
		"assets/F2_v2/f2a_texture/");			// テクスチャの存在フォルダ
}

PlayerWing::~PlayerWing()
{

}



