#pragma once

#define PlusBufNum  (3)
#define Thread   (256)

// 形
#define Velocity_Event_SHAPE_BOX   (0x01)	// オン：ボックス　オフ：球体
#define Velocity_Event_Sin         (0x02)   // 波
#define Velocity_Event_ColorMove   (0x04)	// 色移動
#define Velocity_Simulation_Space  (0x08)   // ローカルかワールドで動かすか　（Trueがワールド、falseがローカル）

#define Color_Event_PictureColor	(0x01)	// 画像による色変化
#define Color_0   (0x02)	//
#define Color_1   (0x04)	// 
#define Color_2   (0x08)    // 