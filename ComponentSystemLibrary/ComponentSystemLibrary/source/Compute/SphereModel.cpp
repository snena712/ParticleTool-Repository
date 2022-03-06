#include "SphereModel.h"
#include "../../DX11System/DX11Settransform.h"

using namespace Component;

SphereModel::SphereModel(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("player");

	model = owner->AddComponent<Component::ModelRenderInstance>(
		"assets/Model/sphere.fbx",		// モデルデータ			// カレントファイルからの読み込み
		"shader/vs3dtest.hlsl",				// 頂点シェーダー
		"shader/ps3dtest.hlsl",				// ピクセルシェーダー
		"");		// テクスチャの存在フォルダ
}

SphereModel::~SphereModel()
{}

//-----------------------------
// create by Suehara
//-----------------------------
