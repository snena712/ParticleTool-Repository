#include "Skydome.h"
#include <string>
#include "../Component/CameraComponent.h"
#include "../DX11System/ModelResourceManager.h"
#include "../DX11System/shaderhashmap.h"
#include "../DX11System/DX11Settransform.h"
#include "../DX11System/dx11mathutil.h"

void Skydome::Init(const char* _filename, const char* _vsfile, const char* _psfile, const char* _texfolder) {

	bool sts;

	sts = ModelResourceManager::GetInstance()->Load(_filename, _texfolder);
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _filename);
		MessageBox(nullptr, str, "error", MB_OK);
		return;
	}

	m_assimpfileName = _filename;	//ファイル名を保存

	// 頂点データの定義（アニメーション対応）
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	unsigned int numElements = ARRAYSIZE(layout);

	ID3D11Device* dev;
	dev = CDirectXGraphics::GetInstance()->GetDXDevice();

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		_vsfile,
		layout,
		numElements);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return;
	}
	m_VertexShaderName = _vsfile;	//ファイル名を保存

	// ピクセルシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetPixelShader(_psfile);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return;
	}
	m_PixelShaderName = _psfile;	//ファイル名を保存

	m_drawExists = true;
	m_scale = 1.0f;
}

void Skydome::Exit() {
	ModelResourceManager::GetInstance()->Unload(m_assimpfileName);
}

void Skydome::Draw() {

	if (m_drawExists)
	{

		DirectX::XMFLOAT4X4 make, mtx, scale;

		DX11MtxIdentity(mtx);
		DX11MtxScale(m_scale, m_scale, m_scale, scale);
		DX11MtxMultiply(make, scale, mtx);

		ID3D11DeviceContext* devcontext;			// デバイスコンテキスト
		devcontext = CDirectXGraphics::GetInstance()->GetImmediateContext();
		// 頂点フォーマットをセット
		devcontext->IASetInputLayout(ShaderHashmap::GetInstance()->GetVertexLayout(m_VertexShaderName));
		// 頂点シェーダーをセット
		devcontext->VSSetShader(ShaderHashmap::GetInstance()->GetVertexShader(m_VertexShaderName), nullptr, 0);
		// ピクセルシェーダーをセット
		devcontext->PSSetShader(ShaderHashmap::GetInstance()->GetPixelShader(m_PixelShaderName), nullptr, 0);
		// 描画
		ModelData* md = ModelResourceManager::GetInstance()->GetResource(m_assimpfileName);
		if (md != nullptr) {
			md->Draw(devcontext, make);
		}
	}
}
