#pragma once
#include "../Component/GameObject.h"
#include "../Component\CameraComponent.h"

class MainCamera :public GameObject
{
public:
	enum class CameraType{
		FPS,
		TPS,
		DELAY,
		STATIC
	};

private:
	 Transform* tra;
	 Component::Camera* cam;

	 CameraType camtype=CameraType::TPS;	// カメラの種類

	 GameObject* player;

	 Transform*  playertra;
	 bool init = false;
	 bool once = false;	// ゴール用
	 
	 int ControllerNum = -1; //コントローラー

	 XMFLOAT3 pos;
	 XMFLOAT3 g_startpos;
	 int nowcnt = 0;	// 現在のカウント
	 int g_nowrotcnt = 0;
	 const int g_rotcnt = 45;	// 終了までのカメラ移動カウント
	 bool GoalCamera();
	 void CameraPos();
	 void CameraLook();
	 void Input();

	 void UpdateSphereCamera(
		 float radius,					// 半径(入力)
		 float elevation,				// 仰角（入力）
		 float azimuth,					// 方位角（入力）
		 const DirectX::XMFLOAT3& lookat);		// 注視点（入力）

	 void FPSCamera();
	 void TPSCamera();
	 void DelayCamera();		// 遅延カメラ
	 void sssss();
public:
	MainCamera();
	~MainCamera();

	void CameraUpdate();

	void Update();
	void Draw();
	void Start();
};

