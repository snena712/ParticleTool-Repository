#pragma once

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include <wrl/client.h>


#include "Component/GameObject.h"
#include "Component/SceneManager.h"
#include "DX11System/CDirectxGraphics.h"
#include "DX11System/DX11Settransform.h"
#include "DX11System/CLight.h"
#include "DX11System/ImageResourceManager.h"
#include "DX11System/ModelResourceManager.h"
#include "DX11System/Skydome.h"
#include "DX11System/DX11util.h"
#include "DX11System/XAudio2.h"
#include "DX11System/CDirectInput.h"
#include "DX11System/Controller.h"
#include "DX11System/mystring.h"
#include "DX11System/2dsystem.h"

#include "Application.h"
#include "game.h"

// 追加
#include "source/PostEffect/DirectGraphicsTexture.h"
#include "source/MainGameManager.h"
#include "source/Compute/ComputeManager.h"
#include "source/PostEffect/PostEffectManager.h"


// シーン一覧
#include "Scenes/TitleScene.h"
#include "source\Map1_1Class.h"
#include "source/Scene/SelectScene.h"

// デバッグ用ImGui
#include "DX11System/ImageResourceManager.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"
#include "Component\ImGuiComponent.h"

