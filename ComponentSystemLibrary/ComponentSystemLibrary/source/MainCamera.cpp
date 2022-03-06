#include "MainCamera.h"
#include "Component/SceneManager.h"
#include "Component/Scene.h"
#include "GoalClass.h"
#include "DX11System\input.h"
#include <array>
#define YPOS_OFFSET (3.0f)

XMFLOAT4X4 g_mtx;

MainCamera::MainCamera() {

	this->SetName("MainCamera");

	tra = this->transform;
	tra->position=Vector3{ 0.0f,0.0f,0.0f };
	tra->scale=Vector3{ 0.0f,0.0f,0.0f };
		
	cam = this->AddComponent<Component::Camera>();
	cam->SetMainCamera(cam);

	camtype = CameraType::TPS;	// カメラの種類
}

MainCamera::~MainCamera() {

}

void MainCamera::Update() {
		
	CameraUpdate();
	
	//コンポーネント更新
	GameObject::Update();

}

void MainCamera::Draw() {

	//コンポーネント描画
	GameObject::Draw();

}

void MainCamera::CameraUpdate()
{

	// プレイヤーが存在するか？
	if (player != nullptr)
	{
		// プレイヤー更新
		playertra = player->transform;
	}

	// 状態変化
	Input();

	// 位置の更新
	CameraPos();	
	
	// 視点の更新
}

// 初期化
void MainCamera::Start()
{
	if (!init) 
	{
		Component::Camera* cam = this->AddComponent<Component::Camera>();

		switch (camtype)
		{
		case MainCamera::CameraType::FPS:
			// カメラの位置をプレイヤーから取得
			player = GameObject::Find("player");
			playertra = player->transform;

			// ステージ取得
			stage = dynamic_cast<StageManagerClass*>(GameObject::Find("StageManger"));
			goalUi = dynamic_cast<GoalUIClass*>(GameObject::Find("GoalUI"));
			
			// FPSカメラの設定

			break;
		
		case MainCamera::CameraType::TPS:
			
			// カメラの位置をプレイヤーから取得
			player = GameObject::Find("player");
			playertra = player->transform;

			// ステージ取得
			stage = dynamic_cast<StageManagerClass*>(GameObject::Find("StageManger"));
			goalUi = dynamic_cast<GoalUIClass*>(GameObject::Find("GoalUI"));
			
			break;
		case MainCamera::CameraType::DELAY:
			// カメラの位置をプレイヤーから取得
			player = GameObject::Find("player");
			playertra = player->transform;

			// ステージ取得
			stage = dynamic_cast<StageManagerClass*>(GameObject::Find("StageManger"));
			goalUi = dynamic_cast<GoalUIClass*>(GameObject::Find("GoalUI"));
			break;

		case MainCamera::CameraType::STATIC:
			// カメラの位置をプレイヤーから取得
			player = GameObject::Find("player");
			playertra = player->transform;

			// ステージ取得
			stage = dynamic_cast<StageManagerClass*>(GameObject::Find("StageManger"));
			goalUi = dynamic_cast<GoalUIClass*>(GameObject::Find("GoalUI"));
			break;
		default:
			break;
		}
		init = true;
		
	}
}

// 位置の更新
void MainCamera::CameraPos()
{
	auto scene = SceneManager::Instance()->GetActiveScene()->GetSceneName();

	switch (camtype)
	{
	case MainCamera::CameraType::FPS:
		if (scene == "Title") {
			FPSCamera();
		}
		break;

	case MainCamera::CameraType::TPS:

		if (scene == "Title") {
		//	tra->position = Vector3(playertra->position.x, 10, playertra->position.z - 8);
			TPSCamera();
		}

		/*if (scene == "Map1_1Class") {
			tra->position = Vector3(playertra->position.x, 10, playertra->position.z - 8);
		}

		if (scene == "Map1_2Class") {
			tra->position = Vector3(playertra->position.x, 10, playertra->position.z - 8);
		}

		if (scene == "Map1_3Class") {
			tra->position = Vector3(playertra->position.x, 10, playertra->position.z - 8);
		}*/
		break;

	case MainCamera::CameraType::DELAY:
		DelayCamera();
		break;

	case MainCamera::CameraType::STATIC:
		UpdateSphereCamera(10, 10, 10, (DirectX::XMFLOAT3(playertra->position.x, playertra->position.y, playertra->position.z)));
		break;


	default:
		break;
	}
}

// 入力
void MainCamera::Input()
{
	if (GetKeyboardTrigger(DIK_F11))
	{
		// 状態変化
		switch (camtype)
		{
		case MainCamera::CameraType::FPS:
			camtype = CameraType::TPS;
			break;
		case MainCamera::CameraType::TPS:
			camtype = CameraType::DELAY;
			break;
		case MainCamera::CameraType::DELAY:
			camtype = CameraType::FPS;
			break;
		}
	}
}


// 視点の更新（現在使用してない）
void MainCamera::CameraLook()
{
	DirectX::XMFLOAT3 c_lookat{0,0,0};

	switch (camtype)
	{
	case MainCamera::CameraType::FPS:
		c_lookat = DirectX::XMFLOAT3(playertra->position.x, playertra->position.y , playertra->position.z+2);

		printf("%f %f %f",c_lookat.x, c_lookat.y, c_lookat.z);
		system("CLS");

		// ゴールの更新
		if (GoalCamera())
		{
			goalUi->Goal();
		}
		break;

	case MainCamera::CameraType::TPS:
		break;

	default:
		break;
	}

	// 視点が決まっていない時の仮処理
	if (c_lookat.x == 0 && c_lookat.y == 0 && c_lookat.z == 0) {
		c_lookat.z = 1;
	}

	//セットする
	cam->SetLookat(c_lookat);
}

// ゴールの更新処理
bool MainCamera::GoalCamera()
{	
	if (stage->GetGoalFlg()&&!once) 
	{
		once = true;

		// 開始位置の保存
		g_startpos.x = tra->position.x;			// X座標保存 
		g_startpos.y = tra->position.y;			// Y座標保存
		g_startpos.z = tra->position.z;			// Z座標保存
	}
	
	float t;
	if (once)
	{	
		if (g_nowrotcnt < g_rotcnt) {

			pos.x = g_startpos.x;
			pos.y = g_startpos.y;
			pos.z = g_startpos.z;

			XMFLOAT3 endpos;
			t = static_cast<float>(g_nowrotcnt + 1) / static_cast<float>(g_rotcnt);

			// 終了位置を計算
			endpos.y = g_startpos.y - 3 * 2;
			endpos.z = g_startpos.z + 1 * 2;

			// 線形補間の式でX座標を計算
			pos.y = g_startpos.y * (1.0f - t) + endpos.y * t;
			pos.z = g_startpos.z * (1.0f - t) + endpos.z * t;

			// 回転数をカウントアップ
			g_nowrotcnt++;
			tra->position.y = pos.y;
			tra->position.z = pos.z;
		}
		else {
			tra->position.y = pos.y;
			tra->position.z = pos.z;
			return true;
		}
		
	}
	return false;
}


// 角度をラジアンに変更
inline float ToRad(float angle) {
	return (angle * DirectX::XM_PI) / 180.0f;
}
  
// 球面束縛カメラ(Ｚ軸回転なしバージョン)
void MainCamera::UpdateSphereCamera(
	float radius,						// 半径(入力)
	float elevation,					// 仰角（入力）
	float azimuth,						// 方位角（入力）
	const DirectX::XMFLOAT3& lookat)	// 注視点（入力）
{

	DirectX::XMFLOAT3 eye;				// 視点

	// 仰角、方位角をラジアンに変換
	elevation = ToRad(elevation);
	azimuth = ToRad(azimuth);

	// 仰角に応じた半径を計算
	float r = radius * sinf(elevation);

	// 視点座標を計算
	eye.x = r * cosf(azimuth);
	eye.y = radius * cosf(elevation);
	eye.z = r * sinf(azimuth);

	// 上向きベクトルを計算(仰角をを９０度回転させるとできる)
	DirectX::XMFLOAT3 up;
	float elevationplus90 = elevation + ToRad(90.0f);			// 仰角を９０度足す

	r = radius * sinf(elevationplus90);

	up.x = r * cosf(azimuth);
	up.y = radius * cosf(elevationplus90);
	up.z = r * sinf(azimuth);

	// 視点ベクトルを注視点分平行移動
	eye.x += lookat.x;
	eye.y += lookat.y;
	eye.z += lookat.z;
	

	// ビュー変換行列を作成する
	cam->SetEye(eye);
	//tra->position = eye;
	tra->position.x = eye.x + player->transform->position.x;
	tra->position.y = eye.y + player->transform->position.y;
	tra->position.z = eye.z + player->transform->position.z-20;
	//cam->SetEye(tra->position);

	cam->SetLookat(lookat);
	cam->SetUp(up);
	cam->CreateCameraMatrix();
}

// FPSカメラ
void MainCamera::FPSCamera()
{
	// プレイヤーの行列取得
	XMFLOAT4X4 mtx;
	mtx = player->transform->GetMtx();
	// ビュー変換行列を変更
	static int svidx = 0;
	static bool firstflag = false;
	static std::array<XMFLOAT3, 100> zaxis;
	// Z軸
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;
	static std::array<XMFLOAT3, 100> yaxis;
	// Y軸
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;
	static std::array<XMFLOAT3, 100> pos;
	// 現在地
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;


	XMFLOAT3 eye;
	// 視点
	XMFLOAT3 lookat;
	// 注視点
	XMFLOAT3 up;
	// 上向きベクトル
	lookat = pos[svidx];
	eye.x = pos[svidx].x - 1 * zaxis[svidx].x;
	eye.y = pos[svidx].y - 1 * zaxis[svidx].y;
	eye.z = pos[svidx].z - 1 * zaxis[svidx].z;

	up = yaxis[svidx];

	cam->SetEye(eye);
	tra->position = eye;
	tra->position.x = eye.x;
	tra->position.y = eye.y;
	tra->position.z = eye.z;
	cam->SetLookat(lookat);
	cam->SetUp(up);

	printf("%f %f %f\n", mtx._41, mtx._42, mtx._43);
	cam->CreateCameraMatrix();
}

// TPSカメラ
void MainCamera::TPSCamera()
{
	// プレイヤーの行列取得
	XMFLOAT4X4 mtx;
	mtx = player->transform->GetMtx();
	// ビュー変換行列を変更
	static int svidx = 0;
	static bool firstflag = false;
	static std::array<XMFLOAT3, 100> zaxis;
	// Z軸
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;
	static std::array<XMFLOAT3, 100> yaxis;
	// Y軸
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;
	static std::array<XMFLOAT3, 100> pos;
	// 現在地
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;


	XMFLOAT3 eye;
	// 視点
	XMFLOAT3 lookat;
	// 注視点
	XMFLOAT3 up;
	// 上向きベクトル
	lookat = pos[svidx];
	eye.x = pos[svidx].x - 30 * zaxis[svidx].x;
	eye.y = pos[svidx].y - 30 * zaxis[svidx].y;
	eye.z = pos[svidx].z - 30 * zaxis[svidx].z;

	up = yaxis[svidx];

	cam->SetEye(eye);
	tra->position.x = eye.x;
	tra->position.y = eye.y;
	tra->position.z = eye.z;
	cam->SetLookat(lookat);
	cam->SetUp(up);

	printf("%f %f %f\n", mtx._41, mtx._42, mtx._43);
	cam->CreateCameraMatrix();
}

// 遅れて追従するカメラ
void MainCamera::DelayCamera()
{
	// プレイヤーの行列取得
	XMFLOAT4X4 mtx;
	mtx = player->transform->GetMtx();
	// ビュー変換行列を変更
	static int svidx = 0;
	static bool firstflag = false;
	static std::array<XMFLOAT3, 100> zaxis;
	// Z軸
	zaxis[svidx].x = mtx._31;
	zaxis[svidx].y = mtx._32;
	zaxis[svidx].z = mtx._33;
	static std::array<XMFLOAT3, 100> yaxis;
	// Y軸
	yaxis[svidx].x = mtx._21;
	yaxis[svidx].y = mtx._22;
	yaxis[svidx].z = mtx._23;
	static std::array<XMFLOAT3, 100> pos;
	// 現在地
	pos[svidx].x = mtx._41;
	pos[svidx].y = mtx._42;
	pos[svidx].z = mtx._43;
	svidx++;
	if (svidx > zaxis.size() - 1)
	{
		svidx = 0;
		firstflag = true;
	}
	// 30フレーム前の情報を使用してカメラを作成
	int useidx = svidx - 30;
	if (useidx < 0)
	{
		if (!firstflag)
		{
			useidx = 0;
		}
		else
		{
			useidx = svidx + zaxis.size() - 30;
		}
	}
	XMFLOAT3 eye;
	// 視点
	XMFLOAT3 lookat;
	// 注視点
	XMFLOAT3 up;
	// 上向きベクトル
	lookat = pos[useidx];

	eye.x = pos[useidx].x - 30 * zaxis[useidx].x;
	eye.y = pos[useidx].y - 30 * zaxis[useidx].y;
	eye.z = pos[useidx].z - 30 * zaxis[useidx].z;

	up = yaxis[useidx];

	cam->SetEye(eye);
	tra->position.x = eye.x;
	tra->position.y = eye.y;
	tra->position.z = eye.z;
	cam->SetLookat(lookat);
	cam->SetUp(up);

	printf("%f %f %f\n", mtx._41, mtx._42, mtx._43);
	cam->CreateCameraMatrix();
}


// 各カメラの位置に対する初期化　更新
// 固定カメラの導入

