#pragma once
#include "../DX11System/Vector.h"
#include "GameObject.h"
#include <vector>
#include <DirectXMath.h>
#include "../DX11System/dx11mathutil.h"
#include "ImGuiComponent.h"

class GameObject;

class Transform
{
private:
	Vector3		oldPos;
	Transform*	parent;
	std::vector<Transform*> child;

public:
	GameObject* gameObject = NULL;

	DirectX::XMFLOAT4	qt;
	Vector3				position;
	Vector3				scale;

	Transform() :position(0, 0, 0), oldPos(0, 0, 0), qt(0, 0, 0, 1), scale(1, 1, 1), parent(nullptr) {}
	~Transform() {}

#ifdef DEBUG
	void ImGuiDebug();
#endif

	Vector3	OldPosition() { return oldPos; }

	Vector3	right() {
		DirectX::XMFLOAT4X4 mat;
		DX11MtxFromQt(mat, qt);

		return Vector3{ mat._11,mat._12,mat._13 };
	}
	Vector3 up() {
		DirectX::XMFLOAT4X4 mat;
		DX11MtxFromQt(mat, qt);

		return Vector3{ mat._21,mat._22,mat._23 };
	}
	Vector3	forward() {
		DirectX::XMFLOAT4X4 mat;
		DX11MtxFromQt(mat, qt);

		return Vector3{ mat._31,mat._32,mat._33 };
	}

	void RotateXaxis(float angle) {
		DirectX::XMFLOAT4 q, axis;
		axis = { 1,0,0,0 };

		DX11QtRotationAxis(q, axis, angle);

		DX11QtMul(qt, qt, q);
	}
	void RotateYaxis(float angle) {
		DirectX::XMFLOAT4 q, axis;
		axis = { 0,1,0,0 };

		DX11QtRotationAxis(q, axis, angle);

		DX11QtMul(qt, qt, q);
	}
	void RotateZaxis(float angle) {
		DirectX::XMFLOAT4 q, axis;
		axis = { 0,0,1,0 };

		DX11QtRotationAxis(q, axis, angle);

		DX11QtMul(qt, qt, q);
	}

	void RotateLocalXaxis(float angle) {
		DirectX::XMFLOAT4 q, axis;
		axis = right();

		DX11QtRotationAxis(q, axis, angle);

		DX11QtMul(qt, qt, q);
	}
	void RotateLocalYaxis(float angle) {
		DirectX::XMFLOAT4 q, axis;
		axis = up();

		DX11QtRotationAxis(q, axis, angle);

		DX11QtMul(qt, qt, q);
	}
	void RotateLocalZaxis(float angle) {
		DirectX::XMFLOAT4 q, axis;
		axis = forward();

		DX11QtRotationAxis(q, axis, angle);

		DX11QtMul(qt, qt, q);
	}

	auto GetLookQt(Vector3 lookPos,Vector3 _up) {
		DirectX::XMFLOAT4 _qt;
		Vector3 z = (lookPos - position).Normalize();
		Vector3 x = _up.Cross(z).Normalize();
		Vector3 y = z.Cross(x).Normalize();

		DirectX::XMFLOAT4X4 mat;
		mat._11 = x.x; mat._12 = x.y; mat._13 = x.z;
		mat._21 = y.x; mat._22 = y.y; mat._23 = y.z;
		mat._31 = z.x; mat._32 = z.y; mat._33 = z.z;

		DX11GetQtfromMatrix(mat, _qt);
		return _qt;
	}
	void SlerpRotate(DirectX::XMFLOAT4 toqt, const float t) {
		DX11QtSlerp(qt, toqt, t, qt);
	}
	void LookAt(Vector3 lookPos,Vector3 _up) {
		qt = GetLookQt(lookPos,_up);
	}
	void LookSlerp(Vector3 lookPos, const float t) {

		DirectX::XMFLOAT4 _qt;
		_qt = GetLookQt(lookPos, Vector3(0, 1, 0));
		DX11QtSlerp(qt, _qt, t, qt);
	}

	auto GetLookStreat(Vector3 lookPos, Vector3 _up) {
		DirectX::XMFLOAT4 _qt;
		Vector3 z = (lookPos - position).Normalize();
		Vector3 x = _up.Cross(z).Normalize();
		Vector3 y = z.Cross(x).Normalize();

		DirectX::XMFLOAT4X4 mat;
		mat._11 = x.x; mat._12 = x.y; mat._13 = x.z;
		mat._21 = y.x; mat._22 = y.y; mat._23 = y.z;
		mat._31 = z.x; mat._32 = z.y; mat._33 = z.z;

		DX11GetQtfromMatrix(mat, _qt);
		return _qt;
	}

	auto GetParent() { return parent; }

	void SetParent(Transform* Parent) {

		if (parent != nullptr &&
			Parent != parent) {
			parent->RemoveChild(this);
		}

		parent = Parent;
		parent->SetChild(this);
	}

	void RemoveParent() {
		if (parent != nullptr) {
			parent->RemoveChild(this);
			parent = nullptr;
		}
	}

	Transform* GetChild(int num) {

		if (child.size() == 0) {
			return nullptr;
		}
		if (child.size() <= num) {
			return nullptr;
		}
		else {
			auto itr = child.begin();
			itr += num;
			return (Transform*)(*itr);
		}
	}

	void SetChild(Transform* Child) {
		auto itr = child.begin();
		while (itr != child.end()) {
			if ((*itr) == Child) {
				return;
			}
			else {
				itr++;
			}
		}

		child.emplace_back(Child);
		Child->SetParent(this);
	}

	void RemoveChild(Transform* Child) {
		auto itr = child.begin();
		while (itr != child.end()) {
			if ((*itr) == Child) {
				itr = child.erase(itr);
				Child->RemoveParent();
			}
			else {
				itr++;
			}
		}
	}

	static void GetMultiplParentTransform(XMFLOAT4X4& output, Transform* tra) {

		if (tra->GetParent() == nullptr) {
			return;
		}
		else {

			DirectX::XMFLOAT4X4 make, mtx, scale;
			Transform *t = tra->GetParent();

			DX11MtxScale(t->scale.x, t->scale.y, t->scale.z, scale);
			DX11MtxFromQt(mtx, t->qt);
			DX11MtxMultiply(make, scale, mtx);

			make._41 = t->position.x;
			make._42 = t->position.y;
			make._43 = t->position.z;

			DX11MtxMultiply(output, output, make);

			GetMultiplParentTransform(output, tra->GetParent());
		}

	}


	auto GetWorldPosition() {
		if (parent == nullptr) {
			return position;
		}
		else {
			DirectX::XMFLOAT4X4 mat;

			DX11MtxFromQt(mat, qt);

			mat._41 = position.x;
			mat._42 = position.y;
			mat._43 = position.z;

			GetMultiplParentTransform(mat, this);

			return Vector3{ mat._41,mat._42,mat._43 };
		}
	}

	auto GetWorldQt() {
		if (parent == nullptr) {
			return qt;
		}
		else {
			DirectX::XMFLOAT4X4 make, mat;

			DX11MtxIdentity(make);

			Transform* tra = this;

			while (tra != nullptr) {
				DX11MtxFromQt(mat, tra->qt);
				DX11MtxMultiply(make, make, mat);
				tra = tra->GetParent();
			}

			DirectX::XMFLOAT4 t;
			DX11GetQtfromMatrix(make, t);
			return t;
		}
	}
};
