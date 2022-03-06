#pragma once

#include "TitleScene.h"
#include "../GameObjects/MainCamera.h"
#include "../GameObjects/PlayerObj.h"
#include "../Component/SceneManager.h"
#include "../source/Title/TitlePlayer.h"
#include "../source/UI/TitleUI/PressButton.h"
#include "../source/UI/TitleUI/TitleMainUI.h"
#include "../source/Title/BackLoad3DObj.h"
#include <vector>
#include <iostream>


void TitleScene::InitScene() {

	std::vector<GameObject*> _objList;

	/*PlayerObj* player = new PlayerObj();
	_objList.emplace_back(player);*/

	GameObject *cam = new GameObject();
	cam->AddComponent < Component::MainCamera >();
	_objList.emplace_back(cam);

	GameObject* tplayer = new GameObject();
	tplayer->AddComponent<Component::TitlePlayer>();
	_objList.emplace_back(tplayer);

	// ƒ‚ƒfƒ‹“Ç‚Ýž‚Ý
	GameObject* load = new GameObject();
	load->AddComponent<Component::BackLoad3DObj>();
	_objList.emplace_back(load);

	SceneManager::Instance()->AddList(&_objList);
}

