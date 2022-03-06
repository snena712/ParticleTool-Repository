#pragma once
#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<math.h>
#include	<vector>
#include    "../../DX11System\DX11util.h"
#include    "../../DX11System\shaderhashmap.h"
#include	"../../DX11System/dx11mathutil.h"
#include    "../../Component/GameObject.h"
#include	"../../DX11System/DX11Settransform.h"

using Microsoft::WRL::ComPtr;

class Box {
private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// 頂点バッファ
	ComPtr<ID3D11Buffer> m_pIndexBuffer;		// インデックスバッファ
	unsigned int		m_divX;					// 水平方向の分割数
	unsigned int		m_divY;					// 垂直方向の分割数
	float				m_radius;				// 半径
	XMFLOAT4			m_color;				// 頂点カラー
	const std::string	m_PixelShaderName = "shader/pssphere.hlsl";		// PSシェーダー名
	const std::string	m_VertexShaderName = "shader/vssphere.hlsl";	// VSシェーダー名

	// 頂点データ
	struct Vertex {
		DirectX::XMFLOAT3	Pos;
		DirectX::XMFLOAT3	Normal;
		DirectX::XMFLOAT4	Color;
	};

	// インデックスデータ
	struct Face {
		unsigned int idx[3];
	};

	std::vector<Vertex>		m_vertex;			//　頂点データ
	std::vector<Face>		m_face;				//	インデックスデータ

	// インデックスデータを作成する
	void CreateIndex();
	// 頂点データを作成する
	void CreateVertex();
	// 正規化
	void Normalize(XMFLOAT3 vector, XMFLOAT3& Normal);
public:
	// 描画
	void Draw();

	// 初期化
	bool Init(XMFLOAT4	color);					// 頂点カラー


	// 頂点バッファの更新
	void ColorUpdate(XMFLOAT4 _color);

	// 位置の定数バッファの設定
	void SetTransform(Transform* _tra, Vector3 _scale, Vector3 _size);
};