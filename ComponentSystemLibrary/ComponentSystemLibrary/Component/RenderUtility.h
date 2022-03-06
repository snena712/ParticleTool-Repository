#pragma once

//�����_�����O�^�C�v�i���i�K�����_�����O�̎d��Ɏg���j
enum class RenderingType
{
	Opaque,			//�s�����I�u�W�F�N�g
	Transparent,	//�������I�u�W�F�N�g
	UI2D			//2D�i�X�N���[���\���j��UI
};

//�A���J�[�|�C���g�i����ɉ��������΍��W�ŕ`�悷��j
enum class AnchorType
{
	LeftTop,		//����
	LeftMiddle,		//������
	LeftBottom,		//����
	Top,			//�㒆��
	Center,			//�^��
	Bottom,			//������
	RightTop,		//�E��
	RightMiddle,	//�E����
	RightBottom		//�E��
};

enum class UV_STATE {
	DEFAULT,	//�f�t�H���g
	TURN_X,		//X���Ŕ��]
	TURN_Y,		//Y���Ŕ��]
	TURN_XY		//XY���Ŕ��]
};
