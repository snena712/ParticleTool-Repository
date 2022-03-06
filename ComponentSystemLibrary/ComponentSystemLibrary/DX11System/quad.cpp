#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<string>
#include	"dx11mathutil.h"
#include	"Shader.h"
#include	"shaderhashmap.h"
#include	"DX11Settransform.h"
#include	"quad.h"
#include	"../Component\CameraComponent.h"

// comptr
using Microsoft::WRL::ComPtr;

// 使用する頂点シェーダー名
const char* Quad::vsfilename[] = {
	"shader/basicvs.hlsl",		//通常（3D）
	"shader/vsbillboard.hlsl",	//ビルボード（3D）
};

// 使用するピクセルシェーダー名
const char* Quad::psfilename[] = {
	"shader/basicps.hlsl",		//通常（3D）
	"shader/basicnotexps.hlsl",	//通常（3D、テクスチャ無し）
	"shader/psbillboard.hlsl",	//ビルボード（3D）
	"shader/multips.hlsl",
};

// 矩形の初期化
bool Quad::Init(int width, int height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv, float z) {
	DX11MtxIdentity(m_worldmtx);	// 初期姿勢

	// ピクセルシェーダーを生成
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[0]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[1]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(basicnotexps.hlsl)", "error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[2]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(monotoneps.hlsl)", "error", MB_OK);
		return false;
	}

	// ピクセルシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetPixelShader(psfilename[3]);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(monotoneps.hlsl)", "error", MB_OK);
		return false;
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// エレメント数
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[0],			// 頂点シェーダーファイル名
		layout,					// 頂点レイアウト
		numElements);			// エレメント数

		// 頂点シェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		vsfilename[1],			// 頂点シェーダーファイル名
		layout,					// 頂点レイアウト
		numElements);			// エレメント数

	// 頂点初期化
	updateVertex(width, height, color, uv, z);

	// デバイスを取得
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点バッファを生成
	sts = CreateVertexBufferWrite(
		device,						// デバイスオブジェクト
		sizeof(Quad::Vertex),		// １頂点当たりバイト数
		4,							// 頂点数
		m_vertex,					// 頂点データ格納メモリ先頭アドレス
		&m_vertexbuffer				// 頂点バッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// インデックスバッファを生成
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		device,						// デバイスオブジェクト
		4,							// インデックス数
		index,						// インデックス格納メモリ先頭アドレス
		&m_indexbuffer				// インデックスバッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	m_mode = RenderMode::DEFAULT;

	return true;
}

// 描画
void Quad::Draw() {

	DirectX::XMFLOAT4X4 mtx;

	// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ビュー変換行列セット
	mtx = Component::Camera::GetMainCamera()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	// プロジェクション変換行列セット
	mtx = Component::Camera::GetMainCamera()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// ワールド変換行列をセット
	if (m_mode == RenderMode::BILLBOARD) {
		DirectX::XMFLOAT4X4 cameramat = Component::Camera::GetMainCamera()->GetCameraMatrix();

		m_worldmtx._11 = cameramat._11;
		m_worldmtx._12 = cameramat._21;
		m_worldmtx._13 = cameramat._31;

		m_worldmtx._21 = cameramat._12;
		m_worldmtx._22 = cameramat._22;
		m_worldmtx._23 = cameramat._32;

		m_worldmtx._31 = cameramat._13;
		m_worldmtx._32 = cameramat._23;
		m_worldmtx._33 = cameramat._33;

		m_worldmtx._14 = 0;
		m_worldmtx._24 = 0;
		m_worldmtx._34 = 0;
		m_worldmtx._44 = 1;
	}

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Quad::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;						// オフセット値をセット

	// 頂点バッファをデバイスコンテキストへセット
	devcontext->IASetVertexBuffers(
		0,									// スタートスロット
		1,									// 頂点バッファ個数
		m_vertexbuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
		&stride,							// ストライド
		&offset);							// オフセット

	// インデックスバッファをデバイスコンテキストへセット
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// インデックスバッファ
		DXGI_FORMAT_R32_UINT,				// フォーマット
		0);									// オフセット

	// トポロジーをセット
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点シェーダー、ピクセルシェーダー、頂点レイアウト取得
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11InputLayout* layout = nullptr;

	
	switch (m_mode)
	{
		//デフォルト
	case RenderMode::DEFAULT:
		vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
		ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[0]);
		layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);
		break;

		//ビルボード
	case RenderMode::BILLBOARD:
		vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[1]);
		ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[2]);
		layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[1]);
		break;

	default:
		break;
	}

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);

	// ドローコール発行
	devcontext->DrawIndexed(
		4,						// インデックス数
		0,						// 開始インデックス
		0);						// 基準頂点インデックス
}

// マルチテクスチャの描画
void Quad::MultiDraw()
{


	DirectX::XMFLOAT4X4 mtx;

	// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	// ビュー変換行列セット
	mtx = Component::Camera::GetMainCamera()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	// プロジェクション変換行列セット
	mtx = Component::Camera::GetMainCamera()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	// ワールド変換行列をセット
	if (m_mode == RenderMode::BILLBOARD) {
		DirectX::XMFLOAT4X4 cameramat = Component::Camera::GetMainCamera()->GetCameraMatrix();

		m_worldmtx._11 = cameramat._11;
		m_worldmtx._12 = cameramat._21;
		m_worldmtx._13 = cameramat._31;

		m_worldmtx._21 = cameramat._12;
		m_worldmtx._22 = cameramat._22;
		m_worldmtx._23 = cameramat._32;

		m_worldmtx._31 = cameramat._13;
		m_worldmtx._32 = cameramat._23;
		m_worldmtx._33 = cameramat._33;

		m_worldmtx._14 = 0;
		m_worldmtx._24 = 0;
		m_worldmtx._34 = 0;
		m_worldmtx._44 = 1;
	}

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	unsigned int stride = sizeof(Quad::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;						// オフセット値をセット

	// 頂点バッファをデバイスコンテキストへセット
	devcontext->IASetVertexBuffers(
		0,									// スタートスロット
		1,									// 頂点バッファ個数
		m_vertexbuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
		&stride,							// ストライド
		&offset);							// オフセット

	// インデックスバッファをデバイスコンテキストへセット
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// インデックスバッファ
		DXGI_FORMAT_R32_UINT,				// フォーマット
		0);									// オフセット

	// トポロジーをセット
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// 頂点シェーダー、ピクセルシェーダー、頂点レイアウト取得
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	ID3D11InputLayout* layout = nullptr;


	switch (m_mode)
	{
		//デフォルト
	case RenderMode::DEFAULT:
		vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[0]);
		ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[3]);
		layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[0]);
		break;

		//ビルボード
	case RenderMode::BILLBOARD:
		vs = ShaderHashmap::GetInstance()->GetVertexShader(vsfilename[1]);
		ps = ShaderHashmap::GetInstance()->GetPixelShader(psfilename[3]);
		layout = ShaderHashmap::GetInstance()->GetVertexLayout(vsfilename[1]);
		break;

	default:
		break;
	}

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);

	// ドローコール発行
	devcontext->DrawIndexed(
		4,						// インデックス数
		0,						// 開始インデックス
		0);						// 基準頂点インデックス
}

void Quad::Exit() {

}

// 拡大、縮小
void Quad::SetScale(float sx, float sy, float sz) {
	DX11MtxScale(sx, sy, sz, m_worldmtx);
}

// 位置をセット
void Quad::SetPosition(float x, float y, float z) {
	m_worldmtx._41 = x;
	m_worldmtx._42 = y;
	m_worldmtx._43 = z;
}

// X軸回転
void Quad::SetRotationX(float angle) {
	DX11MtxRotationX(angle, m_worldmtx);
}
// Y軸回転
void Quad::SetRotationY(float angle) {
	DX11MtxRotationY(angle, m_worldmtx);
}
// Z軸回転
void Quad::SetRotationZ(float angle) {
	DX11MtxRotationZ(angle, m_worldmtx);
}


// 位置と回転
void Quad::SetPosRot(Vector3 pos, Vector3 rot) {
	Vector3 zero = Vector3::zero;
	DX11MakeWorldMatrix(m_worldmtx, zero, rot, pos);
}


// 頂点データ更新
void Quad::updateVertex(float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* uv, float z) {

	m_width = width;
	m_height = height;

	Quad::Vertex	v[4] = {
		// 座標													// カラー	// UV	
		XMFLOAT3(-m_width / 2.0f,	 m_height / 2.0f, z),	color,		uv[0],
		XMFLOAT3(m_width / 2.0f,	 m_height / 2.0f, z),	color,		uv[1],
		XMFLOAT3(-m_width / 2.0f,	-m_height / 2.0f, z),	color,		uv[2],
		XMFLOAT3(m_width / 2.0f,	-m_height / 2.0f, z),	color,		uv[3]
	};

	for (int i = 0; i < 4; i++) {
		m_vertex[i] = v[i];
	}
}


// 頂点バッファ更新
void Quad::updateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	HRESULT hr = devcontext->Map(m_vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(m_vertex), sizeof(Quad::Vertex) * 4);
		devcontext->Unmap(m_vertexbuffer.Get(), 0);
	}
}