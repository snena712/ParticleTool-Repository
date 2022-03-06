#include "SelectSceneCamera.h"

#define YPOS_OFFSET (3.0f)

using namespace Component;


SelectSceneCamera::SelectSceneCamera(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("MainCamera");

	tra = owner->transform;
	tra->position = Vector3{ 0.0f,0.0f,0.0f };
	tra->scale = Vector3{ 0.0f,0.0f,0.0f };

	cam = owner->AddComponent<Component::Camera>();
	cam->SetMainCamera(cam);

	cam->SetFar(10000);

	m_pos = Vector3(0, 0, 6.25f);

}

SelectSceneCamera::~SelectSceneCamera() {

}

void SelectSceneCamera::Update() {

	CameraUpdate();
}

void SelectSceneCamera::CameraUpdate()
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
void SelectSceneCamera::Start()
{
	// カメラの位置をプレイヤーから取得
	player = SceneManager::Instance()->GameObjectFindName("player");
	playertra = player->transform;
	playercompo = player->GetComponent<Component::PlayerObj>();


	TPSRotateCamera(0.5f);
}

// 位置の更新
void SelectSceneCamera::CameraPos()
{
	switch (camtype)
	{
	case SelectSceneCamera::CameraType::ROTATE:

		TPSRotateCamera(0.3f);
		break;

	case SelectSceneCamera::CameraType::STATIC:
		TPSCamera();
		break;

	default:
		break;
	}
}

// 入力
void SelectSceneCamera::Input()
{

}

// 角度をラジアンに変更
inline float ToRad(float angle) {
	return (angle * DirectX::XM_PI) / 180.0f;
}

// 球面束縛カメラ(Ｚ軸回転なしバージョン)
void SelectSceneCamera::UpdateSphereCamera(
	float radius,						// 半径(入力)
	float elevation,					// 仰角（入力）
	float azimuth,						// 方位角（入力）
	const XMFLOAT3& lookat)	// 注視点（入力）
{
	//DirectX::XMFLOAT3 eye;				// 視点

	//// 仰角、方位角をラジアンに変換
	//elevation = ToRad(elevation);
	//azimuth = ToRad(azimuth);

	//// 仰角に応じた半径を計算
	//float r = radius * sinf(elevation);

	//// 視点座標を計算
	//eye.x = r * cosf(azimuth);
	//eye.y = radius * cosf(elevation);
	//eye.z = r * sinf(azimuth);

	//// 上向きベクトルを計算(仰角をを９０度回転させるとできる)
	//DirectX::XMFLOAT3 up;
	//float elevationplus90 = elevation + ToRad(90.0f);			// 仰角を９０度足す

	//r = radius * sinf(elevationplus90);

	//up.x = r * cosf(azimuth);
	//up.y = radius * cosf(elevationplus90);
	//up.z = r * sinf(azimuth);

	//// 視点ベクトルを注視点分平行移動
	//eye.x += lookat.x;
	//eye.y += lookat.y;
	//eye.z += lookat.z;

	////tra->position = eye;
	//tra->position.x = eye.x + player->transform->position.x;
	//tra->position.y = eye.y + player->transform->position.y;
	//tra->position.z = eye.z + player->transform->position.z - 20;
	////cam->SetEye(tra->position);

	//this->transform->LookAt(lookat);
	//cam->CreateCameraMatrix();
}

// FPSカメラ
void SelectSceneCamera::FPSCamera()
{
}

// TPSカメラ
void SelectSceneCamera::TPSCamera()
{
	// 場所
	//tra->position = player->transform->position - player->transform->forward() * 50.0f;

	tra->position = Vector3(0, 0, 6);
	// クオータニオンを入れる　
	cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());

	// lerp使う
}

// 遅れて追従するカメラ
void SelectSceneCamera::DelayCamera()
{
	static int svidx = 0;
	static bool firstflag = false;

	static std::array<XMFLOAT4, 100> qt;
	qt[svidx] = player->transform->qt;

	static std::array<Vector3, 100> forward;
	forward[svidx] = player->transform->forward();

	static std::array<Vector3, 100> up;
	up[svidx] = player->transform->up();

	static std::array<Vector3, 100> right;
	right[svidx] = player->transform->right();

	static std::array<Vector3, 100>pos;
	pos[svidx] = player->transform->position;


	svidx++;
	if (svidx > qt.size() - 1)
	{
		svidx = 0;
		firstflag = true;
	}
	// 30フレーム前の情報を使用してカメラを作成
	int useidx = svidx - 2;
	if (useidx < 0)
	{
		if (!firstflag)
		{
			useidx = 0;
		}
		else
		{
			useidx = svidx + qt.size() - 2;
		}
	}


	// 場所
	float length = 30;

	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;

	Vector3 inputVector;
	Vector3 _pos;

	// 
	inputVector.x = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRX) / 362.0f;
	inputVector.y = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY) / 362.0f;
	float yinputVector = (Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY) / 182.0f;

	//inputVector.xをインとに直して掛けたり割ったりしてする
	float backx = inputVector.x;
	if (inputVector.x < 5 && -5 < inputVector.x) { inputVector.x = 0; }
	int xxx = inputVector.x;
	double radi = xxx * 3.141592653589793f / 360; _pos.x = length * sin(radi);

	if (inputVector.y < 5 && -5 < inputVector.y) { inputVector.y = 0; }
	// todo ｙの値一定以上の時にする
	radi = inputVector.y * 3.141592653589793f / 360;
	//if (radi < 0.75f)radi = 0.75f;

	_pos.y = length * sin(radi);

	float firstleng = 1;			// 初期長さ
	const float confirstleng = 18;	// 固定の初期長さ

	if (inputVector.x == 0 && inputVector.y == 0)
	{
		firstleng = confirstleng;
	}

	float z = 0;
	float w = 0;
	// クオータニオンを入れる　
	cam->GetOwner()->transform->qt = qt[useidx];

	if ((Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER))
	{

		// 左右入力に対するｚ座標の補正　
		if (_pos.x < 0)
		{
			z = _pos.x + 30;
		}
		else {
			z = 30 - _pos.x;
		}

		// 入力がないとき計算のバグをなくす
		if (_pos.x == 0)
		{
			_pos.x = 0.01f;
		}
		if (_pos.y == 0)
		{
			_pos.y = 0.01f;
		}

		//======== カメラの回転=========== //
		cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());

		// 入力なしの初期値の時
		if (firstleng != confirstleng) {
			tra->position = pos[useidx]
				- forward[useidx] * _pos.y
				+ up[useidx] * 6
				- right[useidx] * _pos.x;
		}
		else {
			tra->position = pos[useidx]
				- forward[useidx] * firstleng
				+ up[useidx] * 6
				- right[useidx] * _pos.x;
		}
		Vector3 a = right[useidx] * _pos.x;
		//printf("%f %f %f \n",a.x,a.y,a.z );
	}
	else {

		if (playercompo->GetStreatFlg())
		{
			ShakeCamera();
		}


		Vector3 _pos = m_pos + m_randpos;

		// 通常時
		tra->position = pos[useidx]
			- forward[useidx] * _pos.z + up[useidx] * (_pos.y) + tra->right() * _pos.x;
	}
}


// スタートの演出用
void SelectSceneCamera::TPSRotateCamera(float _pluscnt)
{
	Vector3 eye;				// 視点
	float radius = m_pos.z;					// 半径(入力)
	float elevation = 75;					// 仰角（入力）
	float _azimuth;						// 方位角（入力）
	Vector3 lookat = player->transform->position;	// 注視点（入力）

	azimuth += _pluscnt;
	// 仰角、方位角をラジアンに変換
	elevation = ToRad(elevation);
	_azimuth = ToRad(azimuth);

	// 仰角に応じた半径を計算
	float r = radius * sinf(elevation);

	// 視点座標を計算
	eye.x = r * cosf(_azimuth);
	eye.y = radius * cosf(elevation);
	eye.z = r * sinf(_azimuth);

	// 視点ベクトルを注視点分平行移動
	eye.x += lookat.x;
	eye.y += lookat.y;
	eye.z += lookat.z;

	tra->position = eye;

	cam->GetOwner()->transform->LookAt(lookat, player->transform->up());

}

// 画面のブレ
void SelectSceneCamera::ShakeCamera()
{
	// 前に動いたか
	if (m_maxrandcnt > m_nowrandcnt)
	{
		m_nowrandcnt++;
		m_randpos.x = RandomClass::GetRandom(-0.1f, 0.1f);
		m_randpos.y = RandomClass::GetRandom(-0.1f, 0.1f);
	}
	else {
		m_nowrandcnt = 0;
		m_randpos = Vector3(0, 0, 0);
	}

}

// 死亡時用
void  SelectSceneCamera::DeadCamera()
{
	cam->GetOwner()->transform->LookAt(player->transform->position, owner->transform->up());
}


//　スラープにする？　滑らかにしたい

// 前高さを変数で管理


//角度がみぎ

// いちぶUIも揺れた