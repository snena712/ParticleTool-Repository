#include	<d3d11.h>
#include	<DirectXMath.h>
#include	<wrl/client.h>
#include	<string>
#include	"dx11mathutil.h"
#include	"Shader.h"
#include	"shaderhashmap.h"
#include	"DX11Settransform.h"
#include	"PostEffectQuad.h"
#include	"../Component\CameraComponent.h"

// comptr
using Microsoft::WRL::ComPtr;

// 矩形の初期化
bool PostEffectQuad::Init(int width, int height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2 *uv, float z,
	std::string _vs, std::string _ps) {

	DX11MtxIdentity(m_worldmtx);	// 初期姿勢

	m_psstr = _ps;
	m_vsstr = _vs;

	// エレメント数
	updateVertex_3D(width, height, color, uv, z);
	InitVS(m_vsstr);
	Helper::SetPS(m_psstr);

	// デバイスを取得
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	return true;
}

void PostEffectQuad::NormalSetting()
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


	unsigned int stride = sizeof(PostEffectQuad::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;						// オフセット値をセット

	// トポロジーをセット
	devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// インデックスバッファをデバイスコンテキストへセット
	devcontext->IASetIndexBuffer(
		m_indexbuffer.Get(),				// インデックスバッファ
		DXGI_FORMAT_R32_UINT,				// フォーマット
		0);									// オフセット

	// 頂点シェーダー、ピクセルシェーダー、頂点レイアウト取得
	auto vs = ShaderHashmap::GetInstance()->GetVertexShader(m_vsstr);
	auto ps = ShaderHashmap::GetInstance()->GetPixelShader(m_psstr);
	auto layout = ShaderHashmap::GetInstance()->GetVertexLayout(m_vsstr);

	// シェーダー設定
	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);
}

// 描画　オブジェクト(ビルボード)
void PostEffectQuad::NormalDraw(Vector3* _pos)
{
	// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(PostEffectQuad::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;						// オフセット値をセット

	// ワールド変換行列をセット ビルボード
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

	m_worldmtx._41 = _pos->x;
	m_worldmtx._42 = _pos->y;
	m_worldmtx._43 = _pos->z;

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;

	vs = ShaderHashmap::GetInstance()->GetVertexShader(m_vsstr);
	ps = ShaderHashmap::GetInstance()->GetPixelShader(m_psstr);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);


	// 指定回数更新と変更
	// 頂点バッファをデバイスコンテキストへセット
	devcontext->IASetVertexBuffers(
		0,									// スタートスロット
		1,									// 頂点バッファ個数
		m_vertexbuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
		&stride,							// ストライド
		&offset);							// オフセット

	// ドローコール発行 
	devcontext->DrawIndexed(
		4,						// インデックス数
		0,						// 開始インデックス
		0);						// 基準頂点インデックス
}

// 画面描画
void PostEffectQuad::ScreenDraw()
{
	DirectX::XMFLOAT4X4 mtx;

	// デバイスコンテキストを取得
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	unsigned int stride = sizeof(PostEffectQuad::Vertex);	// ストライドをセット（１頂点当たりのバイト数）
	unsigned  offset = 0;									// オフセット値をセット

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

	ID3D11InputLayout* layout = nullptr;
	ID3D11VertexShader* vs = nullptr;
	ID3D11PixelShader* ps = nullptr;
	
	layout = ShaderHashmap::GetInstance()->GetVertexLayout(m_vsstr);
	vs = ShaderHashmap::GetInstance()->GetVertexShader(m_vsstr);
	ps = ShaderHashmap::GetInstance()->GetPixelShader(m_psstr);

	devcontext->VSSetShader(vs, nullptr, 0);
	devcontext->GSSetShader(nullptr, nullptr, 0);
	devcontext->HSSetShader(nullptr, nullptr, 0);
	devcontext->DSSetShader(nullptr, nullptr, 0);
	devcontext->PSSetShader(ps, nullptr, 0);

	// 頂点フォーマットをセット
	devcontext->IASetInputLayout(layout);

	// ドローコール発行   位置
	devcontext->DrawIndexed(
		4,						// インデックス数
		0,						// 開始インデックス
		0);						// 基準頂点インデックス
}


void PostEffectQuad::Exit() {
}

// 拡大、縮小
void PostEffectQuad::SetScale(float sx, float sy, float sz) {
	DX11MtxScale(sx, sy, sz, m_worldmtx);
}

// 位置をセット
void PostEffectQuad::SetPosition(float x, float y, float z) {
	m_worldmtx._41 = x;
	m_worldmtx._42 = y;
	m_worldmtx._43 = z;
}


// 頂点データ更新
void PostEffectQuad::updateVertex_3D(float width, float height, DirectX::XMFLOAT4 color, DirectX::XMFLOAT2* _uv, float z) {

	m_width = width;
	m_height = height;

	PostEffectQuad::Vertex	v[4] = {
		// 座標													// カラー	// UV	
		XMFLOAT3(-m_width / 2.0f,	 m_height / 2.0f, z),	color,		_uv[0],
		XMFLOAT3(m_width / 2.0f,	 m_height / 2.0f, z),	color,		_uv[1],
		XMFLOAT3(-m_width / 2.0f,	-m_height / 2.0f, z),	color,		_uv[2],
		XMFLOAT3(m_width / 2.0f,	-m_height / 2.0f, z),	color,		_uv[3]
	};

	for (int i = 0; i < 4; i++) {
		m_vertex[i] = v[i];
	}
}

// 頂点バッファ更新
void PostEffectQuad::updateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	HRESULT hr = devcontext->Map(m_vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(m_vertex), sizeof(PostEffectQuad::Vertex) * 4);
		devcontext->Unmap(m_vertexbuffer.Get(), 0);
	}
	devcontext->Unmap(m_vertexbuffer.Get(), 0);
}

bool PostEffectQuad::InitPS(std::string _ps)
{
	// ピクセルシェーダーを生成
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader(_ps);
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
		return false;
	}
}

bool PostEffectQuad::InitVS(std::string _vs)
{
	auto _device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// エレメント数
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーを生成
	bool sts = ShaderHashmap::GetInstance()->SetVertexShader(
		_vs,			// 頂点シェーダーファイル名
		layout,					// 頂点レイアウト
		numElements);			// エレメント数


	// 頂点バッファを生成
	sts = CreateVertexBufferWrite(
		_device,						// デバイスオブジェクト
		sizeof(PostEffectQuad::Vertex),	// １頂点当たりバイト数
		4,							// 頂点数
		m_vertex,					// 頂点データ格納メモリ先頭アドレス
		m_vertexbuffer.GetAddressOf()				// 頂点バッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
		return false;
	}

	// インデックスバッファを生成
	uint32_t index[4] = { 0,1,2,3 };

	sts = CreateIndexBuffer(
		_device,						// デバイスオブジェクト
		4,							// インデックス数
		index,						// インデックス格納メモリ先頭アドレス
		&m_indexbuffer				// インデックスバッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateIndexBuffer error", "error", MB_OK);
		return false;
	}

	return true;
}

//-----------------------------
// create by Suehara
//-----------------------------