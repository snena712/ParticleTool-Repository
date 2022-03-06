#pragma once
#include "CDirectxGraphics.h"
#include "ImageResource.h"
#include <string>
#include <unordered_map>

//=======================================================================================//
//
//		ImageResourceManager�F�摜���\�[�X(ImageResource�N���X)�̃f�[�^�̊Ǘ����s��
//
//=======================================================================================//
class ImageResourceManager {

public:
	ImageResourceManager(const ImageResourceManager&) = delete;
	ImageResourceManager& operator=(const ImageResourceManager&) = delete;
	ImageResourceManager(ImageResourceManager&&) = delete;
	ImageResourceManager& operator=(ImageResourceManager&&) = delete;

	static ImageResourceManager* GetInstance() {
		static ImageResourceManager instance;
		return &instance;
	}

	void Exit();
	bool Load(std::string _fileName);
	bool Unload(std::string _fileName);
	void UnloadAll();
	ImageResource* GetResource(std::string _fileName);

private:
	ImageResourceManager(){}
	std::unordered_map<std::string, std::unique_ptr<ImageResource>> resourceMap;
};