#include "ModelResourceManager.h"
#include "../DX11System/shaderhashmap.h"

std::map<unsigned int, ModelResource*> ModelResourceManager::MR_Map;

bool ModelResource::CreateModelData(const char* _datfileName, const char* _vsfileName, const char* _psfileName) {

	bool sts;

	// datfile�̃��[�h
	sts = datfile->Load(_datfileName,
		CDirectXGraphics::GetInstance()->GetDXDevice(),
		CDirectXGraphics::GetInstance()->GetImmediateContext());
	if (!sts) {
		char str[128];
		sprintf_s(str, 128, "%s load ERROR!!", _datfileName);
		MessageBox(nullptr, str, "error", MB_OK);
		return false;
	}
	datFileName = _datfileName;	//�t�@�C������ۑ�


	// ���_�f�[�^�̒�`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,	    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int numElements = ARRAYSIZE(layout);

	// ���_�V�F�[�_�[�I�u�W�F�N�g�𐶐��A�����ɒ��_���C�A�E�g������
	sts = ShaderHashmap::GetInstance()->SetVertexShader(
		_vsfileName,
		layout,
		numElements);
	if (!sts) {
		MessageBox(nullptr, "CreateVertexShader error", "error", MB_OK);
		return false;
	}
	vertexShaderName = _vsfileName;	//�t�@�C������ۑ�

	// �s�N�Z���V�F�[�_�[�𐶐�
	sts = ShaderHashmap::GetInstance()->SetPixelShader(_psfileName);
	if (!sts) {
		MessageBox(nullptr, "CreatePixelShader error", "error", MB_OK);
		return false;
	}
	pixelShaderName = _psfileName;	//�t�@�C������ۑ�

	return true;
}

void ModelResourceManager::Uninit() {
	UnLoadModelAll();
}

unsigned int ModelResourceManager::LoadModel(const char* _datfileName, const char* _vsfileName, const char* _psfileName) {

	bool sts;
	unsigned int newID = 1;	//�L�[ID

	for (auto& data : MR_Map) {
		//��v����ID�����݂���Ȃ�
		if (data.second->GetDatFileName() == _datfileName) {
			return data.first;
		}
		//ID�̃J�E���g�A�b�v
		else if (data.first == newID) {
			newID++;
		}
	}

	//�V�����L�[�ɑΉ�����V�K�f�[�^���쐬
	MR_Map[newID] = new ModelResource();

	//���f���f�[�^�E�V�F�[�_�̃��[�h�E�쐬
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

	// ���
	MR_Map.clear();

}

ModelResource* ModelResourceManager::GetModelResource(unsigned int _ID) {
	// ���݂��邩���m���߂�
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return MR_Map[_ID];
}

DatFile* ModelResourceManager::GetDatFile(unsigned int _ID) {
	// ���݂��邩���m���߂�
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return MR_Map[_ID]->GetDatFile();
}

ID3D11VertexShader* ModelResourceManager::GetVertexShader(unsigned int _ID) {

	// ���݂��邩���m���߂�
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return ShaderHashmap::GetInstance()->GetVertexShader(MR_Map[_ID]->GetVSFileName());
}

ID3D11PixelShader* ModelResourceManager::GetPixelShader(unsigned int _ID) {
	// ���݂��邩���m���߂�
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return ShaderHashmap::GetInstance()->GetPixelShader(MR_Map[_ID]->GetPSFileName());
}

ID3D11InputLayout* ModelResourceManager::GetVertexLayout(unsigned int _ID) {
	// ���݂��邩���m���߂�
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return ShaderHashmap::GetInstance()->GetVertexLayout(MR_Map[_ID]->GetVSFileName());
}

ComPtr<ID3D11ShaderResourceView> ModelResourceManager::GetTexSRV(unsigned int _ID) {
	// ���݂��邩���m���߂�
	auto it = MR_Map.find(_ID);
	if (it == MR_Map.end()) {
		return nullptr;
	}

	return MR_Map[_ID]->GetSRV();
}
