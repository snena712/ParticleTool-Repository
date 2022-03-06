#pragma once
#include "GameObject.h"
#include "../Application.h"
#include <Windows.h>

class GameObject;

class ComponentBase
{
protected:
	GameObject* owner;//自身を所有しているゲームオブジェクト

public:
	ComponentBase(GameObject* Owner) : owner(Owner) { }
	virtual ~ComponentBase() { }

	GameObject* GetOwner() { return owner; }
	GameObject* SetOwner(GameObject* newowner) { owner = newowner;  return owner; }

	virtual void Start() {}
	virtual void Update() {}
	virtual void PhysicsUpdate() {}
	virtual void LateUpdate() {}
	virtual void DrawObject() {}
	virtual void DrawUI() {}

#ifdef DEBUG
	virtual void ImGuiDebug() {}
#endif

};