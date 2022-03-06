#pragma once
#include	<DirectXMath.h>

namespace MyString {

#define		MAXMOJICNT		1024

// ���_�o�b�t�@�X�V
static void UpdateVertexBuffer();

// �C���f�b�N�X�o�b�t�@�X�V
static void UpdateIndexBuffer();

// �߂�l�@�C���f�b�N�X��
int CreateVertexIndexData(int length);

// ������
bool InitMyString();

// ������`��
void DrawMyString();

// �`�悷�镶������N���A
void ClearMyString();

// �A�X�L�[�t�H���g�`��p�̂t�u���W�擾
void GetAsciiFontUV(char code, DirectX::XMFLOAT2* uv);

//�@������`��
void DrawString(int x, int y, int width, int height, DirectX::XMFLOAT4 color, const char* str);

}

