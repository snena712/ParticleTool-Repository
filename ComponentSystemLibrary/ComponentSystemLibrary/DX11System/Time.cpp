#include "Time.h"
#include <Windows.h>

void Time::Init()
{
	// �^�C�}�𑜓x���~���b��
	::timeBeginPeriod(1);
}

void Time::Update()
{
	// timeGetTime�֐��́A�~���b�P�ʂŃV�X�e���������擾�B 
	// �V�X�e�����Ԃ́AWindows���N�����Ă���̌o�ߎ��ԁB
	uint64_t current_time = ::timeGetTime();

	deltaTime = current_time - lastTime;	//�f���^�^�C���L�^

	lastTime = current_time;				//�X�V���s���̃^�C���L�^

	sleepTime = 16666 - deltaTime;			//�X���b�h�X���[�v�^�C�}�L�^
}

void Time::Exit()
{
	// �^�C�}�𑜓x�����ɖ߂�
	::timeEndPeriod(1);
}
