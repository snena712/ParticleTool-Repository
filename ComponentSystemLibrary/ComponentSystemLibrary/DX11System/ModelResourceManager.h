#pragma once
#include "CDirectxGraphics.h"
#include "ModelData.h"
#include <string>
#include <unordered_map>

//=======================================================================================//
//
//		ModelResourceManager�F3D���f�����\�[�X(ModelData�N���X)�̃f�[�^�̊Ǘ����s��
//
//=======================================================================================//
class ModelResourceManager
{
public:
	ModelResourceManager(const ModelResourceManager&) = delete;
	ModelResourceManager& operator=(const ModelResourceManager&) = delete;
	ModelResourceManager(ModelResourceManager&&) = delete;
	ModelResourceManager& operator=(ModelResourceManager&&) = delete;

	static ModelResourceManager* GetInstance() {
		static ModelResourceManager instance;
		return &instance;
	}

	void Exit();
	bool Load(std::string _fileName, std::string _texFolder);
	bool Unload(std::string _fileName);
	void UnloadAll();
	ModelData* GetResource(std::string _fileName);

private:
	ModelResourceManager() {}
	std::unordered_map<std::string, std::unique_ptr<ModelData>> resourceMap;
};

