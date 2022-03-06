#pragma once

#include "QuadRenderComponent.h"
#include "../DX11System/Shader.h"
#include "../DX11System/Memory.h"

namespace Component {
	class QuadMultiRender :public QuadRender {

	public:
		QuadMultiRender(GameObject* Owner, const char* _firstfilename, const char* _secondfilename);	// 1�ڃ��C���A2�ڍ�����
		~QuadMultiRender();

		void Start();
		void DrawObject();

		std::string GetSubTexFileName() { return texSubFileName; }			//�e�N�X�`�����̎擾
		std::string* GetSubTexFileNameAdress() { return &texSubFileName; }	//�A�h���X�n���i��{�g��Ȃ��j

		void ChangeSubTexture(const char* _fileName);						//���I�ȃe�N�X�`���ύX

		//==========UV=========//
		void SetVolume(float _num) { colorVolume.volume = _num; }			// �����F�̊����̃Z�b�g
		float SetVolume() { return colorVolume.volume; }					// �����F�̊����̎擾

		void ConstBufferInit();			// �萔�o�b�t�@�̏�����
	private:

		ALIGN16 struct ColorVolume
		{
			float volume;
		};

		std::string texSubFileName;		// �Q�߁@<������>

		ColorVolume colorVolume;			// �F�̊����̑傫��

		ID3D11Buffer* m_ColorVolumeBuffer = nullptr;	// �R���X�^���g�o�b�t�@ �����F�̊���
	};


}

