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

	//�C���X�^���X�̎擾
	static Time* GetInstance() {
		static Time instance;
		return &instance;
	}

	//�^�C���̎擾
	uint64_t GetTimeMilli() { return lastTime; }		//�~���b��time��Ԃ�
	float GetTime()
	{
		double t = static_cast<double>(lastTime);
		return (static_cast<float>(t) / 1000.0f);		//�b��time��Ԃ�
	}

	//�f���^�^�C���̎擾
	uint64_t GetDeltaTimeMilli() { return deltaTime; }	//�~���b��dt��Ԃ�
	float GetDeltaTime()
	{
		double t = static_cast<double>(deltaTime);
		return (static_cast<float>(t) / 1000.0f);		//�b��dt��Ԃ�
	}

private:
	Time(){}

	uint64_t deltaTime;	//�f���^�^�C��
	uint64_t lastTime;	//�^�C��
	int64_t sleepTime;	//Application�p�̃X���[�v�^�C�}

	void Init();
	void Update();
	void Exit();

};

