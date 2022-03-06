#pragma once
#include	"DX11System/Memory.h"
#include    "DX11System/Shader.h"

#define TOOLMODE
// ��ԊǗ�
enum class StatusCompute
{
	Dead,
	Live
};

enum class Shape
{
	Sphere,
	Box
};

enum class ColorControl
{
	Normal,
	Picture
};

// �p�[�e�B�N���̌X�������Ă�����̍\����
ALIGN16 struct ParticleBase
{
	XMFLOAT3 pos;	// �ʒu
	float life;		// ���C�t

	XMFLOAT3 vel;	// �p�[�e�B�N���������Ă����	
	int Live;		// ������

	XMFLOAT3 scale;	// �傫��
	float depth;	// �[�x
	
	XMFLOAT3 dummy;	// �_�~�[
	float parentId;	// �eID

	XMFLOAT4 color;	// �`��p�F���

	XMFLOAT3 movePos;	// �p�[�e�B�N���̈ړ���
};

ALIGN16 struct ParticleDraw
{
	XMFLOAT3 pos;		// �ʒu
	float dummyParticleDraw1;

	XMFLOAT3 scale;		// �傫��
	float dummyParticleDraw2;

	XMFLOAT4 color;		// �F
};

// �\�[�g�p�ϐ�
ALIGN16 struct SortStruct
{
	float inc;
	float dir;
	float time;
	float seedkey;
};
// �p�[�e�B�N���̐�����
ALIGN16 struct ParticleNum
{
	float spawnNum;			// ���݂̐�
	float nowSpawnNum;		// ���t���[���̐������ꂽ��
	float maxParticleNum;	// ����������Ă��ė��p�\�Ȑ�
	float seed;				// 16�o�C�g�p�̃_�~�[
	float loopcnt;			// for�̃��[�v��
	float dummynum;			// �_�~�[
	float dummynum2;		// �_�~�[
	float dummynum3;		// �_�~�[
};

// �Ǘ��ԍ�(�萔)
ALIGN16 struct ParticleHeader
{
	float tableNumber;		// �z��ԍ��@�l�Ȃ��Ƃ���-1
};

// �Ǘ��w�b�_�[�i�萔�j
ALIGN16 struct EmitterHeader
{
	//float ;

};

// �傫���̍\����
ALIGN16 struct ScaleAnim
{
	XMFLOAT3 initScale;		// �����̃T�C�Y
	float dummyscale;		// 16�o�C�g�p�̃_�~�[
	XMFLOAT3 finishScale;	// �ŏI�̑傫��
	float dummyscale2;
};

// �F�̍\����
ALIGN16 struct Color
{
	XMFLOAT4 initColor;
	XMFLOAT4 endColor;
};

// �v�Z�̃v���X�l
ALIGN16 struct PlusData
{
	XMFLOAT4X4 Mtx;	// �J�����̍s��

	XMFLOAT3 rand;
	float life;				// �������̃��C�t

	XMFLOAT3 vel;			// �ړ���
	int nowSpawnMaxNum;		// ���t���[���̐����ő吔

	XMFLOAT3 spawnPos;		// �����|�W�V����
	float speed;			// �����͈͂̕�


	XMFLOAT3 spawnPosRange;	// �����͈͂̕�
	int parentId;			// �������̐eID

	ScaleAnim scaleAnim;	// �A�j���[�V����

	Color color;			// �F�̍\����

	XMFLOAT3 velRange;		// �͈͂̔�
	float gravity;			// �d��

	XMFLOAT3 forward;		// �O������
	float pictureWidthSize;	// �F�摜�̉�

	int velocityFlg;		// �ړ��Ɋւ���t���O
	int colorFlg;			// �F�Ɋւ���t���O
	int Live;				// ������		
	int dummyFlg;			//�@�_�~�[
};

// �`�敨�̐؊���
enum class DrawStatus
{
	Second,
	Third
};

//-----------------------------
// create by Suehara
//-----------------------------