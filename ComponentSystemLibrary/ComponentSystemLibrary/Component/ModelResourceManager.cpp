#include "ModelResourceManager.h"
#include "../DX11System/shaderhashmap.h"

std::map<unsigned int, ModelResource*> ModelResourceManager::MR_Map;

bool ModelResource::CreateModelData(const char* _datfileName, const char* _vsfileName, const char* _psfileName) {

	bool sts;

	// datfileのロード
	sts = datfile->Load(_datfileName,
		CDirectXGraphics::GetInstance()->GetDXDevice(),
		CDirectXGraphics::GetInstance()->GetImmediateContext());
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _datfileName);
		MessageBox(nullptr, str, "error", MB_OK);
		return false;
	}
	datFileName = _datfileName;	//ファイル名を保存


	// 頂点データの定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// 頂点シェーダーオブジェクトを生成、同時に頂点レイアウトも生成
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		_vsfileName,
		layout,
		numElements);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}
	vertexShaderName = _vsfileName;	//ファイル名を保存

	// ピクセルシェーダーを生成
	sts = ShaderHashmap::GetInstance()->SetPixelShader(_psfileName);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}
	pixelShaderName = _psfileName;	//ファイル名を保存

	return true;
}

void ModelResourceManager::Uninit() {
	UnLoadModelAll();
}

unsigned int ModelResourceManager::LoadModel(const char* _datfileName, const char* _vsfileName, const char* _psfileName) {

	bool sts;
	unsigned int newID = 1;	//キーID

	for (auto& data : MR_Map) {
		//一致するIDが存在するなら
		if (data.second->GetDatFileName() == _datfileName) {
			return data.first;
		}
		//IDのカウントアップ
		else if (data.first == newID) {
			newID++;
		}
	}

	//新しいキーに対応する新規データを作成
	MR_Map[newID] = new ModelResource();

	//モデルデータ・シェーダのロード・作成
	sts = MR_Map[newID]->CreateModelData(_datfileName, _vsfileName, _psfileName);
	if (!sts) {
		MessageBox(nullptr, "CreateModelData error", "error", MB_OK);
		return 0;
	}

	return newID;
}

void ModelResourceManager::UnLoadModel(unsigned int _ID) {



}

void ModelResourceManager::UnLoadModelAll() {

	// 解放
	MR_Map.clear();

}

ModelResource* ModelResourceManager::GetModelResource(unsigned int _ID) {
	// 存在するかを確かめる
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return MR_Map[_ID];
}

DatFile* ModelResourceManager::GetDatFile(unsigned int _ID) {
	// 存在するかを確かめる
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return MR_Map[_ID]->GetDatFile();
}

ID3D11VertexShader* ModelResourceManager::GetVertexShader(unsigned int _ID) {

	// 存在するかを確かめる
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return ShaderHashmap::GetInstance()->GetVertexShader(MR_Map[_ID]->GetVSFileName());
}

ID3D11PixelShader* ModelResourceManager::GetPixelShader(unsigned int _ID) {
	// 存在するかを確かめる
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return ShaderHashmap::GetInstance()->GetPixelShader(MR_Map[_ID]->GetPSFileName());
}

ID3D11InputLayout* ModelResourceManager::GetVertexLayout(unsigned int _ID) {
	// 存在するかを確かめる
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return ShaderHashmap::GetInstance()->GetVertexLayout(MR_Map[_ID]->GetVSFileName());
}

ComPtr<ID3D11ShaderResourceView> ModelResourceManager::GetTexSRV(unsigned int _ID) {
	// 存在するかを確かめる
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return MR_Map[_ID]->GetSRV();
}
