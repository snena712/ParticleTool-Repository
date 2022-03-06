#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	"Vector.h"
#include    "../source/Compute/SomeSupportCompute.h"
using Microsoft::WRL::ComPtr;

// 矩形クラス
class PostEffectQuad {
public:
	// 頂点データ
	struct Vertex {
		DirectX::XMFLOAT3 pos;		// 座標		
		DirectX::XMFLOAT4 color;	// カラー
		DirectX::XMFLOAT2 tex;		// テクスチャ
	};

	// 現在使用不可
	enum class RenderMode {
		DEFAULT,	//通常
		BILLBOARD,	//ビルボード
	};

public:

	// 矩形の初期化
	bool Init(int width, int height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2 *uv, float z, std::string _vs, std::string _ps );

	//bool InitVS(const std::string _vs);
	bool InitPS(const std::string _vs);
	bool InitVS(const std::string _vs);
	void SetVS(std::string _vs) {
		m_vsstr = _vs; };
	void SetPS(std::string _ps) { m_psstr = _ps; };

	// 描画
	void NormalDraw(Vector3* _date);

	void NormalSetting();

	void ScreenDraw();

	void Exit();

	// 拡大、縮小
	void SetScale(float sx, float sy, float sz);

	// 位置をセット
	void SetPosition(float x, float y, float z);

	// 描画モードのセット
	void SetRenderMode(RenderMode mode) { m_mode = mode; }

	// ワールド変換行列をセットする
	void SetMtx(DirectX::XMFLOAT4X4 mtx) {
		m_worldmtx = mtx;
	}

	// 頂点データ更新
	void updateVertex_3D(float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv, float z = 0.0f);

	// 頂点バッファ更新
	void updateVbuffer();

private:

	//// 配列データ
	std::string m_vsstr = "";
	std::string m_psstr = "";

	DirectX::XMFLOAT4X4		m_worldmtx;				// ワールド変換行列
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// 頂点バッファ
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// インデックスバッファ
	PostEffectQuad::Vertex	m_vertex[4];			// 矩形４頂点
	float					m_width;				// 幅
	float					m_height;				// 高さ
	float					m_depth;				// 深度値
	RenderMode				m_mode;					// 描画モード
};

//-----------------------------
// create by Suehara
//-----------------------------