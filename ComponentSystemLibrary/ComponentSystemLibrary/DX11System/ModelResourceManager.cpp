#include "ModelResourceManager.h"

void ModelResourceManager::Exit() {
	UnloadAll();
}

//���[�h����
bool ModelResourceManager::Load(std::string _fileName, std::string _texFolder)
{
	auto itr = resourceMap.find(_fileName);
	if (itr != resourceMap.end())
	{
		return true;	//���[�h�ς݂Ȃ̂�true��Ԃ�
	}

	//�V�K�f�[�^���쐬
	std::unique_ptr<ModelData> newModel;
	newModel = std::make_unique<ModelData>();

	//���f���f�[�^�̃��[�h
	bool sts = newModel->Load(_texFolder, _fileName);

	if (!sts)
	{
		return false;
	}

	//unique_ptr�̃|�C���^���L���̈ڂ��ւ�
	resourceMap[_fileName].swap(newModel);
	return true;
}

//���[�h�ς݂̃��f���f�[�^�̉��
bool ModelResourceManager::Unload(std::string _fileName)
{
	// ���݂��邩���m���߂�
	auto itr = resourceMap.find(_fileName);
	if (itr != resourceMap.end())
	{
		resourceMap[_fileName].reset();
		resourceMap.erase(_fileName);
		return true;
	}

	return false;
}

//���[�h�ς݃��f���f�[�^�̑S���
void ModelResourceManager::UnloadAll()
{
	for (auto itr = resourceMap.begin(); itr != resourceMap.end(); ++itr)
	{
		itr->second.reset();
	}
	resourceMap.clear();
}

//���[�h�ς݂̃��f�����������A�ꏊ��Ԃ�
ModelData* ModelResourceManager::GetResource(std::string _fileName)
{
	// ���݂��邩���m���߂�
	auto itr = resourceMap.find(_fileName);
	if (itr == resourceMap.end())
	{
		return nullptr;
	}

	return resourceMap[_fileName].get();
}
