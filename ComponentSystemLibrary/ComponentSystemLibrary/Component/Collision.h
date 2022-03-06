#pragma once

#include<math.h>
#include"../DX11System/Vector.h"



/////////////////////////////////////////////////
//	プロトタイプ宣言
/////////////////////////////////////////////////

bool HitCheckBox_Box(Vector3 distans, Vector3 Box1Size, Vector3 Box2Size);

bool HitCheckBox_Sphere(Vector3 BoxPos, Vector3 BoxSize, Vector3 SpherePos, float SphereRadius);
bool HitCheckSphere_Sphere(Vector3 distans, float Sphere1Radius, float Sphere2Radius);


bool HitCheckBox_Circle(Vector3 BoxPos, Vector3 BoxSize, Vector3 CirclePos, float CircleRadius);
bool HitCheckCircle_Circle(Vector3 distans, float Circle1Radius, float Circle2Radius);

template<typename T>
bool NumBetweenAtoB(T num, T A, T B);

bool HitCheckPoint_Sphere(Vector3 SpehrePos, Vector3 PointPos, float SphereRadius);
bool HitCheckPoint_Circle(Vector3 CirclePos, Vector3 PointPos, float CircleRadius);

bool HitCheckLine_Line(Vector3 A_start, Vector3 A_end, Vector3 B_start, Vector3 B_end);