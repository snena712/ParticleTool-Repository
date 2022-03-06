#include "ImageResourceManager.h"

void ImageResourceManager::Exit() {
	UnloadAll();
}

//ロード処理
bool ImageResourceManager::Load(std::string _fileName)
{
	auto itr = resourceMap.find(_fileName);
	if (itr != resourceMap.end())
	{
		return true;	//ロード済みなのでtrueを返す
	}

	//新規データを作成
	std::unique_ptr<ImageResource> newTexture;
	newTexture = std::make_unique<ImageResource>();

	//画像データのロード
	bool sts = newTexture->Load(_fileName);

	if (!sts)
	{
		return false;
	}

	//unique_ptrのポインタ所有権の移し替え
	resourceMap[_fileName].swap(newTexture);
	return true;
}

//ロード済みの画像データの解放
bool ImageResourceManager::Unload(std::string _fileName)
{
	// 存在するかを確かめる
	auto itr = resourceMap.find(_fileName);
	if (itr != resourceMap.end())
	{
		resourceMap[_fileName].reset();
		resourceMap.erase(_fileName);
		return true;
	}

	return false;
}

//ロード済みの画像データの解放
void ImageResourceManager::UnloadAll() {

	for (auto itr = resourceMap.begin(); itr != resourceMap.end(); ++itr)
	{
		itr->second.reset();
	}
	resourceMap.clear();
}

//ロード済み画像データの全解放
ImageResource* ImageResourceManager::GetResource(std::string _fileName)
{
	// 存在するかを確かめる
	auto itr = resourceMap.find(_fileName);
	if (itr == resourceMap.end())
	{
		return nullptr;
	}

	return resourceMap[_fileName].get();
}
