#include "ImGuiComponent.h"

#ifdef DEBUG

#include "Transform.h"
#include "RigidbodyComponent.h"
#include "ColliderBase.h"
#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "SphereColliderComponent.h"
#include "CameraComponent.h"
#include "AnimationComponent.h"
#include "QuadRenderComponent.h"
#include "ModelComponent.h"
#include "../DX11System/CDirectInput.h"
#include "../DX11System/ImageResourceManager.h"
#include "../DX11System/ModelResourceManager.h"

#include "SceneManager.h"

using namespace Component;

bool imageflg = false;

const std::string ImGuiManager::startImage = "imGui/GUISprite/ImGuiStart.png";
const std::string ImGuiManager::stopImage = "imGui/GUISprite/ImGuiStop.jpg";

// �I�[�i�[�̃f�o�b�O�t���O�m�F
bool ImGuiManager::CheckPlayMode()
{
	return ((ImGuiManager::GetFlg().debug) ? true : false);
}

// ImGui�̍X�V����
void ImGuiManager::ImGuiUpdate() {
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_P))
	{
		if (!flgs.debug) {
			flgs.debug = true;
		}
		else
		{
			flgs.debug = false;
		}
	}

	//imGui�̍X�V����
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//-----------------�ύX�_------------------//
	// �|�[�Y
	Pause();

	// �f�o�b�O���s
	if (flgs.debug) {

		// ������s���̏ꏊ�w��
		if (!flgs.firstdebugflg) {
			//ImGui::SetNextWindowPos(ImVec2(0, 0));
			flgs.firstdebugflg = true;
		}
		ImGui::SetNextWindowSize(ImVec2(200, 600), ImGuiCond_Once);
		ImGui::Begin(u8"�f�o�b�O��", &flgs.debug);

		// �t���[�����[�g��\��
		ImGui::Text(u8"���݂�FPS : %.1f FPS", ImGui::GetIO().Framerate);

		// �V�[�����̕\��
		ImGui::Text(u8"���݂̃V�[���� : %s", SceneManager::Instance()->GetActiveScene()->GetSceneName().c_str());

		// �I�u�W�F�N�g�̐���\��
		ImGui::Text(u8"���݂̃Q�[���I�u�W�F�N�g�� : %d", objcnt);
		objcnt = 0;


		// ���󃊃X�g�ɂ�����̑S�Ă��v�Z����
		for (auto& _objList : *SceneManager::Instance()->GetEntityObject())
		{
			string ownname = _objList.obj->GetName();
			const char* bbb = ownname.c_str();
			if (ImGui::TreeNode(bbb)) {

				ImGui::BeginChild(bbb, ImVec2(0, 30), false);
				ImGui::Text("%s", bbb);
				ImGui::SameLine();
				ImGui::Checkbox("open?", _objList.obj->GetDebugAdress());

				ImGui::EndChild();

				if (_objList.obj->transform->GetChild(0) == nullptr)
				{
					ImGui::Text(u8"�Ȃ�");
				}

				// ����100�܂� ��transform�Ɏq���̑傫���擾���
				for (auto i = 0; i < 100; i++)
				{
					if (_objList.obj->transform->GetChild(i) != nullptr)
					{
						string ownname = _objList.obj->transform->GetChild(i)->gameObject->GetName();
						const char* bbb = ownname.c_str();

						ImGui::BeginChild(bbb, ImVec2(0, 30), false);
						ImGui::Text("%s", bbb);
						ImGui::SameLine();
						ImGui::Checkbox("open?", _objList.obj->transform->GetChild(i)->gameObject->GetDebugAdress());
						ImGui::EndChild();
						ImGui::Separator();
					}
					else {
						break;
					}
				}

				ImGui::TreePop();
			}
		}

		// �t���O���Ȃ��ꍇ�I��
		if (!flgs.debug)
		{
			flgs.dend = true;
		}

		// �f�o�b�O�̃t���O���I���̕���T���A�\������
		for (auto& _objList : *SceneManager::Instance()->GetEntityObject())
		{
			objcnt++;
			if (_objList.obj->GetDebug())
			{
				string ownname = _objList.obj->GetName();
				const char* bbb = ownname.c_str();

				ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Once);
				ImGui::Begin("Inspector", _objList.obj->GetDebugAdress());
				ImGui::Text(u8"*���O:%s", bbb);

				if (ImGuiManager::Instance()->CheckPlayMode())
				{
					_objList.obj->transform->ImGuiDebug();

					for (auto& com : _objList.obj->GetComponentList())
					{
						com->ImGuiDebug();	//�S�R���|�[�l���g�̃f�o�b�O�֐����s
					}
				}

				ImGui::End();
			}
		}
	}
}

// ImGui�̕`��X�V����
void ImGuiManager::ImGuiRender()
{
	//imGui�̍X�V����
	if (flgs.debug)
	{
		ImGui::End();
	}
	if (flgs.dend)
	{
		ImGui::End();
		flgs.dend = false;
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

// ImGui�̏I������
void ImGuiManager::ImGuiEnd()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//-----------------�ύX�_------------------//
// �������牺
// �|�[�Y�̏���
void ImGuiManager::Pause()
{
	//ImGui::SetNextWindowPos(ImVec2(Application::Instance()->CLIENT_WIDTH / 2 - 300 / 2, 0));   //��ʈʒu���O������擾�ł���悤�ɂ���

	if (flgs.debug)
	{
		float my_tex_w = 100;
		float my_tex_h = 100;

		int frame_padding = -1;								// -1 == uses default padding (style.FramePadding)
		ImVec2 size = ImVec2(32.0f, 32.0f);                     // Size of the image we want to make visible
		ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
		ImVec2 uv1 = ImVec2(1.0f, 1.0f);						// UV 
		ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint

		ImGui::SetNextWindowSize(ImVec2(350, 80), ImGuiCond_Once);
		ImGui::Begin("Pause", &pauseflg);
		ImGui::Text(u8"�X�V�~�߂�(O�L�[):1�t���[���i�ށi1�L�[�j");
		ImGui::SameLine();

		// �f�o�b�O�|�[�Y�̏���
		// �摜���Đ��ƒ�~��u��

		std::string hoge = "";
		if (pauseflg)
		{
			hoge = startImage;
		}
		else {
			hoge = stopImage;
		}

		auto srv = ImageResourceManager::GetInstance()->GetResource(hoge)->GetSRV();
		if (ImGui::ImageButton((void*)srv, size, uv0, uv1, frame_padding, bg_col, tint_col) ||
			CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_O))
		{
			if (pauseflg == false) {
				pauseflg = true;
			}
			else if (pauseflg == true)
			{
				pauseflg = false;
			}
		}

		// 1�t���[���i�߂�
		OneFrame();

		ImGui::End();

	}
}

// 1�t���[���i�߂鏈��
void ImGuiManager::OneFrame()
{
	string ownname = "oneflame";
	const char* bbb = ownname.c_str();

	float my_tex_w = 100;
	float my_tex_h = 100;

	int frame_padding = -1;                             // -1 == uses default padding (style.FramePadding)
	ImVec2 size = ImVec2(32.0f, 32.0f);                     // Size of the image we want to make visible
	ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
	ImVec2 uv1 = ImVec2(1.0f, 1.0f);// UV coordinates for (32,32) in our texture
	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint


	if (pauseflg)
	{
		ImGui::SameLine();
		auto srv = ImageResourceManager::GetInstance()->GetResource(stopImage)->GetSRV();
		if (ImGui::ImageButton((void*)srv, size, uv0, uv1, frame_padding, bg_col, tint_col) ||
			CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_1))
		{
			if (oneFlameFlg == false) {
				oneFlameFlg = true;
			}
		}


		// ��~���ꎞ�I�ɂ�߂�
		if (oneFlameFlg)
		{
			pauseflg = false;
		}
	}
	// 1�t���[���߂��Ă����Ƃ��̏���
	else if (oneFlameFlg && !pauseflg)
	{
		oneFlameFlg = false;
		pauseflg = true;
	}

}

// �R���X�g���N�^
ImGuiManager::ImGuiManager()
{
	// �摜���[�h
	ImageResourceManager::GetInstance()->Load(startImage);
	ImageResourceManager::GetInstance()->Load(stopImage);
}


#endif


// �q�I�u�W�F�N�g���m�F���Ă��邩�H
// �e���m�F���Ă��邩�H

// ���_�f�[�^����ې����@������悤�ɂ���@�@���߂̍ŐV�łł���
// �����蔻��̃f�o�b�O�̍�����

