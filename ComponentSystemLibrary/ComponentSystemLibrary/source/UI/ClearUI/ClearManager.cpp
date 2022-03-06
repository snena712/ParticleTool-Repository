#include "ClearManager.h"
#include "../../../DX11System/Controller.h"

using namespace Component;

ClearManager::ClearManager(GameObject* _owner) :ComponentBase(_owner)
{
	owner->SetName("ClearManager");

}

ClearManager::~ClearManager()
{

}

void ClearManager::Start()
{
	ControllerNum = Controller::Instance()->GetControllerNum();
	m_fade = SceneManager::Instance()->GameObjectFindName("FadeManager")->GetComponent<Component::FadeManager>();
}

void ClearManager::Update()
{
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_Q))
	{
		MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::END);
	}

	EndUpdate();

	if (ControllerNum >= 0)
	{
		InputController();
	}
	else {
		InputKeybord();
	}
}


void ClearManager::InputKeybord()
{
	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_K))
	{
		if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::OPTION)
		{
			MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::NORMAL);
		}
		else {
			MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::OPTION);
		}
	}

	// オンの時
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::OPTION)
	{
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP)) {

			m_optionNum++;
		}
		if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN)) {

			m_optionNum--;
		}

		if (m_optionNum > m_maxoptionNum)
		{
			m_optionNum = 0;
		}
		if (m_optionNum < 0)
		{
			m_optionNum = m_maxoptionNum;
		}
		printf("%d:　オプション\n", m_optionNum);
	}
}

void ClearManager::InputController()
{
	WORD Buttons = Controller::GAME_PAD[ControllerNum].state.Gamepad.wButtons;
	float y = Controller::GAME_PAD[ControllerNum].state.Gamepad.sThumbLY / 360.0f;

	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::CLEAR)
	{
		// オンの時

		if (y > 5) {
			if (!m_inputflg) {
				m_inputflg = true;
				m_optionNum++;
			}
			else {
				m_nowinputcnt++;
			}
		}
		else if (y < -5)
		{
			if (!m_inputflg) {
				m_inputflg = true;
				m_optionNum--;
			}
			else {
				m_nowinputcnt++;
			}
		}
		else {
			// 
			m_inputflg = false;
			m_nowinputcnt = 0;
		}


		if (m_optionNum > m_maxoptionNum)
		{
			m_optionNum = 0;
		}
		if (m_optionNum < 0)
		{
			m_optionNum = m_maxoptionNum;
		}

		if (m_nowinputcnt > m_maxinputcnt)
		{
			m_nowinputcnt = 0;
			m_inputflg = false;
		}

		// 決定
		if (Buttons & XINPUT_GAMEPAD_A) {
			if (!m_plessInputflg)
			{
				m_plessInputflg = true;
				m_yesflg = true;
			}

		}
		else {
			m_plessInputflg = false;
		}
		printf("%d\n", m_optionNum);
	}
}

// 処理の更新
void ClearManager::EndUpdate()
{
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::CLEAR)
	{
		if (m_maxinputcnt > m_nowinputcnt)
		{
			if (m_backui == nullptr) {
				m_backui = new GameObject();
				m_backui->AddComponent<Component::SelectBackUI>();
				m_backui->SetExistState(false);
				SceneManager::Instance()->AddObject(m_backui, false);
				m_backui->GetComponent<Component::FadeComponent>()->SetFade(10);
			}
			m_nowinputcnt++;
			MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2-200,
				Application::Instance()->CLIENT_HEIGHT / 2,
				40, 40, XMFLOAT4(0, 1.0f, 0.0f, 0.7f), "GAME CLEAR");
		}
		if (m_maxinputcnt== m_nowinputcnt){
			m_nowinputcnt++;
			m_fade->SetFadeScene(60, "SelectScene");
		}
	}
}
