#include "RandomClass.h"
#include <random>

std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l

//=========================//
//	int�̃����_���l
//=========================//
int RandomClass::GetRandom(int Min, int Max) {

	std::uniform_int_distribution<> rand(Min, Max);        // [Min, Max] �͈͂̈�l����
	return rand(mt);
}

//=========================//
//	float�̃����_���l
//=========================//
float RandomClass::GetRandom(float Min, float Max) {

	std::uniform_real_distribution<> rand(Min, Max);        // [Min, Max] �͈͂̈�l����
	return static_cast<float>(rand(mt));
}

//=========================//
//	double�̃����_���l
//=========================//
double RandomClass::GetRandom(double Min, double Max) {

	std::uniform_real_distribution<> rand(Min, Max);        // [Min, Max] �͈͂̈�l����
	return rand(mt);
}
