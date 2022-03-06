#pragma once
#include "../../Component/GameObject.h"
#include "../../Component\CameraComponent.h"
#include "../../source/StageManagerClass.h"
#include "../../source/GoalUIClass.h"
#include "../../source/BulletManager.h"
#include "../../DX11System/RandomClass.h"
#include "../../source/F1_PlayerObj.h"
#include "../../source/Enemy/EnemyManager.h"
#include "../../source/MainGameManager.h"
#include "../../Component/SceneManager.h"
#include "../../Component/Scene.h"
#include "../../source/GoalClass.h"
#include "../../DX11System/CDirectInput.h"
#include <array>

namespace Component {

	class SelectSceneCamera : public ComponentBase
	{
	public:
		enum class CameraType {
			ROTATE,
			STATIC
		};

	private:
		Transform* tra;
		Component::Camera* cam;
		Component::BulletManager* bul;
		Component::PlayerObj* playercompo;
		CameraType camtype = CameraType::ROTATE;	// カメラの種類
		Component::EnemyManager* enemana;

		GameObject* player;
		StageManagerClass* stage;
		GoalUIClass* goalUi;

		Transform*  playertra;
		bool once = false;	// ゴール用

		int ControllerNum = -1; //コントローラー


		XMFLOAT3 pos;
		XMFLOAT3 g_startpos;
		int nowcnt = 0;	// 現在のカウント
		int g_nowrotcnt = 0;
		const int g_rotcnt = 45;	// 終了までのカメラ移動カウント

		Vector3 m_pos;				// 場所のメイン

		//---------スタート用---------//
		int m_startcnt = 0;
		float azimuth = 210;				// 開始角度

		//--------加速時用変数---------//
		int m_nowrandcnt = 0;
		const int m_maxrandcnt = 45;		// カメラのぶれる感覚
		Vector3 m_randpos;					// ぶれの位置
		const float m_poslength = 23;		// プレイヤーとカメラの距離の上限



		void Input();

		void UpdateSphereCamera(
			float radius,					// 半径(入力)
			float elevation,				// 仰角（入力）
			float azimuth,					// 方位角（入力）
			const DirectX::XMFLOAT3& lookat);		// 注視点（入力）

		void CameraPos();			// カメラの位置

		void FPSCamera();
		void TPSCamera();
		void DelayCamera();

		void TPSRotateCamera( float _pluscnt);		// 開始時演出用
		void ShakeCamera();								// カメラのぶれ
		void DeadCamera();								// 死亡時用

	public:
		SelectSceneCamera(GameObject* _obj);
		~SelectSceneCamera();

		void CameraUpdate();

		void Update();
		void Start();
	};
}
