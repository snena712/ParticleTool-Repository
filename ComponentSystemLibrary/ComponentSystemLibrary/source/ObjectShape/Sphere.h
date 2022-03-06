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
#include "../../ComputeDateHeader.h"

using Microsoft::WRL::ComPtr;

class Sphere {
private:
	ComPtr<ID3D11Buffer> m_pVertexBuffer;		// 頂点バッファ
	ComPtr<ID3D11Buffer> m_pIndexBuffer;		// インデックスバッファ
	unsigned int		m_divX;					// 水平方向の分割数
	unsigned int		m_divY;					// 垂直方向の分割数
	float				m_radius;				// 半径
	XMFLOAT4			m_color;				// 頂点カラー
	std::string	m_PixelShaderName = "shader/pssphere.hlsl";		// PSシェーダー名
	std::string	m_VertexShaderName = "shader/vssphere.hlsl";	// VSシェーダー名

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
	void Draw(unsigned int _num, ParticleBase* _particleBase);

	// 初期化
	bool Init(float r,						// 半径
		int division_horizontal,			// 水平方向の分割数
		int division_vertical,				// 垂直方向の分割数
		std::string _VS,					// 頂点シェーダー
		std::string _PS);					// ピクセルシェーダー

	// 頂点バッファの更新
	void ColorUpdate(XMFLOAT4 _color);

	void SetTransform(XMFLOAT3 _pos, float _Radius, float _size);
};