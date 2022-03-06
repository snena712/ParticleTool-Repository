//*****************************************************************************
//!	@file	main.cpp
//!	@brief	
//!	@note	DX11 �ЂȌ`
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <windows.h>

#include "imGui/imgui_impl_win32.h"
#include "imGui/imgui_impl_dx11.h"

#include "Application.h"

//-----------------------------------------------------------------------------
// �}�N���̒�`
//-----------------------------------------------------------------------------
#define   imGui "imGui"
#define   imGui_Name "imGui_test"

//-----------------------------------------------------------------------------
// �����N���C�u�����̐ݒ�
//-----------------------------------------------------------------------------
#pragma comment (lib, "winmm.lib")				// �����N�Ώۃ��C�u������winmm.lib��ǉ�
#pragma comment (lib, "d3d11.lib")				// �����N�Ώۃ��C�u������d3d11.lib��ǉ�
#pragma comment (lib, "dxgi.lib")				// �����N�Ώۃ��C�u������dxgi.lib��ǉ�
#pragma comment (lib, "D3DCompiler.lib")		// �����N�Ώۃ��C�u������d3dcompiler.lib��ǉ�
#pragma comment (lib, "DirectXTex.lib")			// �����N�Ώۃ��C�u������DirectXTex.lib��ǉ�
#pragma comment (lib, "DirectXTex.lib")			// �����N�Ώۃ��C�u������DirectXTex.lib��ǉ�
#pragma comment (lib, "directxtex.lib")			// �����N�Ώۃ��C�u������
#pragma comment (lib, "dxguid.lib")				// �����N�Ώۃ��C�u������
#pragma comment (lib, "dinput8.lib")			// �����N�Ώۃ��C�u������
#pragma comment (lib, "assimp-vc141-mtd.lib")	// �����N�Ώۃ��C�u������

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------


//==============================================================================
//!	@fn		WinMain
//!	@brief	�G���g���|�C���g
//!	@param	�C���X�^���X�n���h��
//!	@param	�Ӗ��Ȃ�
//!	@param	�N�����̈���������
//!	@param	�E�C���h�E�\�����[�h
//!	@retval	TRUE�@�����I��/FALSE�@���s�I��
//!	@note	
//==============================================================================
INT APIENTRY WinMain(HINSTANCE  h_hInst,
	HINSTANCE  h_hPrevInst,
	LPSTR		h_lpszArgs,
	int		h_nWinMode)
{
	// �A�v���P�[�V������������
	Application* App = Application::Instance();		// �C���X�^���X�擾
	App->Init(h_hInst);

	// �E�C���h�E��\������
	ShowWindow(App->GetHWnd(), h_nWinMode);
	UpdateWindow(App->GetHWnd());

	// ���C�����[�v
	long ret = App->MainLoop();

	// �A�v���P�[�V�����I������
	App->Dispose();

	return ret;
}


//******************************************************************************
//	End of file.
//******************************************************************************
