#include "Time.h"
#include <Windows.h>

void Time::Init()
{
	// タイマ解像度をミリ秒に
	::timeBeginPeriod(1);
}

void Time::Update()
{
	// timeGetTime関数は、ミリ秒単位でシステム時刻を取得。 
	// システム時間は、Windowsを起動してからの経過時間。
	uint64_t current_time = ::timeGetTime();

	deltaTime = current_time - lastTime;	//デルタタイム記録

	lastTime = current_time;				//更新実行時のタイム記録

	sleepTime = 16666 - deltaTime;			//スレッドスリープタイマ記録
}

void Time::Exit()
{
	// タイマ解像度を元に戻す
	::timeEndPeriod(1);
}
