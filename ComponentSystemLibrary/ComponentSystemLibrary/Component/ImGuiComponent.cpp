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

// オーナーのデバッグフラグ確認
bool ImGuiManager::CheckPlayMode()
{
	return ((ImGuiManager::GetFlg().debug) ? true : false);
}

// ImGuiの更新処理
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

	//imGuiの更新処理
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//-----------------変更点------------------//
	// ポーズ
	Pause();

	// デバッグ実行
	if (flgs.debug) {

		// 初回実行時の場所指定
		if (!flgs.firstdebugflg) {
			//ImGui::SetNextWindowPos(ImVec2(0, 0));
			flgs.firstdebugflg = true;
		}
		ImGui::SetNextWindowSize(ImVec2(200, 600), ImGuiCond_Once);
		ImGui::Begin(u8"デバッグ中", &flgs.debug);

		// フレームレートを表示
		ImGui::Text(u8"現在のFPS : %.1f FPS", ImGui::GetIO().Framerate);

		// シーン名の表示
		ImGui::Text(u8"現在のシーン名 : %s", SceneManager::Instance()->GetActiveScene()->GetSceneName().c_str());

		// オブジェクトの数を表示
		ImGui::Text(u8"現在のゲームオブジェクト数 : %d", objcnt);
		objcnt = 0;


		// 現状リストにあるもの全てを計算する
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
					ImGui::Text(u8"なし");
				}

				// 現状100まで →transformに子供の大きさ取得作る
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

		// フラグがない場合終了
		if (!flgs.debug)
		{
			flgs.dend = true;
		}

		// デバッグのフラグがオンの物を探し、表示する
		for (auto& _objList : *SceneManager::Instance()->GetEntityObject())
		{
			objcnt++;
			if (_objList.obj->GetDebug())
			{
				string ownname = _objList.obj->GetName();
				const char* bbb = ownname.c_str();

				ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_Once);
				ImGui::Begin("Inspector", _objList.obj->GetDebugAdress());
				ImGui::Text(u8"*名前:%s", bbb);

				if (ImGuiManager::Instance()->CheckPlayMode())
				{
					_objList.obj->transform->ImGuiDebug();

					for (auto& com : _objList.obj->GetComponentList())
					{
						com->ImGuiDebug();	//全コンポーネントのデバッグ関数実行
					}
				}

				ImGui::End();
			}
		}
	}
}

// ImGuiの描画更新処理
void ImGuiManager::ImGuiRender()
{
	//imGuiの更新処理
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

// ImGuiの終了処理
void ImGuiManager::ImGuiEnd()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

//-----------------変更点------------------//
// ここから下
// ポーズの処理
void ImGuiManager::Pause()
{
	//ImGui::SetNextWindowPos(ImVec2(Application::Instance()->CLIENT_WIDTH / 2 - 300 / 2, 0));   //画面位置を外部から取得できるようにする

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
		ImGui::Text(u8"更新止める(Oキー):1フレーム進む（1キー）");
		ImGui::SameLine();

		// デバッグポーズの処理
		// 画像を再生と停止を置く

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

		// 1フレーム進める
		OneFrame();

		ImGui::End();

	}
}

// 1フレーム進める処理
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


		// 停止を一時的にやめる
		if (oneFlameFlg)
		{
			pauseflg = false;
		}
	}
	// 1フレーム戻ってきたときの処理
	else if (oneFlameFlg && !pauseflg)
	{
		oneFlameFlg = false;
		pauseflg = true;
	}

}

// コンストラクタ
ImGuiManager::ImGuiManager()
{
	// 画像ロード
	ImageResourceManager::GetInstance()->Load(startImage);
	ImageResourceManager::GetInstance()->Load(stopImage);
}


#endif


// 子オブジェクトを確認しているか？
// 親を確認しているか？

// 頂点データから丸生成　見えるようにする　　かめの最新版でする
// 当たり判定のデバッグの高速化

