#include "PlayerJatParts.h"
#include "../../Component/SceneManager.h"
#include "../../DX11System/mystring.h"

using namespace Component;

PlayerJatParts::PlayerJatParts(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("PlayerJatParts");

	tra = owner->transform;

	// 爆破アニメーション
	model = owner->AddComponent<Component::ModelRender>(
		"assets/F2_v2/f2a_ohane.fbx",		// モデルデータ			// カレントファイルからの読み込み
		"shader/vs.hlsl",						// 頂点シェーダー
		"shader/ps.hlsl",						// ピクセルシェーダー
		"assets/F2_v2/f2a_texture/");			// テクスチャの存在フォルダ
}

PlayerJatParts::~PlayerJatParts()
{
}


void PlayerJatParts::Start()
{
	player = SceneManager::Instance()->GameObjectFindName("player");

}

void PlayerJatParts::Update()
{
	//RotUpdate();
}

// 角度の設定
void PlayerJatParts::SetRot(int _num)
{
	if (m_rot < m_maxrot && m_rot > -m_maxrot)
	{
		m_rot += _num;
		m_rotflg = true;
		owner->transform->RotateLocalXaxis(_num);
	}
}

// 角度の更新
void PlayerJatParts::RotUpdate()
{
	if (!m_rotflg)
	{
		if (m_rot > 0)
		{
			m_rot--;
			owner->transform->RotateLocalXaxis(-m_rotreturn);
		}
		if (m_rot < 0)
		{
			m_rot++;
			owner->transform->RotateLocalXaxis(m_rotreturn);
		}
	}
	else {
		m_rotflg = false;
	}
}


// newとデリーとしたくないから最初から初期化しとく