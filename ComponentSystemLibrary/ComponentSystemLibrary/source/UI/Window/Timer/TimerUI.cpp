#include "TimerUI.h"

using namespace Component;

TimerUI::TimerUI(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("TimerUI");

	// 爆破アニメーション

}

TimerUI::~TimerUI()
{

}

void TimerUI::Start()
{
	StartTimer();
}

void TimerUI::Update()
{
	ReduceTime();
	//PosUpdate();

	LimitCheck();
}

void TimerUI::DrawObject()
{

}

void TimerUI::StartTimer()
{
	m_starttime = Time::GetInstance()->GetTimeMilli();
}

void TimerUI::PosUpdate()
{
	// 経過時間　ミリ秒
	float _time = (Time::GetInstance()->GetTimeMilli() - m_starttime)/1000;
	
	// 分取得
	int _minutes = (int)_time / 60;
	if (_minutes > 99) {
		// 上限値
		_minutes = 99;
	}
	std::string _minutesstr(std::to_string(_minutes));
	if (_minutes < 10) {
		// 10以下の時0表示
		_minutesstr.insert(0, "0");
	}

	// 秒取得
	int _seconds = (int)_time % 60;
	std::string _secondsstr(std::to_string(_seconds));
	if (_seconds < 10) {
		// 10以下の時0表示
		_secondsstr.insert(0, "0");
	}

	// 小数点取得
	float num;
	int b = (int)_time;
	
	// int型のキャスト変換で整数化
	float _decimal = _time - b;
	std::string _decimalstr(std::to_string(_decimal).erase(4));


	std::string _timestr = "TIME:"+_minutesstr + ":" + _secondsstr+":"+ _decimalstr.erase(0,1);

	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 + 190,
		Application::Instance()->CLIENT_HEIGHT / 2 - 355,
		25, 25, XMFLOAT4(0, 1.0f, 0.2f, 0.5f), _timestr.c_str());
}

void TimerUI::ReduceTime()
{

	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::NORMAL)
	{
		// 経過時間　ミリ秒
		if (MainGameManager::GetInstance()->GetSceneState() != MainGameManager::SceneState::OPTION)
		{
			_time = (Time::GetInstance()->GetTimeMilli() - m_starttime) / 1000;
			if (m_optionstock != NULL)
			{
				m_limitTime += (Time::GetInstance()->GetTimeMilli() - m_optionstock) / 1000;
				m_optionstock = NULL;
			}
			_time = m_limitTime - _time;
			m_stocktime = _time;
		}
	}
	if (MainGameManager::GetInstance()->GetSceneState() == MainGameManager::SceneState::OPTION)
	{
		if (m_optionstock==NULL)
		{
			m_optionstock = Time::GetInstance()->GetTimeMilli();
		}
	}

	// 分取得
	int _minutes = (int)_time / 60;
	if (_minutes < 0) {
		// 上限値
		_minutes = 0;
	}
	std::string _minutesstr(std::to_string(_minutes));
	if (_minutes < 10) {
		// 10以下の時0表示
		_minutesstr.insert(0, "0");
	}

	// 秒取得
	int _seconds = (int)_time % 60;
	if (_seconds < 0) {
		// 10以下の時0表示
		_seconds = 0;
	}
	std::string _secondsstr(std::to_string(_seconds));
	if (_seconds < 10) {
		// 10以下の時0表示
		_secondsstr.insert(0, "0");
	}

	// 小数点取得
	int b = (int)_time;

	// int型のキャスト変換で整数化
	float _decimal = _time - b;

	if (_decimal < 0)
	{
		_decimal = 0;
	}


	std::string _decimalstr(std::to_string(_decimal).erase(4));

	std::string _timestr = "TIME :" + _minutesstr + ":" + _secondsstr + ":" + _decimalstr.erase(0, 2);

	MyString::DrawString(Application::Instance()->CLIENT_WIDTH / 2 - 535,
		Application::Instance()->CLIENT_HEIGHT / 2 - 360,
		20, 20, XMFLOAT4(0, 1.0f, 0.0f, 0.7f), _timestr.c_str());
}

void TimerUI::LimitCheck()
{
	if (m_stocktime < 0&& !m_endflg)
	{
		m_endflg = true;
		MainGameManager::GetInstance()->SetSceneState(MainGameManager::SceneState::END);
	}
}