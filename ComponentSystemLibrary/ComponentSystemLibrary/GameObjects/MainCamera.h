#pragma once
#include "../Component/GameObject.h"
#include "../Component\CameraComponent.h"
#include "../DX11System/Controller.h"
#include "../DX11System/RandomClass.h"
#include "../source/Enemy/EnemyManager.h"
#include "../source/MainGameManager.h"

namespace Component {

	class MainCamera : public ComponentBase
	{
	public:
		enum class CameraType {
			FPS,
			TPS,
			DELAY,
			STATIC
		};

	private:
		Transform* tra;
		Component::Camera* cam;
		CameraType camtype = CameraType::TPS;	// カメラの種類
		Component::EnemyManager* enemana;

		int ControllerNum = -1; //コントローラー


		XMFLOAT3 pos;
		XMFLOAT3 g_startpos;

		//--------- 操作用------------//
		bool pressFlg = false;
		Vector3 m_pos;						// 場所のメイン
		Vector3 m_mouseRightPressPos;		// 左ボタン押した位置
		Vector3 m_mouseRightPressOldPos;	// 左ボタン押した位置


		XMFLOAT4 m_lookRoot;

		void CameraPos();
		void Input();

		void TPSCamera();

	public:
		MainCamera(GameObject* _obj);
		~MainCamera();

		void CameraUpdate();

		void Update();
		void Start();

		XMFLOAT4 GetLookRoot() { return m_lookRoot; }
	};
}
