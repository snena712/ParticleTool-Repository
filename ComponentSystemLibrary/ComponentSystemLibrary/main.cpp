//*****************************************************************************
//!	@file	main.cpp
//!	@brief	
//!	@note	DX11 ひな形
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>

#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"

#include "Application.h"

//-----------------------------------------------------------------------------
// マクロの定義
//-----------------------------------------------------------------------------
#define   imGui "imGui"
#define   imGui_Name "imGui_test"

//-----------------------------------------------------------------------------
// リンクライブラリの設定
//-----------------------------------------------------------------------------
#pragma comment (lib, "winmm.lib")				// リンク対象ライブラリにwinmm.libを追加
#pragma comment (lib, "d3d11.lib")				// リンク対象ライブラリにd3d11.libを追加
#pragma comment (lib, "dxgi.lib")				// リンク対象ライブラリにdxgi.libを追加
#pragma comment (lib, "D3DCompiler.lib")		// リンク対象ライブラリにd3dcompiler.libを追加
#pragma comment (lib, "DirectXTex.lib")			// リンク対象ライブラリにDirectXTex.libを追加
#pragma comment (lib, "DirectXTex.lib")			// リンク対象ライブラリにDirectXTex.libを追加
#pragma comment (lib, "directxtex.lib")			// リンク対象ライブラリに
#pragma comment (lib, "dxguid.lib")				// リンク対象ライブラリに
#pragma comment (lib, "dinput8.lib")			// リンク対象ライブラリに
#pragma comment (lib, "assimp-vc141-mtd.lib")	// リンク対象ライブラリに

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------


//==============================================================================
//!	@fn		WinMain
//!	@brief	エントリポイント
//!	@param	インスタンスハンドル
//!	@param	意味なし
//!	@param	起動時の引数文字列
//!	@param	ウインドウ表示モード
//!	@retval	TRUE　成功終了/FALSE　失敗終了
//!	@note	
//==============================================================================
INT APIENTRY WinMain(HINSTANCE  h_hInst,
	HINSTANCE  h_hPrevInst,
	LPSTR		h_lpszArgs,
	int		h_nWinMode)
{
	// アプリケーション初期処理
	Application* App = Application::Instance();		// インスタンス取得
	App->Init(h_hInst);

	// ウインドウを表示する
	ShowWindow(App->GetHWnd(), h_nWinMode);
	UpdateWindow(App->GetHWnd());

	// メインループ
	long ret = App->MainLoop();

	// アプリケーション終了処理
	App->Dispose();

	return ret;
}


//******************************************************************************
//	End of file.
//******************************************************************************
