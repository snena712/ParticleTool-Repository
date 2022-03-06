//*****************************************************************************
//!	@file	Application.cpp
//!	@brief	
//!	@note	�A�v���P�[�V�����N���X
//!	@author	
//*****************************************************************************

//-----------------------------------------------------------------------------
//	Include header files.
//-----------------------------------------------------------------------------
#include <Crtdbg.h>
#include <Windows.h>
#include <chrono>
#include <thread>
#include "Application.h"
#include "../ComponentSystemLibrary/DX11System/CWindow.h"
#include "../ComponentSystemLibrary/DX11System/macro.h"
#include "game.h"
#include "DX11System/Time.h"

//-----------------------------------------------------------------------------
// �X�^�e�B�b�N�@�����o�[
//-----------------------------------------------------------------------------
const char*			Application::WINDOW_TITLE = "�A�v���P�[�V�����N���X";
const char*			Application::WINDOW_CLASS_NAME = "win32app";

const uint32_t		Application::WINDOW_STYLE_WINDOWED = (WS_VISIBLE | WS_CAPTION | WS_SYSMENU);
const uint32_t		Application::WINDOW_EX_STYLE_WINDOWED = (0);
const uint32_t		Application::WINDOW_STYLE_FULL_SCREEN = (WS_VISIBLE | WS_POPUP);
const uint32_t		Application::WINDOW_EX_STYLE_FULL_SCREEN = (0);


//--------------------��ʃT�C�Y--------------------//
const uint32_t		Application::CLIENT_WIDTH =  1080;
const uint32_t		Application::CLIENT_HEIGHT = 720;


uint32_t			Application::SYSTEM_WIDTH = 0;
uint32_t			Application::SYSTEM_HEIGHT = 0;

const float			Application::FPS = 60;


//==============================================================================
//!	@fn		CApplication
//!	@brief	�R���X�g���N�^
//!	@param	
//!	@retval	
//==============================================================================
Application :: Application(): m_SystemCounter		( 0 )
{}

//==============================================================================
//!	@fn		~Application
//!	@brief	�f�X�g���N�^
//!	@param	
//!	@retval	
//==============================================================================
Application :: ~Application()
{
	Dispose();
}

//==============================================================================
//!	@fn		GetInstance
//!	@brief	�C���X�^���X�擾
//!	@param	
//!	@retval	�C���X�^���X
//==============================================================================
Application* Application::Instance()
{
	static Application Instance;

	return &Instance;
}

//==============================================================================
//!	@fn		InitSystemWH
//!	@brief	�V�X�e���̈�̕��ƍ�����������
//!	@param	
//!	@retval	
//==============================================================================
void Application :: InitSystemWH()
{

}

//==============================================================================
//!	@fn		Init
//!	@brief	������
//!	@param	�C���X�^���X�n���h��
//!	@retval	
//==============================================================================
bool Application :: Init( HINSTANCE h_Instance )
{
	// �������[���[�N�����o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF|_CRTDBG_LEAK_CHECK_DF);


#ifdef DEBUG
	// �R���\�[�������蓖�Ă�
	AllocConsole();
	// �W���o�͂̊��蓖��
	freopen_s(&m_fp, "CON", "w", stdout);
#endif
	  
	// ���ƍ���������
	InitSystemWH();

	// �E�C���h�E�쐬
	CWindow* window = CWindow::Instance();
	window->RegisterClass( h_Instance, WINDOW_CLASS_NAME, CS_OWNDC );

	window->SetWindow( h_Instance,
					   WINDOW_STYLE_WINDOWED,
					   WINDOW_EX_STYLE_WINDOWED,
					   NULL,
					   CLIENT_WIDTH + SYSTEM_WIDTH,
					   CLIENT_HEIGHT + SYSTEM_HEIGHT,
					   WINDOW_CLASS_NAME,
					   WINDOW_TITLE,
					   false);
			
	m_hWnd = window->GetHandle();
	m_hInst = h_Instance;

	return true;
}

//==============================================================================
//!	@fn		Dispose
//!	@brief  �I������
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Dispose()
{
#ifdef DEBUG
	// �W���o�̓N���[�Y
	fclose(m_fp);
	// �R���\�[���J��
	::FreeConsole();
#endif
	

	return;
}

//==============================================================================
//!	@fn		MainLoop
//!	@brief	���C�����[�v
//!	@param	
//!	@retval	���b�Z�[�WID
//==============================================================================
unsigned long Application :: MainLoop()
{
	MSG		msg;	
	ZeroMemory( &msg, sizeof( msg ) );	

	CWindow* window = CWindow::Instance();

	// �Q�[���̏�������
	GameInit();

	//�^�C������������
	Time::GetInstance()->Init();

	while (window->ExecMessage()) {

		Time::GetInstance()->Update();

		GameInput();		// �ްѓ���	
		GameUpdate();		// �ްэX�V
		GameRender();		// �ްѕ`��

		if (Time::GetInstance()->sleepTime > 0) {
			float tt = Time::GetInstance()->sleepTime / 1000.0f;
			//printf("sleep:%f \n", tt);
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(tt)));
			//�w�肵�����Ύ��Ԃ������X���b�h���u���b�N���� (function template)
		}
	}

	//�^�C���I������
	Time::GetInstance()->Exit();

	// �Q�[���̏I������
	GameDispose();

	return window->GetMessage();
}

//==============================================================================
//!	@fn		Input
//!	@brief	�L�[����
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Input(uint64_t deltataime)
{
}

//==============================================================================
//!	@fn		Update
//!	@brief	�X�V
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Update(uint64_t deltataime)
{
	// �V�X�e���J�E���^
	m_SystemCounter ++;
}

//==============================================================================
//!	@fn		Render
//!	@brief	�`��
//!	@param	
//!	@retval	
//==============================================================================
void Application :: Render(uint64_t deltatime)
{
}

//==============================================================================
//!	@fn		GetHWnd
//!	@brief	HWND �擾
//!	@param	
//!	@retval	HWND
//==============================================================================
HWND Application :: GetHWnd()
{
	return m_hWnd;
}

//==============================================================================
//!	@fn		GetHInst
//!	@brief	HINSTANCE �擾
//!	@param	
//!	@retval	HINSTANCE
//==============================================================================
HINSTANCE Application::GetHInst()
{
	return m_hInst;
}

//******************************************************************************
//	End of file.
//******************************************************************************
