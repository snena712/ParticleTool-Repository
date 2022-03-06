#include	"Box.h"

// 法線ベクトルを計算
// ノーマライズ
void Box::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {
	DX11Vec3Normalize(Normal, Normal);
}

// インデックスデータを作成
void Box::CreateIndex() {	// インデックスリスト
	
	Face g_IndexList[]{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};

	for (auto ver : g_IndexList)
	{
		m_face.emplace_back(ver);
	}

}

// 頂点データを作成
void Box::CreateVertex() {

	float azimuth = 0.0f;			// 方位角
	float elevation = 0.0f;			// 仰角

	XMFLOAT3	Normal;
	// 方位角と仰角から球メッシュの頂点データを作成

	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), m_color },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f),  XMFLOAT3(1.0f, 1.0f, -1.0f),  m_color },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),   XMFLOAT3(1.0f, 1.0f, 1.0f),   m_color },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),  XMFLOAT3(-1.0f, 1.0f, 1.0f),  m_color },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f),XMFLOAT3(-1.0f, -1.0f, -1.0f),m_color },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), m_color },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f),  XMFLOAT3(1.0f, -1.0f, 1.0f),  m_color },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), m_color },
	};

	// 頂点情報の入れ込み
	for (auto ver : vertices)
	{
		m_vertex.emplace_back(ver);
	}

}

// 描画
void Box::Draw() {

	ID3D11DeviceContext* _devicecon = GetDX11DeviceContext();

	// 頂点バッファをセットする
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	_devicecon->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	_devicecon->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			// インデックスバッファをセット
	_devicecon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);				// トポロジーをセット（旧プリミティブタイプ）
	_devicecon->IASetInputLayout(ShaderHashmap::GetInstance()->GetVertexLayout(m_VertexShaderName));				// 頂点レイアウトセット

	_devicecon->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_VertexShaderName), nullptr, 0);			// 頂点シェーダーをセット
	_devicecon->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_PixelShaderName), nullptr, 0);			// ピクセルシェーダーをセット

	_devicecon->DrawIndexed(
		static_cast<unsigned int>(m_face.size() * 3),		// 描画するインデックス数（面数×３）
		0,									// 最初のインデックスバッファの位置
		0);									// 頂点バッファの最初から使う
}

bool Box::Init(
	XMFLOAT4 color) {

	bool sts;

	// カラー値を保存
	m_color = color;

	// デバイスの設定
	ID3D11Device* _device = GetDX11Device();

	// 球のインデックスデータを作成
	CreateIndex();

	// 球の頂点データを作成
	CreateVertex();

	// 頂点バッファ作成
	sts = CreateVertexBufferWrite(
		_device,
		sizeof(Vertex),						// １頂点当たりバイト数
		m_vertex.size(),					// 頂点数
		m_vertex.data(),					// 頂点データ格納メモリ先頭アドレス
		m_pVertexBuffer.GetAddressOf());	// 頂点バッファ
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(vertex buffer) error", "Error", MB_OK);
		return false;
	}

	// インデックスバッファ作成
	sts = CreateIndexBuffer(
		_device,								// デバイスオブジェクト
		m_face.size() * 3,					// インデックス数
		m_face.data(),						// インデックスデータ先頭アドレス
		m_pIndexBuffer.GetAddressOf());		// インデックスバッファ
	if (!sts) {
		MessageBox(NULL, "CreateBuffer(index buffer) error", "Error", MB_OK);
		return false;
	}

	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		m_VertexShaderName,
		layout,
		numElements);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
	}

	// ピクセルシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetPixelShader(m_PixelShaderName);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
	}

	return true;
}


// 頂点バッファの更新
// 毎フレーム更新を変更時のみに変更する
void Box::ColorUpdate(XMFLOAT4 _color)
{
	for (int i=0;i< m_vertex.size();i++)
	{
		m_vertex[i].Color = _color;
	}

	// bufferの更新
	D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };

	GetDX11DeviceContext()->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	memcpy(MappedResource.pData, m_vertex.data(), sizeof(Vertex) * m_vertex.size()); // 3頂点分コピー
	GetDX11DeviceContext()->Unmap(m_pVertexBuffer.Get(), 0);
}

void Box::SetTransform(Transform* _tra, Vector3 _scale, Vector3 _size)
{
	// 当たり判定の枠の描画
	
	DirectX::XMFLOAT4X4 make, mtx, scale;

	DX11MtxScale(_scale.x * _size.x, _scale.y * _size.y, _scale.z * _size.z, scale);
	DX11MtxFromQt(mtx, _tra->qt);
	DX11MtxMultiply(make, scale, mtx);

	make._41 = _tra->position.x;
	make._42 = _tra->position.y;
	make._43 = _tra->position.z;

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, make);
}