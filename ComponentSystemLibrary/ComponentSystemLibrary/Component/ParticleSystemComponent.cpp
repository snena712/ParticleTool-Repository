#include "ParticleSystemComponent.h"
#include "../DX11System/CDirectxGraphics.h"
#include "../DX11System/shaderhashmap.h"
#include "../DX11System/DX11Settransform.h"
#include "../DX11System/dx11mathutil.h"
#include "../DX11System/quad.h"

using namespace Component;

std::vector<ParticleSystem::Particle> ParticleSystem::particleArray;// パーティクルの管理配列
std::vector<ParticleSystem*> ParticleSystem::particleSystemArray;	// パーティクルシステムの管理配列
DirectX::XMFLOAT4X4	ParticleSystem::m_worldmtx;						// ワールド変換行列
ComPtr<ID3D11Buffer>	ParticleSystem::m_vertexbuffer;				// 頂点バッファ

ParticleSystem::ParticleSystem(GameObject* Owner) :ComponentBase(Owner) {

	particleSystemArray.emplace_back(this);

}

ParticleSystem::~ParticleSystem() {

	for (int i = 0; i < particleSystemArray.size(); i++) {

		if (particleSystemArray[i] == this) {
			particleSystemArray.erase(particleSystemArray.begin() + i);
			break;
		}
	}
}

void ParticleSystem::SystemInit() {

	DX11MtxIdentity(m_worldmtx);	// 初期姿勢

	// ピクセルシェーダーを生成
	bool sts = ShaderHashmap::GetInstance()->SetPixelShader("shader/particleps.hlsl");
	if (!sts) {
		MessageBox(nullptr, "SetPixelShader error(basicps.hlsl)", "error", MB_OK);
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
		"shader/particlevs.hlsl",	// 頂点シェーダーファイル名
		layout,						// 頂点レイアウト
		numElements);				// エレメント数

	// デバイスを取得
	ID3D11Device* device;
	device = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点バッファを生成
	sts = CreateVertexBufferWrite(
		device,						// デバイスオブジェクト
		sizeof(Particle),			// １頂点当たりバイト数
		particleArray.size(),		// 頂点数
		&particleArray[0],			// 頂点データ格納メモリ先頭アドレス
		&m_vertexbuffer				// 頂点バッファ
	);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexBuffer error", "error", MB_OK);
	}

	// ジオメトリシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetGeometryShader("shader/particlevs.hlsl");
}

void ParticleSystem::SystemUninit() {

}

void ParticleSystem::Update() {

}

void ParticleSystem::Draw() {

	if (particleArray.size() > 0) {
		// デバイスコンテキストを取得
		ID3D11DeviceContext* devcontext;
		devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

		// 座標変換用の行列をセット
		DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::WORLD, m_worldmtx);

		unsigned int stride = sizeof(Particle);	// ストライドをセット（１頂点当たりのバイト数）
		unsigned  offset = 0;					// オフセット値をセット

		// 頂点バッファをデバイスコンテキストへセット
		devcontext->IASetVertexBuffers(
			0,									// スタートスロット
			1,									// 頂点バッファ個数
			m_vertexbuffer.GetAddressOf(),		// 頂点バッファの先頭アドレス
			&stride,							// ストライド
			&offset);							// オフセット

		// トポロジーをセット
		devcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// 頂点シェーダー、ジオメトリシェーダー、ピクセルシェーダー取得
		ID3D11VertexShader* vs;
		vs = ShaderHashmap::GetInstance()->GetVertexShader("shader/particlevs.hlsl");
		ID3D11GeometryShader* gs;
		gs = ShaderHashmap::GetInstance()->GetGeometryShader("shader/particlegs.hlsl");
		ID3D11PixelShader* ps;
		ps = ShaderHashmap::GetInstance()->GetPixelShader("shader/particleps.hlsl");

		// 頂点レイアウト取得
		ID3D11InputLayout* layout;
		layout = ShaderHashmap::GetInstance()->GetVertexLayout("shader/particlevs.hlsl");

		devcontext->VSSetShader(vs, nullptr, 0);
		devcontext->GSSetShader(gs, nullptr, 0);
		devcontext->HSSetShader(nullptr, nullptr, 0);
		devcontext->DSSetShader(nullptr, nullptr, 0);
		devcontext->PSSetShader(ps, nullptr, 0);

		// 頂点フォーマットをセット
		devcontext->IASetInputLayout(layout);

		// ドローコール発行
		devcontext->Draw(
			particleArray.size(),	// 頂点数
			0);						// 開始位置
	}
}

// 頂点バッファ更新
void ParticleSystem::updateVbuffer() {
	D3D11_MAPPED_SUBRESOURCE pData;
	ID3D11DeviceContext* devcontext;
	devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();

	HRESULT hr = devcontext->Map(m_vertexbuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	if (SUCCEEDED(hr)) {
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&particleArray[0]), sizeof(Particle) * particleArray.size());
		devcontext->Unmap(m_vertexbuffer.Get(), 0);
	}
}