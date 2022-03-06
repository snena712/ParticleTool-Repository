#include	"Sphere.h"

// 法線ベクトルを計算
// ノーマライズ
void Sphere::Normalize(XMFLOAT3 vector, XMFLOAT3& Normal) {
	DX11Vec3Normalize(Normal, Normal);
}

// インデックスデータを作成
void Sphere::CreateIndex() {

	// インデックス生成
	for (unsigned int y = 0; y < m_divY; y++) {
		for (unsigned int x = 0; x < m_divX; x++) {
			int count = (m_divX + 1) * y + x;			// 左上座標のインデックス

			Face f;
			// 上半分
			f.idx[0] = count;
			f.idx[1] = count + 1;
			f.idx[2] = count + 1 + (m_divX + 1);

			m_face.emplace_back(f);

			// 下半分
			f.idx[0] = count;
			f.idx[1] = count + (m_divX + 1) + 1;
			f.idx[2] = count + (m_divX + 1);

			m_face.emplace_back(f);

		}
	}
}

// 頂点データを作成
void Sphere::CreateVertex() {

	float azimuth = 0.0f;			// 方位角
	float elevation = 0.0f;			// 仰角

	XMFLOAT3	Normal;
	// 方位角と仰角から球メッシュの頂点データを作成
	for (unsigned int y = 0; y <= m_divY; y++) {
		elevation = (PI * (float)y) / (float)m_divY;    // 仰角をセット
		float r = m_radius * sinf(elevation);				// 仰角に応じた半径を計算

		for (unsigned int x = 0; x <= m_divX; x++) {
			azimuth = (2 * PI * (float)x) / (float)m_divX;	// 方位角をセット

			Vertex v;
			v.Pos.x = r * cosf(azimuth);
			v.Pos.y = m_radius * cosf(elevation);
			v.Pos.z = r * sinf(azimuth);

			Normalize(v.Pos, Normal);		// 法線を計算
			v.Normal = Normal;				// 法線をセット

			v.Color = m_color;				// 頂点カラー

			m_vertex.emplace_back(v);
		}
	}
}

// 描画
void Sphere::Draw(unsigned int _num, ParticleBase* _particleBase) {

	ID3D11DeviceContext *_devicecon = GetDX11DeviceContext();

	// 頂点バッファをセットする
	unsigned int stride = sizeof(Vertex);
	unsigned  offset = 0;
	_devicecon->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);

	_devicecon->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);						// インデックスバッファをセット
	_devicecon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);							// トポロジーをセット（旧プリミティブタイプ）
	_devicecon->IASetInputLayout(ShaderHashmap::GetInstance()->GetVertexLayout(m_VertexShaderName));	// 頂点レイアウトセット

	_devicecon->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_VertexShaderName), nullptr, 0);			// 頂点シェーダーをセット
	_devicecon->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_PixelShaderName), nullptr, 0);			// ピクセルシェーダーをセット

	//ColorUpdate(XMFLOAT4(1,1,1,1));

	for (int i = 0; i < _num; i++)
	{
		SetTransform(_particleBase[i].pos, 1, _particleBase[i].scale.x);
		// 数の定位数バッファの更新

		// 描画数分ドローコール掛ける
		_devicecon->DrawIndexed(
			static_cast<unsigned int>(m_face.size()*3),		// 描画するインデックス数（面数×３）
			0,									// 最初のインデックスバッファの位置
			0);									// 頂点バッファの最初から使う
	}
}

bool Sphere::Init(float r,				// 半径
	int division_horizontal,			// 水平方向の分割数
	int division_vertical,				// 垂直方向の分割数
	std::string _VS,					// 頂点シェーダー
	std::string _PS)					// ピクセルシェーダー
{

	bool sts;
	// 分割数を保存
	m_divX = division_horizontal;
	m_divY = division_vertical;

	// 半径を保存
	m_radius = r;

	// カラー値を保存
	//m_color = color;

	// デバイスの設定
	ID3D11Device* _device = GetDX11Device();

	// 球のインデックスデータを作成
	CreateIndex();

	// 球の頂点データを作成
	CreateVertex();

	// シェーダー名設定
	m_VertexShaderName = _VS;
	m_PixelShaderName = _PS;

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

// 頂点バッファの色更新
// 毎フレーム更新を変更時のみに変更する
void Sphere::ColorUpdate(XMFLOAT4 _color)
{
	// 保存している色と異なる時のみ実行
	if (m_color.x != _color.x || m_color.y != _color.y || m_color.z != _color.z || m_color.z != _color.z) {

		HRESULT hr = S_OK;

		void* _date = m_vertex.data();

		// bufferの更新
		D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };
		hr = GetDX11DeviceContext()->Map(m_pVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		if (SUCCEEDED(hr)) {
			Vertex* pBuffer = (Vertex*)MappedResource.pData;
			if (pBuffer[0].Color.x != 0 || pBuffer[0].Color.y != 0 || pBuffer[0].Color.z != 0 || pBuffer[0].Color.w != 0)
			{
				m_color = _color;
				for (int i = 0; i < m_divX * m_divY; ++i)
				{
					pBuffer[i].Color = _color;
				}
			}
			GetDX11DeviceContext()->Unmap(m_pVertexBuffer.Get(), 0);
		}
	}
}

void Sphere::SetTransform(XMFLOAT3 _pos, float _Radius, float _size)
{
	DirectX::XMFLOAT4X4 make, mtx, scale;

	DX11MtxScale(_Radius * _size, _Radius * _size, _Radius * _size, scale);
	DX11MtxFromQt(mtx, XMFLOAT4(0, 0, 0, 1));
	DX11MtxMultiply(make, scale, mtx);

	make._41 = _pos.x+10;
	make._42 = _pos.y;
	make._43 = _pos.z;

	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, make);
}