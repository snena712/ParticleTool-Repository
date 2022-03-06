#pragma once
#include "../../Component/Scene.h"
#include "../../Component/GameObject.h"

class SelectScene : public Scene {

public:
	SelectScene(std::string _str) : Scene(_str) {}
	~SelectScene() {}

	void InitScene();

};