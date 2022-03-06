#pragma once
#include "ComputeDetail.h"


// ビット演算の管理
enum BitControl
{
	ON,		// オン
	OFF,	// オフ
	REVERSE	// 反転
};

// ビット単位のフラグ操作関数
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