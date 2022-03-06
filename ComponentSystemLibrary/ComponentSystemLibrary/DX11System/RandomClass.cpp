#include "RandomClass.h"
#include <random>

std::random_device rnd;     // 非決定的な乱数生成器を生成
std::mt19937 mt(rnd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値

//=========================//
//	intのランダム値
//=========================//
int RandomClass::GetRandom(int Min, int Max) {

	std::uniform_int_distribution<> rand(Min, Max);        // [Min, Max] 範囲の一様乱数
	return rand(mt);
}

//=========================//
//	floatのランダム値
//=========================//
float RandomClass::GetRandom(float Min, float Max) {

	std::uniform_real_distribution<> rand(Min, Max);        // [Min, Max] 範囲の一様乱数
	return static_cast<float>(rand(mt));
}

//=========================//
//	doubleのランダム値
//=========================//
double RandomClass::GetRandom(double Min, double Max) {

	std::uniform_real_distribution<> rand(Min, Max);        // [Min, Max] 範囲の一様乱数
	return rand(mt);
}
