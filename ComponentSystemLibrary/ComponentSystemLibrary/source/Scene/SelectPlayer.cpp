#include "SelectPlayer.h"
#include "../../DX11System\quad.h"
#include "../../DX11System\CDirectxGraphics.h"
#include "../../DX11System\Shader.h"
#include "../../DX11System/CDirectInput.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../DX11System\ImageResourceManager.h"
#include "../../Component/SphereColliderComponent.h"
#include "../../Component/BoxColliderComponent.h"
#include "XAudio2.h"
#include "../../DX11System/XAudio2.h"
#include "../../Component\ImGuiComponent.h"
#include "../../Component/SceneManager.h"
#include "../drawaxis.h"
#include "../../DX11System/Controller.h"
#include "../../DX11System/mystring.h"
#include "../../DX11System/XAudio2.h"
#include "../MainGameManager.h"
using namespace Component;

SelectPlayer::SelectPlayer(GameObject *_owner) : ComponentBase(_owner)
{
	// 
	owner->SetName("player");

	tra = owner->transform;

	tra->position = Vector3(5, 5, 0);

	owner->AddComponent<Component::Rigidbody>();
	rb = owner->GetComponent<Component::Rigidbody>();

	model = owner->AddComponent<Component::ModelRender>(
		"assets/F2/f2a_game_v2.fbx",	// モデルデータ			// カレントファイルからの読み込み
		"shader/vs.hlsl",				// 頂点シェーダー
		"shader/ps.hlsl",				// ピクセルシェーダー
		"assets/F2/f2a_texture/");		// テクスチャの存在フォルダ

	fade = new GameObject();
	fade->AddComponent<Component::FadeManager>();
	SceneManager::Instance()->AddObject(fade, false);
	fade->GetComponent<Component::FadeManager>()->SetOutScene(60);
	//各オブジェクト用意

	MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::SELECT);
}

SelectPlayer::~SelectPlayer()
{
}

void  SelectPlayer::Update()
{

	Input();

	// 回転
	if (ControllerNum != -1) {
		Controller();
	}
	else
	{
		KeyBord();
	}	

	InputLimit();
	
	ColorChange();

	SelectUpdate();
	
	StrLength();
	
	LinePos();
}

void  SelectPlayer::DrawObject()
{
	String();
}

void SelectPlayer::Start()
{
	ControllerNum = Controller::Instance()->GetControllerNum();

	line = SceneManager::Instance()->GameObjectFindName("SelectUnderbarUI");

	SetChangeFlg(ColorMODE::UP);
}

// 入力
void SelectPlayer::Input()
{

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_SPACE)) {
		if (!CheckPlaying(SOUND_LABEL_SE000))
		{
			PlaySound(SOUND_LABEL_SE000);
		}
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_V)) {

	}
}


void  SelectPlayer::KeyBord()
{

	if (m_type == InputType::VERTEX)
	{
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP)) {

			m_inputnum--;
		}
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN)) {

			m_inputnum++;
		}
	}
	else
	{
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_LEFT)) {

			m_inputnum--;
		}

		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RIGHT)) {

			m_inputnum++;
		}
	}
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_W)) {
		m_inputflg = true;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_S)) {

	}


}


void SelectPlayer::Controller()
{
	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;

	//アナログ方向キー
	// 左
	Vector3 inputVector;
	if (!m_inputflg) {
		inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLX / 360.0f;
		inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLY / 360.0f;
	}
	//// 右
	//inputVector;

	//inputVector.x = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRX / 365.0f;
	//inputVector.y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbRY / 365.0f;

	if (m_type == InputType::VERTEX)
	{
		if (inputVector.y > 15) {
			if (!m_plessInputflg) {
				m_plessInputflg = true;
				m_inputnum--;
			}
			else {
				m_nowinputcnt++;
			}
		}
		else if (inputVector.y < -15)
		{
			if (!m_plessInputflg) {
				m_plessInputflg = true;
				m_inputnum++;
			}
			else {
				m_nowinputcnt++;
			}
		}
		else {
			// 
			m_plessInputflg = false;
			m_nowinputcnt = 0;
		}
	}
	else
	{
		if (inputVector.x > 15) {
			if (!m_plessInputflg) {
				m_plessInputflg = true;
				m_inputnum++;
			}
			else {
				m_nowinputcnt++;
			}
		}
		else if (inputVector.x < -15)
		{
			if (!m_plessInputflg) {
				m_plessInputflg = true;
				m_inputnum--;
			}
			else {
				m_nowinputcnt++;
			}
		}
		else {
			// 
			m_plessInputflg = false;
			m_nowinputcnt = 0;
		}
	}

	if (m_nowinputcnt > m_maxinputcnt)
	{
		m_nowinputcnt = 0;
		m_plessInputflg = false;
	}

	// 決定
	if (Controller::Instance()->GetButtonATrigger()) {

		m_inputflg = true;
	}

	if (Controller::Instance()->GetButtonBTrigger()) {
		m_cancelflg = true;
	}
}

void SelectPlayer::InputCnt()
{
	// 入力あった時
	if (m_inputflg)
	{
		switch (m_inputnum)
		{
		case 0:
			if (!GetChangeFlg())
			{
				SetChangeFlg(ColorMODE::DOWN);
			}
			if (GetChangeFlg())
			{
				m_select = Select::STAGESELECT;
				m_type = InputType::VERTEX;
				m_inputflg = false;
				SetChangeFlg(ColorMODE::UP);
			}
		
			break;
		case 1:
			m_select = Select::WAIT;
			fade->GetComponent<Component::FadeManager>()->SetFadeScene(60, "TitleScene");
			break;
		case 2:
			m_select = Select::END;
			GameEnd();
		case 3:			
			break;

		case 4:
		
			break;
		default:
			break;
		}
	}

	m_cancelflg = false;
	
	printf("NONE ");
	printf("%d \n", m_inputnum);
}

void SelectPlayer::StageSelect()
{
	// 入力あった時
	if (m_inputflg)
	{
		switch (m_inputnum)
		{
		case 0:
			if (!GetChangeFlg())
			{
				SetChangeFlg(ColorMODE::DOWN);
			}
			if (GetChangeFlg())
			{
				m_type = InputType::VERTEX;
				m_select = Select::GAMESTART;
				SetChangeFlg(ColorMODE::UP);
				m_inputflg = false;
				m_loadscnestr = "Map1_1";
				m_inputnum = 0;

			}
			break;
		case 1:
			if (!GetChangeFlg())
			{
				SetChangeFlg(ColorMODE::DOWN);
			}
			if (GetChangeFlg())
			{
				m_type = InputType::VERTEX;
				m_select = Select::GAMESTART;
				SetChangeFlg(ColorMODE::UP);
				m_inputflg = false;
				m_loadscnestr = "Map1_2";
				m_inputnum = 0;

			}
			break;
		case 2:
			if (!GetChangeFlg())
			{
				SetChangeFlg(ColorMODE::DOWN);
			}
			if (GetChangeFlg())
			{
				m_type = InputType::VERTEX;
				m_select = Select::GAMESTART;
				SetChangeFlg(ColorMODE::UP);
				m_inputflg = false;
				m_loadscnestr = "Map1_3";
				m_inputnum = 0;

			}
			break;

			// 
		case 3:
			if (!GetChangeFlg())
			{
				SetChangeFlg(ColorMODE::DOWN);
			}
			if (GetChangeFlg())
			{
				SetChangeFlg(ColorMODE::UP);
				m_select = Select::NONE;
				m_type = InputType::VERTEX; 
				m_inputflg = false;	
				m_inputnum = 0;
			}
			break;
		default:
			break;
		}
	}

	// キャンセルボタン入力時
	if (m_cancelflg)
	{
		if (!GetChangeFlg())
		{
			SetChangeFlg(ColorMODE::DOWN);
		}
		if (GetChangeFlg())
		{
			m_type = InputType::VERTEX;
			m_select = Select::NONE;
			SetChangeFlg(ColorMODE::UP);
			m_cancelflg =false;
			m_inputnum = 0;
		}
	}
	int size = 18;
	const int brank = 10;
	const float posy = 150;
	switch (m_inputnum)
	{
	case 0:
		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 200,
			Application::Instance()->CLIENT_HEIGHT / 2 - posy + (size + brank) * 0,
			size, size, m_color, "STAGE-1 DATE");

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 200,
			Application::Instance()->CLIENT_HEIGHT / 2 - posy + (size + brank)* 1,
			size, size, m_color, "ENEMY : 4");

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 200,
			Application::Instance()->CLIENT_HEIGHT / 2 - posy + (size + brank) * 2,
			size, size, m_color, "TIME  : 10:00:00");
		break;
	case 1:
		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 200,
			Application::Instance()->CLIENT_HEIGHT / 2 - posy + (size + brank) * 0,
			size, size, m_color, "STAGE-2 DATE");

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 200,
			Application::Instance()->CLIENT_HEIGHT / 2 - posy + (size + brank) * 1,
			size, size, m_color, "ENEMY : 6");

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 200,
			Application::Instance()->CLIENT_HEIGHT / 2 - posy + (size + brank) * 2,
			size, size, m_color, "TIME  : 05:00:00");
		break;
	case 2:
		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 200,
			Application::Instance()->CLIENT_HEIGHT / 2 - posy + (size + brank) * 0,
			size, size, m_color, "STAGE-3 DATE");

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 200,
			Application::Instance()->CLIENT_HEIGHT / 2 - posy + (size + brank) * 1,
			size, size, m_color, "ENEMY : 10");

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 200,
			Application::Instance()->CLIENT_HEIGHT / 2 - posy + (size + brank) * 2,
			size, size, m_color, "TIME  : 05:00:00");

		break;

		// 
	case 3:


		break;
	default:
		break;
	}


	printf("STAGESELECT ");
	printf("%d \n", m_inputnum);
}


void SelectPlayer::SelectUpdate()
{

	switch (m_select)
	{
	case Select::GAMESTART:
		GameStart();
		break;
	case Select::STAGESELECT:
		StageSelect();
		break;
	case Select::OPTION:
		break;
	case Select::SCORE:
		break;
	case Select::END:
		GameEnd();
		break;
	case Select::NONE:
		InputCnt();
		break;
	default:
		break;
	}
}

// シーン移動していいかの選択
void SelectPlayer::GameStart()
{
	printf("GAME_START ");
	printf("%d \n", m_inputnum);

	switch (m_inputnum)
	{
		// はい
	case 0:
		// UIの色変更
		// 入力ある時に実行
		if (m_inputflg)
		{
			// フェード実行
			fade->GetComponent<Component::FadeManager>()->SetFadeScene(60, m_loadscnestr);
			m_inputflg = false;
			m_select = Select::WAIT;
		}
		break;

		// いいえ
	case 1:
		// UIの色変更
		// 元に戻る
		if (m_inputflg)
		{
			m_select = Select::STAGESELECT;
			m_inputflg = false;
			m_type = InputType::VERTEX;
		}

		break;
	
	default:
		break;
	}

	if (m_cancelflg)
	{
		if (!GetChangeFlg())
		{
			SetChangeFlg(ColorMODE::DOWN);
		}
		if (GetChangeFlg())
		{
			m_type = InputType::VERTEX;
			m_select = Select::STAGESELECT;
			SetChangeFlg(ColorMODE::UP);
			m_cancelflg = false;
		}
	}

}

void SelectPlayer::InputLimit()
{
	switch (m_select)
	{
	case Select::GAMESTART:
		if (m_inputnum < 0)
		{
			m_inputnum = 2;
		}
		if (m_inputnum > 1)
		{
			m_inputnum = 0;
		}
		break;
	case Select::STAGESELECT:
		if (m_inputnum < 0)
		{
			m_inputnum = 3;
		}
		if (m_inputnum > 3)
		{
			m_inputnum = 0;
		}
		break;

	case Select::OPTION:
		break;
	case Select::SCORE:
		break;
	case Select::END:

		break;
	case Select::NONE:
		if (m_inputnum < 0)
		{
			m_inputnum = 2;
		}
		if (m_inputnum > 2)
		{
			m_inputnum = 0;
		}
		break;
	default:
		break;
	}
}

void SelectPlayer::GameEnd()
{
	printf("GAME_END \n");
	exit(EXIT_FAILURE);
}

void SelectPlayer::LinePos()
{
	const Vector3 base = Vector3(-1.46f, 0.65f, 1.05f);

	float _len =  m_strlen * 0.037f;

	line->GetComponent<Component::SelectUnderbarUI>()->SetPos(Vector3(base.x + _len, base.y - m_inputnum * 0.1f, base.z));

	line->GetComponent<Component::QuadRender>()->SetWidth(m_strlen * 0.074f);
}

void SelectPlayer::String()
{
	const int size = 25;
	const int brank = 10;

	switch (m_select)
	{
	case Component::SelectPlayer::Select::NONE:

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 515,
			Application::Instance()->CLIENT_HEIGHT / 2 - 340,
			35, 35, m_color, select.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
			Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 2,
			size, size, m_color, start.c_str());

		//MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
		//	Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 3,
		//	size, size, m_color, tutorial.c_str());

		//MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
		//	Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 4,
		//	size, size, m_color, setting.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
			Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 3,
			size, size, m_color, backtitle.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
			Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 4,
			size, size, m_color, end.c_str());

		break;

	case Component::SelectPlayer::Select::STAGESELECT:

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 515,
			Application::Instance()->CLIENT_HEIGHT / 2 - 340 ,
			35, 35, m_color, select.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2  -290,
			Application::Instance()->CLIENT_HEIGHT / 2 - 335 ,
			30, 30, m_color, stage.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
			Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 2,
			size, size, m_color, stage1.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
			Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 3,
			size, size, m_color, stage2.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
			Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 4,
			size, size, m_color, stage3.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
			Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 5,
			size, size, m_color, back.c_str());

		break;

	case Component::SelectPlayer::Select::GAMESTART:
		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 515,
			Application::Instance()->CLIENT_HEIGHT / 2 - 340,
			35, 35, m_color, select.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 290,
			Application::Instance()->CLIENT_HEIGHT / 2 - 335,
			30, 30, m_color, stage.c_str());

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
			Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 2,
			size, size, m_color, "YES");

		MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 500,
			Application::Instance()->CLIENT_HEIGHT / 2 - 320 + (size + brank) * 3,
			size, size, m_color, "NO");

		break;
	case Component::SelectPlayer::Select::OPTION:
		break;
	case Component::SelectPlayer::Select::SCORE:
		break;
	case Component::SelectPlayer::Select::END:

		break;
	default:
		break;
	}
	
}

void SelectPlayer::StrLength()
{
	switch (m_select)
	{
	case Component::SelectPlayer::Select::NONE:

		// 文字の大きさ
		switch (m_inputnum)
		{
		case 0:
			m_strlen = start.length();
			break;

		case 1:
			m_strlen = tutorial.length();
			break;

		case 2:
			m_strlen = setting.length();
			break;

		case 3:
			m_strlen = backtitle.length();
			break;

		case 4:
			m_strlen = end.length();
			break;
		default:
			break;
		}

		break;

	case Component::SelectPlayer::Select::STAGESELECT:

		switch (m_inputnum)
		{
		case 0:
			m_strlen = stage1.length();
			break;

		case 1:
			m_strlen = stage2.length();
			break;

		case 2:
			m_strlen = stage3.length();
			break;

		case 3:
			m_strlen = back.length();
			break;

		default:
			break;
		}

		break;

	case Component::SelectPlayer::Select::GAMESTART:
		break;
	case Component::SelectPlayer::Select::OPTION:
		break;
	case Component::SelectPlayer::Select::SCORE:
		break;
	case Component::SelectPlayer::Select::END:

		break;
	default:
		break;
	}
}




bool SelectPlayer::Slider()
{
	if (m_slideflg)
	{
		if (m_nowsldcnt < m_sldcntmax)
		{
			m_nowsldcnt++;
			return false;
		}
		else {
			m_nowsldcnt = 0;
			m_slideflg = false;
			return true;
		}
	}
}

bool SelectPlayer::ColorChange()
{
	if (colorMode != ColorMODE::NONE)
	{
		if (m_nowchancnt < m_chancntmax)
		{
			m_nowchancnt++;

			if (colorMode == ColorMODE::UP) {
				m_color.x += m_maxcolor.x / m_chancntmax;
				m_color.y += m_maxcolor.y / m_chancntmax;
				m_color.z += m_maxcolor.z / m_chancntmax;
				m_color.w += m_maxcolor.w / m_chancntmax;
			}
			if (colorMode == ColorMODE::DOWN) 
			{
				m_color.x -= m_maxcolor.x / m_chancntmax;
				m_color.y -= m_maxcolor.y / m_chancntmax;
				m_color.z -= m_maxcolor.z / m_chancntmax;
				m_color.w -= m_maxcolor.w / m_chancntmax;
			}

			m_chanflg = false;
			return false;
		}
		else {			

			if (colorMode == ColorMODE::UP) {
				m_color.x = m_maxcolor.x;
				m_color.y = m_maxcolor.y;
				m_color.z = m_maxcolor.z;
				m_color.w = m_maxcolor.w;
			}
			if (colorMode == ColorMODE::DOWN)
			{
				m_color.x = 0;
				m_color.y = 0;
				m_color.z = 0;
				m_color.w = 0;
			}

			m_nowchancnt = 0;
			m_chanflg = true;
			colorMode = ColorMODE::NONE;

			return true;
		}
	}
	else {
		m_chanflg = false;
	}
}


// 飛行機のゲームの操作 BF

// Localがtransformの各軸に対して回転する
// 普通のものはワールド