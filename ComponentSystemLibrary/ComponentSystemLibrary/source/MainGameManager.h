#pragma once
class MainGameManager
{
public:
	MainGameManager(const MainGameManager&) = delete;
	MainGameManager& operator=(const MainGameManager&) = delete;
	MainGameManager(MainGameManager&&) = delete;
	MainGameManager& operator=(MainGameManager&&) = delete;

	static MainGameManager* GetInstance() {
		static MainGameManager instance;
		return &instance;
	}

	enum class SceneState
	{
		AWAKE,
		START,
		NORMAL,
		OPTION,
		END,
		CLEAR,
		TITLE,
		SELECT
	};

	SceneState m_state= SceneState::AWAKE;

	SceneState GetSceneState() { 
		return m_state; }
	void SetSceneState(SceneState _state) { m_state = _state; }


private:
	MainGameManager() {};
	~MainGameManager() {};

};


