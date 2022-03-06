#include "ImageResourceManager.h"

void ImageResourceManager::Exit() {
	UnloadAll();
}

//���[�h����
bool ImageResourceManager::Load(std::string _fileName)
{
	auto itr = resourceMap.find(_fileName);
	if (itr != resourceMap.end())
	{
		return true;	//���[�h�ς݂Ȃ̂�true��Ԃ�
	}

	//�V�K�f�[�^���쐬
	std::unique_ptr<ImageResource> newTexture;
	newTexture = std::make_unique<ImageResource>();

	//�摜�f�[�^�̃��[�h
	bool sts = newTexture->Load(_fileName);

	if (!sts)
	{
		return false;
	}

	//unique_ptr�̃|�C���^���L���̈ڂ��ւ�
	resourceMap[_fileName].swap(newTexture);
	return true;
}

//���[�h�ς݂̉摜�f�[�^�̉��
bool ImageResourceManager::Unload(std::string _fileName)
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

//���[�h�ς݂̉摜�f�[�^�̉��
void ImageResourceManager::UnloadAll() {

	for (auto itr = resourceMap.begin(); itr != resourceMap.end(); ++itr)
	{
		itr->second.reset();
	}
	resourceMap.clear();
}

//���[�h�ς݉摜�f�[�^�̑S���
ImageResource* ImageResourceManager::GetResource(std::string _fileName)
{
	// ���݂��邩���m���߂�
	auto itr = resourceMap.find(_fileName);
	if (itr == resourceMap.end())
	{
		return nullptr;
	}

	return resourceMap[_fileName].get();
}
