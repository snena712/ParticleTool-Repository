//*****************************************************************************
//!	@file	Application.cpp
//!	@brief	
//!	@note	アプリケーションクラス
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <Crtdbg.h>
#include <Windows.h>
#include <chrono>
#include <thread>
#include "Application.h"
#include "../ComponentSystemLibrary/DX11System/CWindow.h"
#include "../ComponentSystemLibrary/DX11System/macro.h"
#include "game.h"
#include "DX11System/Time.h"

//-----------------------------------------------------------------------------
// スタティック　メンバー
//-----------------------------------------------------------------------------
const char*			Application::WINDOW_TITLE = "アプリケーションクラス";
const char*			Application::WINDOW_CLASS_NAME = "win32app";

const uint32_t		Application::WINDOW_STYLE_WINDOWED = (WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
const uint32_t		Application::WINDOW_EX_STYLE_WINDOWED = (0);
const uint32_t		Application::WINDOW_STYLE_FULL_SCREEN = (WS_VISIBLE | WS_POPUP);
const uint32_t		Application::WINDOW_EX_STYLE_FULL_SCREEN = (0);


//--------------------画面サイズ--------------------//
const uint32_t		Application::CLIENT_WIDTH =  1080;
const uint32_t		Application::CLIENT_HEIGHT = 720;


uint32_t			Application::SYSTEM_WIDTH = 0;
uint32_t			Application::SYSTEM_HEIGHT = 0;

const float			Application::FPS = 60;


//==============================================================================
//!	@fn		CApplication
//!	@brief	コンストラクタ
//!	@param	
//!	@retval	
//==============================================================================
Application :: Application(): m_SystemCounter		( 0 )
{}

//==============================================================================
//!	@fn		~Application
//!	@brief	デストラクタ
//!	@param	
//!	@retval	
//==============================================================================
Application :: ~Application()
{
	Dispose();
}

//==============================================================================
//!	@fn		GetInstance
//!	@brief	インスタンス取得
//!	@param	
//!	@retval	インスタンス
//==============================================================================
Application* Application::Instance()
{
	static Application Instance;

	return &Instance;
}

//==============================================================================
//!	@fn		InitSystemWH
//!	@brief	システム領域の幅と高さを初期化
//!	@param	
//!	@retval	
//==============================================================================
void Application :: InitSystemWH()
{

}

//==============================================================================
//!	@fn		Init
//!	@brief	初期化
//!	@param	インスタンスハンドル
//!	@retval	
//==============================================================================
bool Application :: Init( HINSTANCE h_Instance )
{
	// メモリーリークを検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);


#ifdef DEBUG
	// コンソールを割り当てる
	AllocConsole();
	// 標準出力の割り当て
	freopen_s(&m_fp, "CON", "w", stdout);
#endif
	  
	// 幅と高さ初期化
	InitSystemWH();

	// ウインドウ作成
	CWindow* window = CWindow::Instance();
	window->RegisterClass( h_Instance, WINDOW_CLASS_NAME, CS_OWNDC );

	window->SetWindow( h_Instance,
					   WINDOW_STYLE_WINDOWED,
					   WINDOW_EX_STYLE_WINDOWED,
					   NULL,
					   CLIENT_WIDTH + SYSTEM_WIDTH,
					   CLIENT_HEIGHT + SYSTEM_HEIGHT,
					   WINDOW_CLASS_NAME,
					   WINDOW_TITLE,
					   false);
			
	m_hWnd = window->GetHandle();
	m_hInst = h_Instance;

	return true;
}

//==============================================================================
//!	@fn		Dispose
//!	@brief  終了処理
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Dispose()
{
#ifdef DEBUG
	// 標準出力クローズ
	fclose(m_fp);
	// コンソール開放
	::FreeConsole();
#endif
	

	return;
}

//==============================================================================
//!	@fn		MainLoop
//!	@brief	メインループ
//!	@param	
//!	@retval	メッセージID
//==============================================================================
unsigned long Application :: MainLoop()
{
	MSG		msg;	
	ZeroMemory( &msg, sizeof( msg ) );	

	CWindow* window = CWindow::Instance();

	// ゲームの初期処理
	GameInit();

	//タイム初期化処理
	Time::GetInstance()->Init();

	while (window->ExecMessage()) {

		Time::GetInstance()->Update();

		GameInput();		// ｹﾞｰﾑ入力	
		GameUpdate();		// ｹﾞｰﾑ更新
		GameRender();		// ｹﾞｰﾑ描画

		if (Time::GetInstance()->sleepTime > 0) {
			float tt = Time::GetInstance()->sleepTime / 1000.0f;
			//printf("sleep:%f \n", tt);
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));
			//指定した相対時間だけ現スレッドをブロックする (function template)
		}
	}

	//タイム終了処理
	Time::GetInstance()->Exit();

	// ゲームの終了処理
	GameDispose();

	return window->GetMessage();
}

//==============================================================================
//!	@fn		Input
//!	@brief	キー入力
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Input(uint64_t deltataime)
{
}

//==============================================================================
//!	@fn		Update
//!	@brief	更新
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Update(uint64_t deltataime)
{
	// システムカウンタ
	m_SystemCounter ++;
}

//==============================================================================
//!	@fn		Render
//!	@brief	描画
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Render(uint64_t deltatime)
{
}

//==============================================================================
//!	@fn		GetHWnd
//!	@brief	HWND 取得
//!	@param	
//!	@retval	HWND
//==============================================================================
HWND Application :: GetHWnd()
{
	return m_hWnd;
}

//==============================================================================
//!	@fn		GetHInst
//!	@brief	HINSTANCE 取得
//!	@param	
//!	@retval	HINSTANCE
//==============================================================================
HINSTANCE Application::GetHInst()
{
	return m_hInst;
}

//******************************************************************************
//	End of file.
//******************************************************************************
