#pragma once
#include "CDirectxGraphics.h"
#include "DatFile.h"
#include <string>
#include <map>


//=================================================================================//
//
//		ModelResource�FModelResourceManager�ŊǗ�����3D���f�����\�[�X�P�̂̃f�[�^
//
//=================================================================================//
class DatResource
{
private:
	std::string				datFileName;				// dat�t�@�C����
	DatFile*				datfile;					// dat�t�@�C��
	std::string				vertexShaderName;			// ���_�V�F�[�_�[���i���_���C�A�E�g�Ƌ��ʁj
	std::string				pixelShaderName;			// �s�N�Z���V�F�[�_�[��

	ComPtr<ID3D11ShaderResourceView> texSRV = nullptr;		// �e�N�X�`���r�q�u

public:
	DatResource() { datfile = new DatFile(); }
	~DatResource(){
		if (datfile != nullptr)
			delete datfile;
		datFileName.clear();
		vertexShaderName.clear();
		pixelShaderName.clear();
	}

	DatFile* GetDatFile() { return datfile; }

	std::string GetDatFileName() const { return datFileName; }
	void SetDatFileName(std::string _fileName) { datFileName = _fileName; }

	std::string GetVSFileName() const { return vertexShaderName; }
	void SetVSFileName(std::string _fileName) { vertexShaderName = _fileName; }
	
	std::string GetPSFileName() const { return pixelShaderName; }
	void SetPSFileName(std::string _fileName) { pixelShaderName = _fileName; }

	ComPtr<ID3D11ShaderResourceView> GetSRV() const { return texSRV; }
	void SetSRV(ComPtr<ID3D11ShaderResourceView> newSrv) { texSRV = newSrv; }

	bool CreateModelData(const char* datfileName, const char* vsfileName, const char* psfileName);

};

//=======================================================================================//
//
//		ModelResourceManager�F3D���f�����\�[�X(ModelResource�N���X)�̃f�[�^�̊Ǘ����s��
//
//=======================================================================================//
class DatResourceManager
{
private:
	static std::map<unsigned int, DatResource*> MR_Map;

public:
	static void Uninit();

	static unsigned int LoadModel(const char* _datfileName, const char* _vsfileName, const char* _psfileName);
	static void UnLoadModel(unsigned int _ID);
	static void UnLoadModelAll();

	static DatResource* GetModelResource(unsigned int _ID);		//���f�����\�[�X�̎擾

	static DatFile* GetDatFile(unsigned int _ID);					//���f�����\�[�X��DF�̎擾
	static ID3D11VertexShader* GetVertexShader(unsigned int _ID);	//���f�����\�[�X��VS�̎擾
	static ID3D11PixelShader* GetPixelShader(unsigned int _ID);		//���f�����\�[�X��PS�̎擾
	static ID3D11InputLayout* GetVertexLayout(unsigned int _ID);	//���f�����\�[�X��VL�̎擾
	static ComPtr<ID3D11ShaderResourceView> GetTexSRV(unsigned int _ID);
};

