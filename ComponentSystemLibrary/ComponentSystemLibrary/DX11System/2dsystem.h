#pragma once
#include	<DirectXMath.h>

struct ConstantBufferWorld		// ���[���h�ϊ��s��
{
	DirectX::XMFLOAT4X4 World;
};

struct ConstantBufferViewPort
{
	unsigned int	ViewportWidth[4];			// �r���[�|�[�g��
	unsigned int	ViewportHeight[4];				// �r���[�|�[�g����
};

bool Init2D();
void Exit2D();