#include "GameObject.h"
#include "SceneManager.h"

GameObject::GameObject() :name("NoName"), isExist(true), objectType(ObjectType::Obstracle)
{
	transform = new Transform();
	transform->gameObject = this;

	static unsigned int id = 0;
	objectID = id++;

}

GameObject::~GameObject()
{
	name.clear();
	for (auto& component : componentList)
	{
		delete component;
	}

	componentList.shrink_to_fit();
}

void GameObject::Start()
{
	for (auto& component : componentList)
	{
		component->Start();
	}
}

void GameObject::Update()
{
	for (auto& component : componentList)
	{
		component->Update();
	}
}

void GameObject::PhysicsUpdate()
{
	for (auto& component : componentList)
	{
		component->PhysicsUpdate();
	}
}

void GameObject::LateUpdate()
{
	for (auto& component : componentList)
	{
		component->LateUpdate();
	}
}

void GameObject::DrawObject()
{
	for (auto& component : componentList)
	{
		component->DrawObject();
	}
}

void GameObject::DrawUI()
{
	for (auto& component : componentList)
	{
		component->DrawUI();
	}
}
