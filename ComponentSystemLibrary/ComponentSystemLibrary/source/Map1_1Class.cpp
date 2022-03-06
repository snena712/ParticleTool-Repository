#pragma once
#include "Map1_1Class.h"
#include "../GameObjects/MainCamera.h"
#include "../Component/SceneManager.h"
#include "../DX11System/XAudio2.h"
#include "Compute\TestComputeObj.h"
#include "UI/Fade/FadeManager.h"
#include "Map/TileLine.h"
#include "UI/Arrow/ArrowUI.h"
#include <vector>
#include <iostream>

#include "../source/PostEffect/Shimmer.h"


void Map1_1Class::InitScene() {

	//ParticleEffect *par = new ParticleEffect();
	//AddEntityObject(par, false);

	std::vector<GameObject*> _objList;


	GameObject* cam = new GameObject();
	cam->AddComponent<Component::MainCamera>();
	_objList.emplace_back(cam);


	//�p�[�e�B�N��
	GameObject* efemana = new GameObject();
	efemana->AddComponent<Component::TestComputeObj>();
	_objList.emplace_back(efemana);

	//GameObject* efemana2 = new GameObject();
	//efemana2->AddComponent<Component::TestComputeObj>();
	//_objList.emplace_back(efemana2);

	// �|�X�g�G�t�F�N�g
	GameObject* sim = new GameObject();
	sim->AddComponent<Component::Shimmer>();
	_objList.emplace_back(sim);


	SceneManager::Instance()->AddList(&_objList);

	//PlaySound(SOUND_LABEL_BGM000);
}

