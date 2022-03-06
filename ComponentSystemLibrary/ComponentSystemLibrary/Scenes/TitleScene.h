#pragma once

#include "../Component/Scene.h"

class TitleScene : public Scene {

public:
	TitleScene(std::string _str) : Scene(_str){}
	~TitleScene(){}

	void InitScene();

};