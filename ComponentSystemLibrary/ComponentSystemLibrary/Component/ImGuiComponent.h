#pragma once

#include "../Application.h"

#ifdef DEBUG

#include "ComponentBaseClass.h"
#include "../DX11System/quad.h"
#include <string>
#include <vector>
#include "../DX11System/Vector.h"
#include <string>
#include <cctype>
#include <stdio.h>

#include "../imGui/imgui.h"
#include "../imGui/imgui_impl_win32.h"
#include "../imGui/imgui_impl_dx11.h"
#include "D3D11.h"

// �e��t���O
class Flgs
{
public:
	bool debug = false;
	bool firstdebugflg = false;	// �f�o�b�O�p
	bool dend = false;
};

// ImGui�Ǘ��N���X
class ImGuiManager {
private:
	enum class PlayMode
	{
		Debug,
		Release,
	};
	//---------�摜�p�ϐ�-------------//
	static const std::string startImage;
	static const std::string stopImage;

	//------------�ϐ�-----------//
	PlayMode	playMode;						// ���s���[�h
	bool		firstflg;						// ����I�[�v���i�|�W�V�����w��j
	bool        pauseflg = false;				// �ꎞ��~
	bool        oneFlameFlg = false;			// 1�t���[���i�߂�
	int objcnt;									// �I�u�W�F�N�g�̐�
	Flgs flgs;									// �X�V�t���O

public:
	//-------------------------��{����---------------------------//
	static ImGuiManager* Instance() {
		static ImGuiManager singleton;
		return &singleton;
	}

	ImGuiManager();
	//~ImGuiManager();

	// ���C�����[�v�Ŏg�p
	void ImGuiUpdate();
	void ImGuiRender();
	void ImGuiEnd();

	//-------------------�f�o�b�O���[�h�̑���---------------------//
	bool CheckPlayMode();											// ImGui�e�̓���m�F
	void DebugStop() { playMode = PlayMode::Release; }				// ImGui�f�o�b�O���Ȃ�
	void DebugPlay() { playMode = PlayMode::Debug; }				// ImGui�f�o�b�O����

	void Pause();													// �|�[�Y�̏���
	void OneFrame();												// �P�t���[���i�߂�

	//-------------------�t���O�擾�֐�---------------------//
	Flgs GetFlg() { return flgs; }									// �t���O�Ǘ��N���X�̕Ԃ�
	bool GetPause() { return pauseflg; }							// �|�[�Y�̗L��
};

#endif
