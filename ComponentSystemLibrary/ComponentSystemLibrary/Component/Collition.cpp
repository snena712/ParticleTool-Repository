#include"Collision.h"

#include<iostream>


// 箱 と 箱 の当たり判定
bool HitCheckBox_Box(Vector3 distans, Vector3 Box1Size, Vector3 Box2Size) {

	if ((fabsf(Box1Size.x / 2.0f + Box2Size.x / 2.0f) >= fabsf(distans.x)) &&
		(fabsf(Box1Size.y / 2.0f + Box2Size.y / 2.0f) >= fabsf(distans.y)) &&
		(fabsf(Box1Size.z / 2.0f + Box2Size.z / 2.0f) >= fabsf(distans.z))) {
		return true;
	}

	return false;

}

// 球 と 球 の当たり判定
bool HitCheckSphere_Sphere(Vector3 distans, float Sphere1Radius, float Sphere2Radius) {

	if (distans.x * distans.x + distans.y * distans.y + distans.z * distans.z <
		pow(Sphere1Radius + Sphere2Radius,2.0f)) {
		return true;
	}

	return false;
}

// 箱 と 球 の当たり判定
bool HitCheckBox_Sphere(Vector3 BoxPos, Vector3 BoxSize, Vector3 SpherePos, float SphereRadius) {

	if (NumBetweenAtoB(SpherePos.x, BoxPos.x - BoxSize.x / 2.0f, BoxPos.x + BoxSize.x / 2.0f)) {		//球の X 座標が Box の -X ~ X 頂点の間にあるかチェックする
		if (NumBetweenAtoB(SpherePos.y, BoxPos.y - BoxSize.y / 2.0f, BoxPos.y + BoxSize.y / 2.0f)) {	//球の Y 座標が Box の -Y ~ Y 頂点の間にあるかチェックする
			if (fabsf(SpherePos.z - BoxPos.z / 2.0f) < SphereRadius) {
				return true;
			}
		}
		if (NumBetweenAtoB(SpherePos.z, BoxPos.z - BoxSize.z / 2.0f, BoxPos.z + BoxSize.z / 2.0f)) {	//球の Z 座標が Box の -Z ~ Z 頂点の間にあるかチェックする
			if (fabsf(SpherePos.y - BoxPos.y / 2.0f) < SphereRadius) {
				return true;
			}
		}

		for (int i = 0; i < 4; i++) {

			if (HitCheckPoint_Circle(Vector3{SpherePos.y,SpherePos.z,0},								//４頂点それぞれに当たり判定の確認
				Vector3{	BoxPos.y + BoxSize.y / 2.0f * (float)((i >> 0) & 1 * 2 - 1),
							BoxPos.z + BoxSize.z / 2.0f * (float)((i >> 1) & 1 * 2 - 1), 0 },
				SphereRadius)) {

				return true;

			}

		}

	}

	else if (NumBetweenAtoB(SpherePos.y, BoxPos.y - BoxSize.y / 2.0f, BoxPos.y + BoxSize.y / 2.0f)) {	//球の Y 座標が Box の -Y ~ Y 頂点の間にあるかチェックする
		if (NumBetweenAtoB(SpherePos.z, BoxPos.z - BoxSize.z / 2.0f, BoxPos.z + BoxSize.z / 2.0f)) {	//球の Z 座標が Box の -Z ~ Z 頂点の間にあるかチェックする
			if (fabsf(SpherePos.y - BoxPos.y / 2.0f) < SphereRadius) {
				return true;
			}
		}

		for (int i = 0; i < 4; i++) {
					
			if (HitCheckPoint_Circle(Vector3{ SpherePos.x,SpherePos.z, 0 },								//４頂点それぞれに当たり判定の確認
				Vector3{	BoxPos.x + BoxSize.x / 2.0f * (float)((i >> 0) & 1 * 2 - 1),
							BoxPos.z + BoxSize.z / 2.0f * (float)((i >> 1) & 1 * 2 - 1), 0 },
				SphereRadius)) {

				return true;

			}

		}
	}
	else if (NumBetweenAtoB(SpherePos.z, BoxPos.z - BoxSize.z / 2.0f, BoxPos.z + BoxSize.z / 2.0f)) {	//球の Z 座標が Box の -Z ~ Z 頂点の間にあるかチェックする
		
		for (int i = 0; i < 4; i++) {

			if (HitCheckPoint_Circle(Vector3{ SpherePos.x,SpherePos.y, 0 },								//４頂点それぞれに当たり判定の確認
				Vector3{	BoxPos.x + BoxSize.x / 2.0f * (float)((i >> 0) & 1 * 2 - 1),
							BoxPos.y + BoxSize.y / 2.0f * (float)((i >> 1) & 1 * 2 - 1), 0 },
				SphereRadius)) {

				return true;

			}

		}
	}

	for (int i = 0; i < 8; i++) {

		if (HitCheckPoint_Sphere(SpherePos,																//８頂点それぞれに当たり判定の確認
			Vector3{	BoxPos.x + BoxSize.x / 2.0f * (float)((i >> 0) & 1 * 2 - 1),
						BoxPos.y + BoxSize.y / 2.0f * (float)((i >> 1) & 1 * 2 - 1),
						BoxPos.z + BoxSize.z / 2.0f * (float)((i >> 2) & 1 * 2 - 1) },
			SphereRadius)) {

			return true;

		}

	}

	return false;
}

// 箱 と 円 の当たり判定  ( 2D )
bool HitCheckBox_Circle(Vector3 BoxPos, Vector3 BoxSize, Vector3 CirclePos, float CircleRadius) {


	if (NumBetweenAtoB(CirclePos.x, BoxPos.x + BoxSize.x / 2.0f, BoxPos.x - BoxSize.x / 2.0f)) {
		if (NumBetweenAtoB(CirclePos.y, BoxPos.y + BoxSize.y / 2.0f, BoxPos.y - BoxSize.y / 2.0f)) {
			return true;
		}
		if (fabsf(CirclePos.y - BoxPos.y) < CircleRadius) {
			return true;
		}
	}
	else if (NumBetweenAtoB(CirclePos.y, BoxPos.y + BoxSize.y / 2.0f, BoxPos.y - BoxSize.y / 2.0f)) {
		if (fabsf(CirclePos.x - BoxPos.x) < CircleRadius) {
			return true;
		}
	}

	for (int i = 0; i < 4; i++) {

		if (HitCheckPoint_Circle(Vector3{ CirclePos.x,CirclePos.y, 0 },
			Vector3{	BoxPos.x + BoxSize.x / 2.0f * (float)((i >> 0) & 1 * 2 - 1),
						BoxPos.y + BoxSize.y / 2.0f * (float)((i >> 1) & 1 * 2 - 1), 0 },
			CircleRadius)) {

			return true;
		}
	}

	return false;
}

// 円 と 円 の当たり判定  ( 2D )
bool HitCheckCircle_Circle(Vector3 distans, float Circle1Radius, float Circle2Radius) {


	if (pow(distans.x,2.0f) + pow(distans.y, 2.0f) <
		pow(Circle1Radius + Circle2Radius, 2.0f)) {
		return true;
	}

	return false;
}



// "Num" が A と B の間にあるか確認する
template<typename T>
bool NumBetweenAtoB(T num, T A, T B) {


	if (num == A && num == B) {
		return true;
	}

	if (A < B) {
		if (A < num && num < B) {
			return true;
		}
	}
	else {
		if (B < num && num < A) {
			return true;
		}
	}

	return false;

}

// 点 と 球 の当たり判定
bool HitCheckPoint_Sphere(Vector3 SpherePos, Vector3 PointPos, float SpehreRadius) {

	if (pow(SpherePos.x - PointPos.x, 2.0f) + 
		pow(SpherePos.y - PointPos.y, 2.0f) + 
		pow(SpherePos.z - PointPos.z, 2.0f) < pow(SpehreRadius, 2.0f)) {

		return true;
	}

	return false;
}

// 点 と 円 の当たり判定  ( 2D )
bool HitCheckPoint_Circle(Vector3 CirclePos, Vector3 PointPos, float CircleRadius) {

	if (pow(CirclePos.x - PointPos.x, 2.0f) +
		pow(CirclePos.y - PointPos.y, 2.0f) < pow(CircleRadius, 2.0f)) {

		return true;
	}

	return false;
}

// 線 と 線 の当たり判定 ( 2D )
bool HitCheckLine_Line(Vector3 A_start, Vector3 A_end, Vector3 B_start, Vector3 B_end) {
	float ta = (B_start.x - B_end.x) * (A_start.y - B_start.y) + (B_start.y - B_end.y) * (B_start.x - A_start.x);
	float tb = (B_start.x - B_end.x) * (A_end.y - B_start.y) + (B_start.y - B_end.y) * (B_start.x - A_end.x);
	float tc = (A_start.x - A_end.x) * (B_start.y - A_start.y) + (A_start.y - A_end.y) * (A_start.x - B_start.x);
	float td = (A_start.x - A_end.x) * (B_end.y - A_start.y) + (A_start.y - A_end.y) * (A_start.x - B_end.x);

	//return tc * td < 0 && ta * tb < 0;
	return tc * td <= 0 && ta * tb <= 0; // 端点を含む場合
};
