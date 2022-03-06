#pragma once
#include "../Buffer/StructuredBuffer.h"
#include "../Buffer/ConstBuffer.h"
#include "../../Component/QuadRenderComponent.h"
#include "../../DX11System/DX11Settransform.h"
#include "../../DX11System/ImageResourceManager.h"
#include "../../Component/ComputeComponent.h"
#include "../../ComputeDateHeader.h"
#include "../../DX11System/CLight.h"
#include "../ObjectShape/Sphere.h"
#include "../Map/TileLine.h"
#include "SphereModel.h"
#include "../../Component/ModelRenderInstance.h"
#include "SomeSupportCompute.h"
#include "../PostEffect/RenderTarget2D.h"
#include "ComputeDetail.h"
#include "../PostEffect/BloomPostEffect.h"

const UINT NUM_ELEMENTS = 1 << 13;

class ComputeManager
{
	//---pos---//
	DirectX::XMFLOAT4X4 worldmtx;

	// デバック用変数
	bool m_debug = false;						// デバックの画面あるか
	bool initEnd = false;

	bool *m_stopFlg = new bool(false);			// 再生停止
	bool m_bloomFlg = true;
	float m_timeSpeed = 1.0f;					// 経過時間のスピード

	// 画像用変数
	std::string m_filename;
	std::string m_noiseTex;

	// マス
#if defined( TOOLMODE)
	GameObject *tile;
	bool *tileDrawFlg = new bool(true);					// タイルの描画
	bool *particleNumDrawFlg = new bool(false);			// 数の描画
	bool *particleArrowDrawFlg = new bool(true);		// 矢印の描画
	bool *particleRangeDrawFlg = new bool(true);		// 矢印の描画
#endif
	// 生成個数
	unsigned int m_MaxnumElement = 10000;		// 最大数
	float m_startTime;							// 再生時間の保存
	PlusData* Plus = new PlusData[PlusBufNum];	// エミッターの状態
	DrawStatus m_drawStatus;					// 描画状態
	Component::ModelRenderInstance* model;		// パーティクルの球体３ｄモデル
	Component::ComputeComponent* stackCompute;	// ImGuiの開いているコンピュート

	//---- shader ---//
	ComPtr<ID3D11VertexShader> m_pParticleVS = NULL;
	ComPtr<ID3D11GeometryShader> m_pParticleGS = NULL;
	ComPtr<ID3D11PixelShader >m_pParticlePS = NULL;

	ComPtr <ID3D11ComputeShader> m_pSortCS = NULL;		// ソート用シェーダー
	ComPtr <ID3D11ComputeShader> m_pInitCS = NULL;		// 初期化用シェーダー
	ComPtr <ID3D11ComputeShader> m_pParticleCS = NULL;	// 基本更新
	ComPtr <ID3D11ComputeShader> m_pSpawnCS = NULL;		// 生成用シェーダー
	ComPtr <ID3D11ComputeShader> m_pDeadCS = NULL;		// 死亡処理

	ID3D11ComputeShader* GetCSInitShader() { return m_pInitCS.Get(); }
	ID3D11ComputeShader* GetCSShader() { return m_pParticleCS.Get(); }
	ID3D11ComputeShader* GetCSSpawnShader() { return m_pSpawnCS.Get(); }
	ID3D11ComputeShader* GetCSDeadShader() { return m_pDeadCS.Get(); }

	// Blend&Sampler
	ID3D11BlendState* m_blendState;
	ID3D11SamplerState* m_sampler;

	// パーティクル本体
	ParticleBase* m_emitter;					// エミッターの配列
	ParticleBase* m_copyEmitter;				// コピー元
	ParticleDraw* m_particleDraw;

	//--buffer
	StructuredBuffer* m_particleBuffer;			// 基本のパーティクル
	StructuredBuffer* m_resultBuffer;			// 計算結果取得物
	StructuredBuffer* m_resultTestBuffer;		// 計算結果取得物
	ConstBuffer* m_plusBuffer;					// 各emitterの持つ構造体バッファ
	ConstBuffer* m_SortBuffer;					// ソート
	SortStruct* sortStruct;						// ソート

	// 構造体変数
	StructuredBuffer*	m_particleNumBuffer;	// パーティクル数管理
	ParticleNum*		m_particleNum;			// 生成数管理

	ComPtr <ID3D11ShaderResourceView> colorPictureSRV;		// 色管理画像
	std::vector<Component::ComputeComponent*> computeList;	// コンピュートコンポーネントリスト

	void InitDate();								// データ初期化
	void InitShader();								// しぇーだーの初期化
	void Reset();									// 各種設定物のリセット
	void Copy();									// CS計算結果の取得
	void BeforeDrawCS();							// 描画前CS更新　ソート
	void UpdateCS();								// CSの実行
	void BitonicSort(int gpu_data);					// ソート
	void SetBufferPlusDate();						// プラスデータの埋め込み
	void ImGui();									// デバッグ用GUI
	void SaveImGui();								// セーブ
	void LoadImGui();								// ロード
	void ResetImGui();								// エミッターのリセット
	void MenuBar();									// バーの使用
	void DrawMenuBar();								// バーの描画
	void AddParticleImGui();						// パーティクルの追加
	void DrawParticleNumImGui();					// パーティクル数描画するか
	void DrawArrowImGui();							// パーティクル数描画するか
	bool SameNameCheck(std::string _name);			// 同じ名前の確認
	void CreateBlendState();						// ブレンドステートの作成
	void CreateSampleState();						// サンプラーの作成

public :
	static ComputeManager* Instance() {
		static ComputeManager singleton;
		return &singleton;
	}

	// 基本変数
	void Init();
	void Update();
	void Draw();
	void AfterDraw();

	void FirstCS();												// CS更新初めの処理
	void InitBuffer();											// CS初期化
	void CopyParticleNumBuffer();								// 数のUAVから情報取得
	void SetComputeList(Component::ComputeComponent* _com);		// コンピュートのオブジェクトのセット
	void RemoveComputeList();									// 生成数0のオブジェクトの死亡
	void SetLookEmitter(Vector3 _vec);							// ダブルクリックで注視
	std::string ListNameCheck(std::string _name);				// リスト内で同じ名前があるかの確認
	DrawStatus GetDrawStatus() { return m_drawStatus; }			// 描画の状態取得
	bool* GetStopFlg() { return m_stopFlg;}						// パーティクルの再生停止
	void SetStopFlg(bool _flg) { *m_stopFlg = _flg; }			// パーティクルの再生停止
	void SetTimeSpeed(float _spd) { m_timeSpeed = _spd; }		// 時間の経過スピード
	float &GetTimeSpeed() { return m_timeSpeed; }				// 時間の経過スピード

	// 色画像SRVセット
	void SetColorPictureSRV(ID3D11ShaderResourceView* _srv) { colorPictureSRV = _srv; }

	ParticleNum *GetParticleNum() { return m_particleNum; }	
	ParticleBase *GetParticleBase() { return m_copyEmitter; }

	// buffer取得
	StructuredBuffer* GetParticleNumBufferBuffer() { return m_particleNumBuffer; }		// パーティクル数管理
	StructuredBuffer* GetStructuredBuffer() { return m_particleBuffer; }				// パーティクル全体のバッファ
	StructuredBuffer* GetResultBuffer() { return m_resultBuffer; }						// 結果取得用のバッファ

	// 各Shaderの取得
	ID3D11VertexShader* GetVSShader() { return m_pParticleVS.Get(); }
	ID3D11PixelShader* GetPixShader() { return m_pParticlePS.Get(); }
	ID3D11GeometryShader* GetGSShader() { return m_pParticleGS.Get(); }
	ID3D11ComputeShader* GetCSSortShader() { return m_pSortCS.Get(); }

	// bufferのコピー
	ID3D11Buffer* CreateAndCopyToDebugBuf(ID3D11Device* pDevice, ID3D11DeviceContext* pd3dImmediateContext, ID3D11Buffer* pBuffer)
	{
		ID3D11Buffer* debugbuf = NULL;

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		pBuffer->GetDesc(&desc);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.MiscFlags = 0;
		if (SUCCEEDED(pDevice->CreateBuffer(&desc, NULL, &debugbuf)))
		{
#if defined(DEBUG) || defined(PROFILE)
			debugbuf->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("Debug") - 1, "Debug");
#endif

			pd3dImmediateContext->CopyResource(debugbuf, pBuffer);
		}

		return debugbuf;
	}
};

//-----------------------------
// create by Suehara
//-----------------------------