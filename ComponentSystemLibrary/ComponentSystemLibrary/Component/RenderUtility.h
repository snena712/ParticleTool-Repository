#pragma once

//レンダリングタイプ（多段階レンダリングの仕訳に使う）
enum class RenderingType
{
	Opaque,			//不透明オブジェクト
	Transparent,	//半透明オブジェクト
	UI2D			//2D（スクリーン表示）のUI
};

//アンカーポイント（これに応じた相対座標で描画する）
enum class AnchorType
{
	LeftTop,		//左上
	LeftMiddle,		//左中央
	LeftBottom,		//左下
	Top,			//上中央
	Center,			//真ん中
	Bottom,			//下中央
	RightTop,		//右上
	RightMiddle,	//右中央
	RightBottom		//右下
};

enum class UV_STATE {
	DEFAULT,	//デフォルト
	TURN_X,		//X軸で反転
	TURN_Y,		//Y軸で反転
	TURN_XY		//XY軸で反転
};
