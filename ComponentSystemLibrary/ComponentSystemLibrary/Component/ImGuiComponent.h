#pragma once

#include "../Application.h"

#ifdef DEBUG

#include "ComponentBaseClass.h"
#include "../DX11System/quad.h"
#include <string>
#include <vector>
#include "../DX11System/Vector.h"
#include <string>
#include <cctype>
#include <stdio.h>

#include "../imGui/imgui.h"
#include "../imGui/imgui_impl_win32.h"
#include "../imGui/imgui_impl_dx11.h"
#include "D3D11.h"

// 各種フラグ
class Flgs
{
public:
	bool debug = false;
	bool firstdebugflg = false;	// デバッグ用
	bool dend = false;
};

// ImGui管理クラス
class ImGuiManager {
private:
	enum class PlayMode
	{
		Debug,
		Release,
	};
	//---------画像用変数-------------//
	static const std::string startImage;
	static const std::string stopImage;

	//------------変数-----------//
	PlayMode	playMode;						// 実行モード
	bool		firstflg;						// 初回オープン（ポジション指定）
	bool        pauseflg = false;				// 一時停止
	bool        oneFlameFlg = false;			// 1フレーム進める
	int objcnt;									// オブジェクトの数
	Flgs flgs;									// 更新フラグ

public:
	//-------------------------基本処理---------------------------//
	static ImGuiManager* Instance() {
		static ImGuiManager singleton;
		return &singleton;
	}

	ImGuiManager();
	//~ImGuiManager();

	// メインループで使用
	void ImGuiUpdate();
	void ImGuiRender();
	void ImGuiEnd();

	//-------------------デバッグモードの操作---------------------//
	bool CheckPlayMode();											// ImGui親の動作確認
	void DebugStop() { playMode = PlayMode::Release; }				// ImGuiデバッグしない
	void DebugPlay() { playMode = PlayMode::Debug; }				// ImGuiデバッグする

	void Pause();													// ポーズの処理
	void OneFrame();												// １フレーム進める

	//-------------------フラグ取得関数---------------------//
	Flgs GetFlg() { return flgs; }									// フラグ管理クラスの返す
	bool GetPause() { return pauseflg; }							// ポーズの有無
};

#endif
