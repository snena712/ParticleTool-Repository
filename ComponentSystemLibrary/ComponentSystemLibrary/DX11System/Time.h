#pragma once
#include <chrono>

class Application;

class Time
{
public:
	Time(const Time&) = delete;
	Time& operator=(const Time&) = delete;
	Time(Time&&) = delete;
	Time& operator=(Time&&) = delete;

	friend Application;

	//インスタンスの取得
	static Time* GetInstance() {
		static Time instance;
		return &instance;
	}

	//タイムの取得
	uint64_t GetTimeMilli() { return lastTime; }		//ミリ秒でtimeを返す
	float GetTime()
	{
		double t = static_cast<double>(lastTime);
		return (static_cast<float>(t) / 1000.0f);		//秒でtimeを返す
	}

	//デルタタイムの取得
	uint64_t GetDeltaTimeMilli() { return deltaTime; }	//ミリ秒でdtを返す
	float GetDeltaTime()
	{
		double t = static_cast<double>(deltaTime);
		return (static_cast<float>(t) / 1000.0f);		//秒でdtを返す
	}

private:
	Time(){}

	uint64_t deltaTime;	//デルタタイム
	uint64_t lastTime;	//タイム
	int64_t sleepTime;	//Application用のスリープタイマ

	void Init();
	void Update();
	void Exit();

};

