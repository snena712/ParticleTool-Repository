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
#include "DX11System/DX11util.h"
#include "DX11System/XAudio2.h"
#include "DX11System/input.h"
#include "DX11System/ImageResourceManager.h"
#include "DX11System/Controller.h"
#include "DX11System/Application.h"
#include "game.h"

// シーン一覧
#include "test.h"
#include "Scene/TitleScene.h"

// デバッグ用ImGui
#include "DX11System/ImageResourceManager.h"
#include "imGui/imgui_impl_dx11.h"
#include "imGui/imgui_impl_win32.h"
#include "Component\ImGuiComponent.h"

