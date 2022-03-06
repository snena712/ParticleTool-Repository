#pragma once
#include "ComputeDetail.h"


// �r�b�g���Z�̊Ǘ�
enum BitControl
{
	ON,		// �I��
	OFF,	// �I�t
	REVERSE	// ���]
};

// �r�b�g�P�ʂ̃t���O����֐�
static void BitFlgChanger(int &_flg, int _changeBit, BitControl _bit)
{
	switch (_bit)
	{
	case ON:
		_flg |= _changeBit;
		break;
	case OFF:
		_flg &= ~_changeBit;
		break;
	case REVERSE:
		_flg ^= _changeBit;
		break;
	default:
		break;
	}
}

static bool GetBitFlg(int &_flg, int _checkBit)
{
	if (_flg& _checkBit)
		return true;
	
	return false;
}

//-----------------------------
// create by Suehara
//-----------------------------