#include "ScoreUI.h"

using namespace Component;

ScoreUI::ScoreUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("ScoreUI");
}

ScoreUI::~ScoreUI()
{

}

void ScoreUI::Start()
{
	enemana=SceneManager::Instance()->GameObjectFindName("EnemyManager")
		->GetComponent<Component::EnemyManager>();
}

void ScoreUI::Update()
{

}

void ScoreUI::DrawObject()
{
	ScoreUpdate();
	PosUpdate();
}

void ScoreUI::PosUpdate()
{
	std::string _nowstr(std::to_string(m_score));
	std::string _plusstr(std::to_string(m_plusscore));

	std::string _scorestr;

	if (m_plusscore > 0) {
		_scorestr = "SCORE:" + _nowstr + "+" + _plusstr;
	}
	else {
		_scorestr = "SCORE:" + _nowstr;
	}

	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 535,
		Application::Instance()->CLIENT_HEIGHT / 2 - 320,
		20, 20, XMFLOAT4(0.0f, 1.0f, 0.0f, 0.7f), _scorestr.c_str());
}

// �X�R�A�̍X�V
void ScoreUI::ScoreUpdate()
{
	// �X�R�A��̎��S���ƃ}�l�[�W���[�̎��S���̔�r
	if (m_deadcnt != enemana->GetDeadEnemyCnt())
	{
		m_deadcnt++;
		m_plusscore += m_scorepoint;
	}

	// �v���X�̃X�R�A������Ƃ�
	if (m_plusscore > 0) 
	{
		m_score+=10;
		m_plusscore-=10;
	}
}

// ����@�X�R�A�@�v���X�@�̏�
// �������ȉ��o�ɂ���@�v���X�̒l�����X�Ƀ}�C�i�X�ɂȂ�@�X�R�A�����X�ɑ�����
// enemy����n�����������H