#include "BoxFrame.h"


using namespace Component;

BoxFrame::BoxFrame(GameObject* _owner) : ComponentBase(_owner)
{
	// 爆破アニメーション
	model = owner->AddComponent<Component::ModelRender>(
		"assets/Model/Frame/BoxFrame.fbx",		// モデルデータ			// カレントファイルからの読み込み
		"shader/basicvs.hlsl",						// 頂点シェーダー
		"shader/basicps.hlsl",						// ピクセルシェーダー
		"");			// テクスチャの存在フォルダ
}

BoxFrame::~BoxFrame()
{}

void BoxFrame::DrawObject()
{
}

void BoxFrame::Update()
{

}

