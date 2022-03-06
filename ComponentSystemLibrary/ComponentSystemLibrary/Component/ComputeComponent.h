#pragma once
#include "ComponentBaseClass.h"
#include "../source/Buffer/ConstBuffer.h"
#include "../source/Buffer/StructuredBuffer.h"
#include "../DX11System\Shader.h"
#include "../DX11System\CDirectxGraphics.h"
#include "../Component/QuadRenderComponent.h"
#include "../DX11System/ImageResourceManager.h"
#include "../ComputeDateHeader.h"
#include "../source/Compute/ComputeBitChanger.h"
#include "CameraComponent.h"
#include "../DX11System/RandomClass.h"
#include <iostream>
#include <fstream> 
#include <sstream> 
#include <windows.h>
#include <tchar.h>
#include "../DX11System/CDirectInput.h"
#include "../DX11System/Time.h"
#include "../source/Compute/WindowFileDealer.h"
#include "../source/UI/Arrow/ArrowUI.h"
#include "../source/Frame/BoxFrame.h"

namespace Component {
	class ComputeComponent :public IRenderBase {

	private:
		//-----各種シェーダー----//
		std::string m_emitterName;		// 親の名前
		std::string m_colorPictureName;	// 親の名前

		// component
#if defined( TOOLMODE)
		Component::ArrowUI* arrow;
		GameObject* m_boxFrame;
#endif

		//-- デバッグ --//
		bool m_debugOpen = false;							// メインのImgui開閉フラグ
		bool waveFlg = false;								// 波ノイズフラグ
		bool colorMoveFlg = false;							// 色による移動フラグ
		char buf[32];										// 画像文字
		int m_stackSpawnNum = 0;							// 停止時に生成の数を０にして一時的にここへ保存
		Vector3 m_nowRot;									// エミッターの進行方向角度
		Shape veloShape;									// 力の形		
		ColorControl colorControl;							// 色の管理変数
		int item_current = 0;								// 移動管理用変数	
		int m_simulationCnt = 0;								// 移動管理用変数	
		int m_colorPictureCnt = 0;							// 色画像
		ComPtr <ID3D11ShaderResourceView> colorPictureSRV;	// 画像SRV		

		//-- ファイル --//
		std::string filename;								// セーブロード用の関数	
		std::string fileFormat = "efe";						// ファイル形式

		//-- エミッター情報 --//	
		StructuredBuffer* m_PlusStructBuffer;		// エミッターの値のバッファー
		PlusData* plusData = new PlusData;			// エミッターの値　

		//-- 経過時間と生存可能時間 --//
		float m_elapsedTime = 0;		// 生成開始から経過時間
		float m_liveTime = 0;			// パーティクルが生成される時間

		//-- 再生周り --//
		bool m_startFlg = true;			// 再生フラグ
		bool m_loopFlg = false;			// ループフラグ
		bool m_lookFlg = false;			// 注視するか？

		// 基本関数
		void Init();					// 初期化
		void ComputeUpdate();			// コンピュートの更新
		void NoLoopTimerUpdate();		// ループフラグのないものの時間経過と消失
		void UpdateVelocity();			// 移動量の更新
		void BitFlgUpdate();			// ビットフラグの更新
		void LoadColorPicture();		// 画像取得

		// セーブロード用の文字
		const std::string nameStr = "name";
		const std::string spawnNumStr = "spawnNum";
		const std::string lifeStr = "life";
		const std::string velStr = "vel";
		const std::string scaleAnim_initScaleStr = "scaleAnim_initScale";
		const std::string scaleAnim_finishScaleStr = "scaleAnim_finishScale";
		const std::string spawnPosStr = "spawnPos";
		const std::string speedStr = "speed";
		const std::string velRangeStr = "velRange";
		const std::string forwardStr = "rot";
		const std::string gravityStr = "gravity";
		const std::string velFlgStr = "velFlgType";
		const std::string InitColorStr = "InitColor";
		const std::string EndColorStr = "EndColor";
		const std::string spawnPosRangeStr = "spawnPosRange";
		const std::string liveTimeSrt = "liveTimeSrt";
		const std::string loopFlgSrt = "loopFlgSrt";

	public:
		ComputeComponent(GameObject* Owner, const char* _filename, const char* _efeName);
		~ComputeComponent();

		void Start();
		void Update();
		void DrawObject() {};

		void SetSpawnPos(Vector3 _pos) { plusData->spawnPos = _pos; }
		PlusData GetPlusData() { return *plusData; }

		// デバッグ用
		void DebugImGui();
		bool DebugImGuiOpen();

		// セーブロード用の関数
		void Save();
		void WindowsLoad();
		void NormalLoad(std::string _str);
		void LoadDeal(std::string _str);

		// ImGuiの開閉フラグ
		bool GetOpenFlg() { return m_debugOpen; }
		void ResetOpenFlg() { m_debugOpen = false; }

		// フラグ取得
		bool GetLoopFlg() { return m_loopFlg; }							// パーティクルのループ
		bool GetStartFlg() { return m_startFlg; }						// パーティクルの再生停止
		void SetStartFlg(bool _flg) { m_startFlg = _flg; }				// パーティクルの再生停止
		void ResetElapsedTime() { m_elapsedTime = 0; }
		void SetInitColor(XMFLOAT4 _col) { plusData->color.initColor = _col; }
		void SetEndColor(XMFLOAT4 _col) { plusData->color.initColor = _col; }

		// エミッター名の取得
		std::string GetEmitterName() { return m_emitterName; }
		void SetEmitterName(std::string _name) { m_emitterName = _name; }

		// 画像管理周り
		void DrawPictureColor();								// デバッグ描画
		XMFLOAT2 GetPictureSize(std::string _str);				// 画像のサイズ取得
		ID3D11ShaderResourceView* GetColorPictureSRV() {		// SRV取得
			return colorPictureSRV.Get();
		}

#if defined( TOOLMODE)
		// 矢印の描画
		void ArrowDraw() { arrow->DrawLine(plusData->spawnPos); }
		void BoxFrameDraw() {
			m_boxFrame->transform->position = Vector3(plusData->spawnPos.x, plusData->spawnPos.y, plusData->spawnPos.z);
			m_boxFrame->transform->scale = Vector3(plusData->spawnPosRange.x, plusData->spawnPosRange.y, plusData->spawnPosRange.z);
			m_boxFrame->DrawObject();
			m_boxFrame->SetExistState(false);
		}
#endif

	};
}

// colorPictureSRV 初期化時にSRV取得するようにする