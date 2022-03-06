#pragma once
#include <math.h>
#include <DirectXMath.h>
#include <string>

#define PI (3.14159265358979323846)

class Vector3
{
public:

	float x, y, z;
	static const Vector3 zero;

	Vector3() : x(0), y(0), z(0) {};

	Vector3(float val)
	{
		(this->x) = val;
		(this->y) = val;
		(this->z) = val;
	}

	Vector3(float x, float y, float z)
	{
		(this->x) = x;
		(this->y) = y;
		(this->z) = z;
	};

	operator DirectX::XMFLOAT3() const {

		DirectX::XMFLOAT3 xm3;

		xm3.x = (this->x);
		xm3.y = (this->y);
		xm3.z = (this->z);

		return xm3;
	}

	operator DirectX::XMFLOAT4() const {

		DirectX::XMFLOAT4 xm4;

		xm4.x = (this->x);
		xm4.y = (this->y);
		xm4.z = (this->z);
		xm4.w = 0.0f;

		return xm4;
	}

	operator std::string() const {

		std::string str;

		str = "Vector3 ( " +
			std::to_string((this->x)) + ", " +
			std::to_string((this->y)) + ", " +
			std::to_string((this->x)) + " )";

		return str;
	}

	Vector3 operator + (const Vector3 dvec) const
	{
		Vector3 v3;

		v3.x = ((this)->x) + dvec.x;
		v3.y = ((this)->y) + dvec.y;
		v3.z = ((this)->z) + dvec.z;
		return v3;
	};

	Vector3 operator - (const Vector3 dvec) const
	{
		Vector3 v3;

		v3.x = ((this)->x) - dvec.x;
		v3.y = ((this)->y) - dvec.y;
		v3.z = ((this)->z) - dvec.z;
		return v3;
	};

	Vector3 operator * (float fDat) const
	{
		Vector3 v3;

		v3.x = ((this)->x) * fDat;
		v3.y = ((this)->y) * fDat;
		v3.z = ((this)->z) * fDat;

		return v3;
	};

	Vector3 operator * (const Vector3 fDat) const
	{
		Vector3 v3;

		v3.x = ((this)->x) * fDat.x;
		v3.y = ((this)->y) * fDat.y;
		v3.z = ((this)->z) * fDat.z;

		return v3;
	};

	Vector3 operator / (float fDat) const
	{
		Vector3 v3;

		if (fDat == 0.0f)
		{
			return *this;
		}

		v3.x = ((this)->x) / fDat;
		v3.y = ((this)->y) / fDat;
		v3.z = ((this)->z) / fDat;
		return v3;
	};

	Vector3 operator / (const Vector3 fDat) const
	{
		Vector3 v3;

		v3.x = ((this)->x) / fDat.x;
		v3.y = ((this)->y) / fDat.y;
		v3.z = ((this)->z) / fDat.z;

		return v3;
	};

	bool operator < (const float f) const
	{

		if (this->x > f) {
			return false;
		}
		if (this->y > f) {
			return false;
		}
		if (this->z > f) {
			return false;
		}

		return true;
	};

	bool operator > (const float f) const
	{

		if (this->x < f) {
			return false;
		}
		if (this->y < f) {
			return false;
		}
		if (this->z < f) {
			return false;
		}

		return true;
	};


	Vector3& operator += (const Vector3 dvec)
	{
		((this)->x) += dvec.x;
		((this)->y) += dvec.y;
		((this)->z) += dvec.z;
		return *this;
	};

	Vector3& operator -= (const Vector3 dvec)
	{
		((this)->x) -= dvec.x;
		((this)->y) -= dvec.y;
		((this)->z) -= dvec.z;
		return *this;
	};

	Vector3& operator *= (float fDat)
	{
		((this)->x) *= fDat;
		((this)->y) *= fDat;
		((this)->z) *= fDat;
		return *this;
	};

	Vector3& operator *= (const Vector3 fDat)
	{
		((this)->x) *= fDat.x;
		((this)->y) *= fDat.y;
		((this)->z) *= fDat.z;
		return *this;
	};

	Vector3& operator /= (float fDat)
	{

		if (fDat == 0.0f)
		{
			return *this;
		}

		((this)->x) /= fDat;
		((this)->y) /= fDat;
		((this)->z) /= fDat;
		return *this;
	};

	Vector3& operator /= (const Vector3 fDat)
	{

		if (fDat.x == 0.0f || fDat.y == 0.0f || fDat.z == 0.0f)
		{
			return *this;
		}

		((this)->x) /= fDat.x;
		((this)->y) /= fDat.y;
		((this)->z) /= fDat.z;
		return *this;
	};

	bool operator == (const Vector3 dvec) const
	{
		if ((this->x) != dvec.x)
			return false;
		if ((this->y) != dvec.y)
			return false;
		if ((this->z
			) != dvec.z)
			return false;

		return true;
	}

	bool operator != (const Vector3 dvec) const
	{
		if ((this->x) == dvec.x)
			return false;
		if ((this->y) == dvec.y)
			return false;
		if ((this->z) == dvec.z)
			return false;

		return true;
	}

	Vector3 operator = (const DirectX::XMFLOAT3 dvec) const
	{
		Vector3 v3;

		v3.x = dvec.x;
		v3.y = dvec.y;
		v3.z = dvec.z;
		return v3;
	};

	Vector3 operator + (const DirectX::XMFLOAT3 dvec) const
	{
		Vector3 v3;

		v3.x = ((this)->x) + dvec.x;
		v3.y = ((this)->y) + dvec.y;
		v3.z = ((this)->z) + dvec.z;
		return v3;
	};

	Vector3 operator - (const DirectX::XMFLOAT3 dvec) const
	{
		Vector3 v3;

		v3.x = ((this)->x) - dvec.x;
		v3.y = ((this)->y) - dvec.y;
		v3.z = ((this)->z) - dvec.z;
		return v3;
	};

	Vector3 operator * (const DirectX::XMFLOAT3 fDat) const
	{
		Vector3 v3;

		v3.x = ((this)->x) * fDat.x;
		v3.y = ((this)->y) * fDat.y;
		v3.z = ((this)->z) * fDat.z;

		return v3;
	};

	Vector3 operator / (const DirectX::XMFLOAT3 fDat) const
	{
		Vector3 v3;

		v3.x = ((this)->x) / fDat.x;
		v3.y = ((this)->y) / fDat.y;
		v3.z = ((this)->z) / fDat.z;

		return v3;
	};

	Vector3& operator -= (const DirectX::XMFLOAT3 dvec)
	{
		((this)->x) -= dvec.x;
		((this)->y) -= dvec.y;
		((this)->z) -= dvec.z;
		return *this;
	};

	Vector3& operator *= (const DirectX::XMFLOAT3 fDat)
	{
		((this)->x) *= fDat.x;
		((this)->y) *= fDat.y;
		((this)->z) *= fDat.z;
		return *this;
	};

	Vector3& operator /= (const DirectX::XMFLOAT3 fDat)
	{

		if (fDat.x == 0.0f || fDat.y == 0.0f || fDat.z == 0.0f)
		{
			return *this;
		}

		((this)->x) /= fDat.x;
		((this)->y) /= fDat.y;
		((this)->z) /= fDat.z;
		return *this;
	};

	bool operator == (const DirectX::XMFLOAT3 dvec) const
	{
		if ((this->x) != dvec.x)
			return false;
		if ((this->y) != dvec.y)
			return false;
		if ((this->z) != dvec.z)
			return false;

		return true;
	}

	bool operator != (const DirectX::XMFLOAT3 dvec) const
	{
		if ((this->x) == dvec.x)
			return false;
		if ((this->y) == dvec.y)
			return false;
		if ((this->z) == dvec.z)
			return false;

		return true;
	}

	Vector3 Normalize()
	{
		double len;
		double x, y, z;
		Vector3 out;

		x = (double)(this->x);	//精度保持のため一度doubleでやる
		y = (double)(this->y);
		z = (double)(this->z);

		len = sqrt(x * x + y * y + z * z);

		if (len < (1e-6)) return Vector3(0);

		len = 1.0 / len;
		x *= len;
		y *= len;
		z *= len;

		out.x = (float)x;
		out.y = (float)y;
		out.z = (float)z;

		return out;
	}

	float Magnitude()	//ベクトルの長さを返す
	{
		double len;
		double x, y, z;

		x = (double)(this->x);	//精度保持のため一度doubleでやる
		y = (double)(this->y);
		z = (double)(this->z);

		len = sqrt(x * x + y * y + z * z);

		return len;
	}

	Vector3 Direction(Vector3 v2)//自分から引数に向いたベクトルを返す
	{
		Vector3 direction;
		direction = v2 - (*this);
		direction.Normalize();
		return direction;
	}

	float Distance(Vector3 dvec) {
		return sqrt(pow(this->x - dvec.x, 2.0f) + pow(this->y - dvec.y, 2.0f) + pow(this->z - dvec.z, 2.0f));
	}

	float Dot(Vector3 dvec)
	{
		return ((this->x)*(dvec.x) + (this->y)*(dvec.y) + (this->z)*(dvec.z));
	}

	Vector3 Cross(Vector3 dvec)
	{
		Vector3 vec;
		double x1, y1, z1, x2, y2, z2;

		x1 = (double)(this->x);
		y1 = (double)(this->y);
		z1 = (double)(this->z);
		x2 = (double)(dvec.x);
		y2 = (double)(dvec.y);
		z2 = (double)(dvec.z);

		vec.x = (float)(y1 * z2 - z1 * y2);
		vec.y = (float)(z1 * x2 - x1 * z2);
		vec.z = (float)(x1 * y2 - y1 * x2);

		return vec;
	}

	float Radian(Vector3 dvec) {

		Vector3 avec, bvec;
		avec = this->Normalize();
		bvec = dvec.Normalize();

		if (avec < (1e-6) || bvec < (1e-6)) {
			return NAN;
		}

		float dot = avec.Dot(bvec);

		return acosf(dot);
	}

	float Degree(Vector3 dvec) {
		if (*this < (1e-6) || dvec < (1e-6)) {
			return NAN;
		}
		return this->Radian(dvec) / PI * 180.0f;
	}

	void EulerAnglesToQuaternion(DirectX::XMFLOAT4& qt)
	{
		float x, y, z;

		x = (this->x) / 180.0f * PI;
		y = (this->y) / 180.0f * PI;
		z = (this->z) / 180.0f * PI;

		float cosRoll = cos(x / 2.0);
		float sinRoll = sin(x / 2.0);
		float cosPitch = cos(y / 2.0);
		float sinPitch = sin(y / 2.0);
		float cosYaw = cos(z / 2.0);
		float sinYaw = sin(z / 2.0);

		qt.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
		qt.x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		qt.y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		qt.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	}

	void QuaternionToEulerAngles(DirectX::XMFLOAT4 qt)
	{
		float q0q0 = qt.w * qt.w;
		float q0q1 = qt.w * qt.x;
		float q0q2 = qt.w * qt.y;
		float q0q3 = qt.w * qt.z;
		float q1q1 = qt.x * qt.x;
		float q1q2 = qt.x * qt.y;
		float q1q3 = qt.x * qt.z;
		float q2q2 = qt.y * qt.y;
		float q2q3 = qt.y * qt.z;
		float q3q3 = qt.z * qt.z;

		(this->x) = atan2(2.0 * (q2q3 + q0q1), q0q0 - q1q1 - q2q2 + q3q3) * 180.0f / PI;
		(this->y) = asin(2.0 * (q0q2 - q1q3)) * 180.0f / PI;
		(this->z) = atan2(2.0 * (q1q2 + q0q3), q0q0 + q1q1 - q2q2 - q3q3) * 180.0f / PI;
	}
};

class Vector2
{
public:
	float x, y;
	Vector2() : x(0), y(0) {};
	Vector2(float val)
	{
		(this->x) = val;
		(this->y) = val;
	}
	Vector2(float x, float y)
	{
		(this->x) = x;
		(this->y) = y;
	};

	operator DirectX::XMFLOAT2() const {

		DirectX::XMFLOAT2 xm2;

		xm2.x = (this->x);
		xm2.y = (this->y);

		return xm2;
	}

	operator std::string() const {

		std::string str;

		str = "Vector2 ( " +
			std::to_string((this->x)) + ", " +
			std::to_string((this->y)) + " )";

		return str;
	}

	Vector2 operator + (const Vector2 dvec) const
	{
		Vector2 v2;

		v2.x = ((this)->x) + dvec.x;
		v2.y = ((this)->y) + dvec.y;
		return v2;
	};

	Vector2 operator - (const Vector2 dvec) const
	{
		Vector2 v2;

		v2.x = ((this)->x) - dvec.x;
		v2.y = ((this)->y) - dvec.y;
		return v2;
	};

	Vector2 operator * (float fDat) const
	{
		Vector2 v2;

		v2.x = ((this)->x) * fDat;
		v2.y = ((this)->y) * fDat;
		return v2;
	};

	Vector2 operator * (const Vector2 fDat) const
	{
		Vector2 v2;

		v2.x = ((this)->x) * fDat.x;
		v2.y = ((this)->y) * fDat.y;
		return v2;
	};

	Vector2 operator / (float fDat) const
	{
		Vector2 v2;

		if (fDat == 0.0f)
		{
			return *this;
		}

		v2.x = ((this)->x) / fDat;
		v2.y = ((this)->y) / fDat;
		return v2;
	};

	Vector2 operator / (const Vector2 fDat) const
	{
		Vector2 v2;

		if (fDat.x == 0.0f || fDat.y == 0.0f)
		{
			return *this;
		}

		v2.x = ((this)->x) / fDat.x;
		v2.y = ((this)->y) / fDat.y;
		return v2;
	};

	bool operator < (const float f) const
	{

		if (this->x > f) {
			return false;
		}
		if (this->y > f) {
			return false;
		}

		return true;
	};

	bool operator > (const float f) const
	{

		if (this->x < f) {
			return false;
		}
		if (this->y < f) {
			return false;
		}

		return true;
	};

	Vector2& operator += (float dvec)
	{
		((this)->x) += dvec;
		((this)->y) += dvec;
		return *this;
	};

	Vector2& operator += (const Vector2 dvec)
	{
		((this)->x) += dvec.x;
		((this)->y) += dvec.y;
		return *this;
	};

	Vector2& operator -= (float dvec)
	{
		((this)->x) -= dvec;
		((this)->y) -= dvec;
		return *this;
	};

	Vector2& operator -= (const Vector2 dvec)
	{
		((this)->x) -= dvec.x;
		((this)->y) -= dvec.y;
		return *this;
	};

	Vector2& operator *= (float fDat)
	{
		((this)->x) *= fDat;
		((this)->y) *= fDat;
		return *this;
	};

	Vector2& operator *= (const Vector2 fDat)
	{
		((this)->x) *= fDat.x;
		((this)->y) *= fDat.y;
		return *this;
	};

	Vector2& operator /= (float fDat)
	{

		if (fDat == 0.0f)
		{
			return *this;
		}

		((this)->x) /= fDat;
		((this)->y) /= fDat;
		return *this;
	};

	Vector2& operator /= (const Vector2 fDat)
	{

		if (fDat.x == 0.0f || fDat.y == 0.0f)
		{
			return *this;
		}

		((this)->x) /= fDat.x;
		((this)->y) /= fDat.y;
		return *this;
	};

	bool operator == (const Vector2 dvec) const
	{
		if ((this->x) != dvec.x)
			return false;
		if ((this->y) != dvec.y)
			return false;

		return true;
	}

	bool operator != (const Vector2 dvec) const
	{
		if ((this->x) == dvec.x)
			return false;
		if ((this->y) == dvec.y)
			return false;

		return true;
	}

	Vector2 Normalize()
	{
		double len;
		double x, y;
		Vector2 out;

		x = (double)(this->x);//精度保持のため一度doubleでやる
		y = (double)(this->y);

		len = sqrt(x * x + y * y);

		if (len < (1e-6)) return 0;

		len = 1.0 / len;
		x *= len;
		y *= len;

		out.x = (float)x;
		out.y = (float)y;

		return out;
	}

	float Magnitude()	//ベクトルの長さを返す
	{
		double len;
		double x, y;

		x = (double)(this->x);	//精度保持のため一度doubleでやる
		y = (double)(this->y);

		len = sqrt(x * x + y * y);

		return len;
	}

	Vector2 Direction(Vector2 v2)//自分から引数に向いたベクトルを返す
	{
		Vector2 direction;
		direction = v2 - (*this);
		direction.Normalize();
		return direction;
	}

	float Distance(Vector2 dvec) {
		return sqrt(pow(this->x - dvec.x, 2.0f) + pow(this->y - dvec.y, 2.0f));
	}

	float Dot(Vector2 dvec)
	{
		return ((this->x)*(dvec.x) + (this->y)*(dvec.y));
	}

	float Radian(Vector2 dvec) {


		Vector2 avec, bvec;
		avec = this->Normalize();
		bvec = dvec.Normalize();

		if (avec < (1e-6) || bvec < (1e-6)) {
			return NAN;
		}

		float dot = avec.Dot(bvec);

		return acosf(dot);

	}

	float Degree(Vector2 dvec) {
		if (*this < (1e-6) || dvec < (1e-6)) {
			return NAN;
		}
		return this->Radian(dvec) / PI * 180.0f;
	}
};