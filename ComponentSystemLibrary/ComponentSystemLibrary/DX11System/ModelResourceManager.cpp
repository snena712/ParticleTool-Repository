#include "ModelResourceManager.h"

void ModelResourceManager::Exit() {
	UnloadAll();
}

//ロード処理
bool ModelResourceManager::Load(std::string _fileName, std::string _texFolder)
{
	auto itr = resourceMap.find(_fileName);
	if (itr != resourceMap.end())
	{
		return true;	//ロード済みなのでtrueを返す
	}

	//新規データを作成
	std::unique_ptr<ModelData> newModel;
	newModel = std::make_unique<ModelData>();

	//モデルデータのロード
	bool sts = newModel->Load(_texFolder, _fileName);

	if (!sts)
	{
		return false;
	}

	//unique_ptrのポインタ所有権の移し替え
	resourceMap[_fileName].swap(newModel);
	return true;
}

//ロード済みのモデルデータの解放
bool ModelResourceManager::Unload(std::string _fileName)
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

//ロード済みモデルデータの全解放
void ModelResourceManager::UnloadAll()
{
	for (auto itr = resourceMap.begin(); itr != resourceMap.end(); ++itr)
	{
		itr->second.reset();
	}
	resourceMap.clear();
}

//ロード済みのモデルを検索し、場所を返す
ModelData* ModelResourceManager::GetResource(std::string _fileName)
{
	// 存在するかを確かめる
	auto itr = resourceMap.find(_fileName);
	if (itr == resourceMap.end())
	{
		return nullptr;
	}

	return resourceMap[_fileName].get();
}
