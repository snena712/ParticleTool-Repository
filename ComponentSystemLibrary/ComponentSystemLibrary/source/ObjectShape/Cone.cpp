#include	"Cone.h"

#define VER 19

// 法線ベクトルを計算
// ノーマライズ
void Cone::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {
	DX11Vec3Normalize(Normal, Normal);
}

// インデックスデータを作成
void Cone::CreateIndex() {

	// インデックス生成
	for (int i = 0; i < (VER - 1); i++)
	{
		Face f;
		f.idx[0] = i * 2 + 0;
		f.idx[1] = i * 2 + 1;
		f.idx[2] = i * 2 + 2;

		m_face.emplace_back(f);
	}
}

// 頂点データを作成
void Cone::CreateVertex() {

	Vertex ve;

	for (DWORD i = 0; i < VER; i++)
	{
		float v = (float)i / (float)(VER - 1);
		float theta = v * 3.14f * 2;
		ve.Pos = XMFLOAT3(sinf(theta), -0.7f, cosf(theta));
		ve.Normal = XMFLOAT3(sinf(theta), 0.7f, cosf(theta));
		ve.Color = m_color;
		m_vertex.emplace_back(ve);

		ve.Pos = XMFLOAT3(0.0f, 0.7f, 0.0f);
		ve.Normal = XMFLOAT3(sinf(theta), 0.7f, cosf(theta));
		ve.Color = m_color;
		m_vertex.emplace_back(ve);
	}

	// Vertices 最後は先頭の座標と同じ
	ve.Pos = m_vertex[0].Pos;
	m_vertex.emplace_back(ve);
	ve.Pos = m_vertex[1].Pos;
	m_vertex.emplace_back(ve);
}

// 描画
void Cone::Draw() {

	ID3D11DeviceContext* _devicecon = GetDX11DeviceContext();

	// 頂点バッファをセットする
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	_devicecon->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	_devicecon->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);			// インデックスバッファをセット
	_devicecon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);		// トポロジーをセット（旧プリミティブタイプ）
	_devicecon->IASetInputLayout(ShaderHashmap::GetInstance()->GetVertexLayout(m_VertexShaderName));					// 頂点レイアウトセット

	_devicecon->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_VertexShaderName), nullptr, 0);			// 頂点シェーダーをセット
	_devicecon->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_PixelShaderName), nullptr, 0);			// ピクセルシェーダーをセット

	_devicecon->DrawIndexed(
		static_cast<unsigned int>(m_face.size() * 3),		// 描画するインデックス数（面数×３）
		0,									// 最初のインデックスバッファの位置
		0);									// 頂点バッファの最初から使う
}

bool Cone::Init(
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
		_device,							// デバイスオブジェクト
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
// 毎フレーム更新を変更する
void Cone::BufferUpdate(XMFLOAT4 _color, Vector3 _pos)
{
	// 保存している色と異なる時のみ実行
	/*if (m_color.x != _color.x || m_color.y != _color.y || m_color.z != _color.z || m_color.z != _color.z) {*/

	HRESULT hr = S_OK;

	void* _date = m_vertex.data();

	for (int i = 0; i < VER * 2; ++i)
	{
		m_vertex[i].Color = _color;
		if (i % 2 == 1)
		{
			m_vertex[i].Pos.y = _pos.y;
		}
	}

	// bufferの更新
	D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };
	GetDX11DeviceContext()->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
	memcpy(MappedResource.pData, m_vertex.data(), sizeof(Vertex) * m_vertex.size()); // 3頂点分コピー
	GetDX11DeviceContext()->Unmap(m_pVertexBuffer.Get(), 0);

}


void Cone::SetTransform(Transform* _tra, Vector3 _pos, XMFLOAT4 _qt, float _Radius, float _size)
{
	DirectX::XMFLOAT4X4 make, mtx, scale;

	DX11MtxScale(_Radius * _size, _Radius * _size, _Radius * _size, scale);

	// 回転
	DirectX::XMFLOAT4 q, axis;
	axis = _tra->right();
	DX11QtRotationAxis(q, axis, -90);
	DX11QtMul(_qt, _qt, q);

	// qtから行列作成
	DX11MtxFromQt(mtx, _qt);
	DX11MtxMultiply(make, scale, mtx);

	make._41 = _pos.x;
	make._42 = _pos.y;
	make._43 = _pos.z;

	// バッファに設定
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, make);
}