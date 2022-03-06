#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>

// comptr
using Microsoft::WRL::ComPtr;

// 矩形クラス
class Quad2D {
public:
	// 頂点データ
	struct Vertex {
		DirectX::XMFLOAT3 pos;		// 座標		
		DirectX::XMFLOAT4 color;	// カラー
		DirectX::XMFLOAT2 tex;		// テクスチャ
	};

	enum class UV_STATE {
		DEFAULT,	//デフォルト
		TURN_X,		//X軸で反転
		TURN_Y,		//Y軸で反転
		TURN_XY		//XY軸で反転
	};

	// 矩形の初期化
	bool Init(int width, int height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv);

	// 描画
	void Draw();

	// マルチテクスチャの描画
	void MultiDraw();

	void Exit();

	// ビューポートのサイズをセット
	bool SetViewPortSize(uint32_t width, uint32_t height);

	// 拡大、縮小
	void SetScale(float sx, float sy, float sz);

	// 位置をセット
	void SetPosition(float x, float y, float z);

	// 位置と回転
	void SetPosRot(Vector3 pos, Vector3 rot);

	// ワールド変換行列をセットする
	void SetMtx(DirectX::XMFLOAT4X4 mtx) {
		m_worldmtx = mtx;
	}

	// テクスチャファイル名のセット
	void SetTexture(std::string _texFileName) { m_texFileName = _texFileName; }

	// 頂点データ更新
	void updateVertex(float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv);

	// 頂点バッファ更新
	void updateVbuffer();

private:
	static const char* vsfilename[];
	static const char* psfilename[];

	DirectX::XMFLOAT4X4		m_worldmtx;				// ワールド変換行列
	ComPtr<ID3D11Buffer>	m_vertexbuffer;			// 頂点バッファ
	ComPtr<ID3D11Buffer>	m_indexbuffer;			// インデックスバッファ
	Quad2D::Vertex			m_vertex[4];			// 矩形４頂点
	float					m_width;				// 幅
	float					m_height;				// 高さ
	std::string				m_texFileName;			// テクスチャファイル名
};