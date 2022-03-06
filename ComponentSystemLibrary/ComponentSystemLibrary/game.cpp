//*****************************************************************************
//!	@file	game.cpp
//!	@brief	
//!	@note	�Q�[������
//!	@author	
//*****************************************************************************

#include "systemInclude.h"
#include "source\Compute\TestComputeObj.h"
//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
FILE* m_fp;							// �f�o�b�O�p�R���\�[��

bool once;
//==============================================================================
//!	@fn		GameInit
//!	@brief	�Q�[����������
//!	@param	�C���X�^���X�l
//!	@param	�E�C���h�E�n���h���l
//!	@param	�E�C���h�E�T�C�Y���i�␳�ς݂̒l�j
//!	@param	�E�C���h�E�T�C�Y�����i�␳�ς݂̒l�j
//!	@param�@�t���X�N���[���t���O�@true ; �t���X�N���[���@false : �E�C���h�E
//!	@retval	true �����@false ���s
//==============================================================================
bool GameInit()
{
	// DX ������
	DX11Init(
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT,
		false);

	// �A���t�@�u�����f�B���O��L���ɂ���@
	TurnOnAlphablend();

	// XAudio������
	InitSound();

	bool sts = false;

	// CDirectInput������
	sts = CDirectInput::GetInstance().Init(
		Application::Instance()->GetHInst(),
		Application::Instance()->GetHWnd(),
		Application::CLIENT_WIDTH,
		Application::CLIENT_HEIGHT);
	if (!sts)
	{
		MessageBox(Application::Instance()->GetHWnd(), "CDirectInput", "error", MB_OK);
	}

	// �R���g���[���[�̏�����
	Controller::Instance()->Init();

	// ���s����������
	DX11LightInit(DirectX::XMFLOAT4(1, 1, -1, 0));		// ���s�����̕������Z�b�g

	// �X�J�C�h�[���̏�����
	Skydome::GetInstance()->Init(
		"assets/Skydome_RR6/Dome_RR601.x",
		"shader/vs.hlsl",
		"shader/pstexcol.hlsl",
		"assets/Skydome_RR6/");

	Skydome::GetInstance()->SetScale(50);

	// �R���s���[�g�}�l�[�W���[
	ComputeManager::Instance()->Init();

	PostEffect::PostEffectManager::Instance()->Init();

	// �V�[���̏�����
	Scene* t = new Map1_1Class("Map1_1");

	if (!SceneManager::Instance()->ActiveScene("Map1_1")) {
		exit(EXIT_FAILURE);
	}

	// 2D�V�X�e��������
	Init2D();

	MyString::InitMyString();

	return	true;
}

//==============================================================================
//!	@fn		GameInput
//!	@brief	�Q�[�����͌��m����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameInput(){

	CDirectInput::GetInstance().GetKeyBuffer();
	CDirectInput::GetInstance().GetMouseState();

	// �R���g���[���[
	Controller::Instance()->CheckConnect();
	Controller::Instance()->Update();

}

//==============================================================================
//!	@fn		GameUpdate
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameUpdate() {

#ifdef DEBUG
	ImGuiManager::Instance()->ImGuiUpdate();		// �Q�[���I�u�W�F�N�g�ɂ���H
#endif

	SceneManager::Instance()->ManagerUpdate();

	// �R���s���[�g�}�l�[�W���[
	ComputeManager::Instance()->Update();

}

//==============================================================================
//!	@fn		GameRender
//!	@brief	�Q�[���X�V����
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameRender() {

	// �^�[�Q�b�g�o�b�t�@�N���A	
	float ClearColor[4] = { 0,0,0, 1.0f }; //red,green,blue,alpha�i�q�f�a�̊e�l��0.0�`1.0�Ŏw��j
	DX11BeforeRender(ClearColor);

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* dstv;
	dstv = CDirectXGraphics::GetInstance()->GetDepthStencilView();

	if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::SELECT
		&& MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::TITLE)
	{
		//�X�J�C�h�[���̕`��

		// �����t���[���҂�����
		if (once)
			Skydome::GetInstance()->Draw();
		else
			once = true;
	}

	//Scene�̕`��
	SceneManager::Instance()->ManagerDraw();

#ifdef DEBUG
	// RTV�̐ݒ�
	auto _deviceContex = CDirectXGraphics::GetInstance()->GetImmediateContext();
	auto d = CDirectXGraphics::GetInstance()->GetDepthStencilView();
	auto m_RTVCombine = CDirectXGraphics::GetInstance()->GetRenderTargetView();
	_deviceContex->OMSetRenderTargets(1, &m_RTVCombine, d);

	// ImGui�̕`��
	ImGuiManager::Instance()->ImGuiRender();
#endif

	PostEffect::PostEffectManager().Instance()->AfterRenderClear();

	// �`��㏈��
	DX11AfterRender();
}


//==============================================================================
//!	@fn		GameDispose
//!	@brief	�Q�[���I������
//!	@param�@	
//!	@retval	�Ȃ�
//==============================================================================
void GameDispose() {

	Skydome::GetInstance()->Exit();

	ImageResourceManager::GetInstance()->Exit();
	ModelResourceManager::GetInstance()->Exit();

	Controller::Instance()->Exit();

	SceneManager::Instance()->UninitScene();

#ifdef DEBUG
	ImGuiManager::Instance()->ImGuiEnd();
#endif

	UninitSound();

	CDirectInput::GetInstance().Exit();

	// DX�I������
	DX11Uninit();
}

//******************************************************************************
//	End of file.
//******************************************************************************
