#pragma once
#include "../../Component\GameObject.h"
#include "../../Component/QuadRenderComponent.h"
#include "TestComputeObj.h"
#include "../../DX11System\dx11mathutil.h"
#include "../../Component/GameObject.h"
#include <memory>

class EffectManager :public GameObject
{
public:


private:
	std::vector<GameObject*> m_bulletlist;

	GameObject* _enemana;
	GameObject* _player;

	const int max = 5;
public:
	EffectManager();
	~EffectManager();

	void Start();
	void Update();
	void Draw();

	void BulletChange();
};

