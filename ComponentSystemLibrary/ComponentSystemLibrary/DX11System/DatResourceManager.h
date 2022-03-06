#pragma once
#include "CDirectxGraphics.h"
#include "DatFile.h"
#include <string>
#include <map>


//=================================================================================//
//
//		ModelResource：ModelResourceManagerで管理する3Dモデルリソース単体のデータ
//
//=================================================================================//
class DatResource
{
private:
	std::string				datFileName;				// datファイル名
	DatFile*				datfile;					// datファイル
	std::string				vertexShaderName;			// 頂点シェーダー名（頂点レイアウトと共通）
	std::string				pixelShaderName;			// ピクセルシェーダー名

	ComPtr<ID3D11ShaderResourceView> texSRV = nullptr;		// テクスチャＳＲＶ

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
//		ModelResourceManager：3Dモデルリソース(ModelResourceクラス)のデータの管理を行う
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

	static DatResource* GetModelResource(unsigned int _ID);		//モデルリソースの取得

	static DatFile* GetDatFile(unsigned int _ID);					//モデルリソースのDFの取得
	static ID3D11VertexShader* GetVertexShader(unsigned int _ID);	//モデルリソースのVSの取得
	static ID3D11PixelShader* GetPixelShader(unsigned int _ID);		//モデルリソースのPSの取得
	static ID3D11InputLayout* GetVertexLayout(unsigned int _ID);	//モデルリソースのVLの取得
	static ComPtr<ID3D11ShaderResourceView> GetTexSRV(unsigned int _ID);
};

