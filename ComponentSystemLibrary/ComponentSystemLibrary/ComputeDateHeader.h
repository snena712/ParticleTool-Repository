#pragma once
#include	"DX11System/Memory.h"
#include    "DX11System/Shader.h"

#define TOOLMODE
// 状態管理
enum class StatusCompute
{
	Dead,
	Live
};

enum class Shape
{
	Sphere,
	Box
};

enum class ColorControl
{
	Normal,
	Picture
};

// パーティクルの個々が持っている情報の構造体
ALIGN16 struct ParticleBase
{
	XMFLOAT3 pos;	// 位置
	float life;		// ライフ

	XMFLOAT3 vel;	// パーティクルが持っている力	
	int Live;		// 生存可否

	XMFLOAT3 scale;	// 大きさ
	float depth;	// 深度
	
	XMFLOAT3 dummy;	// ダミー
	float parentId;	// 親ID

	XMFLOAT4 color;	// 描画用色情報

	XMFLOAT3 movePos;	// パーティクルの移動量
};

ALIGN16 struct ParticleDraw
{
	XMFLOAT3 pos;		// 位置
	float dummyParticleDraw1;

	XMFLOAT3 scale;		// 大きさ
	float dummyParticleDraw2;

	XMFLOAT4 color;		// 色
};

// ソート用変数
ALIGN16 struct SortStruct
{
	float inc;
	float dir;
	float time;
	float seedkey;
};
// パーティクルの生成数
ALIGN16 struct ParticleNum
{
	float spawnNum;			// 現在の数
	float nowSpawnNum;		// 今フレームの生成された数
	float maxParticleNum;	// 初期化されていて利用可能な数
	float seed;				// 16バイト用のダミー
	float loopcnt;			// forのループ回数
	float dummynum;			// ダミー
	float dummynum2;		// ダミー
	float dummynum3;		// ダミー
};

// 管理番号(定数)
ALIGN16 struct ParticleHeader
{
	float tableNumber;		// 配列番号　値ないときは-1
};

// 管理ヘッダー（定数）
ALIGN16 struct EmitterHeader
{
	//float ;

};

// 大きさの構造体
ALIGN16 struct ScaleAnim
{
	XMFLOAT3 initScale;		// 初期のサイズ
	float dummyscale;		// 16バイト用のダミー
	XMFLOAT3 finishScale;	// 最終の大きさ
	float dummyscale2;
};

// 色の構造体
ALIGN16 struct Color
{
	XMFLOAT4 initColor;
	XMFLOAT4 endColor;
};

// 計算のプラス値
ALIGN16 struct PlusData
{
	XMFLOAT4X4 Mtx;	// カメラの行列

	XMFLOAT3 rand;
	float life;				// 初期化のライフ

	XMFLOAT3 vel;			// 移動量
	int nowSpawnMaxNum;		// 今フレームの生成最大数

	XMFLOAT3 spawnPos;		// 発生ポジション
	float speed;			// 生成範囲の幅


	XMFLOAT3 spawnPosRange;	// 生成範囲の幅
	int parentId;			// 生成時の親ID

	ScaleAnim scaleAnim;	// アニメーション

	Color color;			// 色の構造体

	XMFLOAT3 velRange;		// 力の範囲
	float gravity;			// 重力

	XMFLOAT3 forward;		// 前方方向
	float pictureWidthSize;	// 色画像の横

	int velocityFlg;		// 移動に関するフラグ
	int colorFlg;			// 色に関するフラグ
	int Live;				// 生存可否		
	int dummyFlg;			//　ダミー
};

// 描画物の切換え
enum class DrawStatus
{
	Second,
	Third
};

//-----------------------------
// create by Suehara
//-----------------------------