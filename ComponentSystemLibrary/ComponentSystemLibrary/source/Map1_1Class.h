#pragma once
#include "../Component/Scene.h"

class Map1_1Class: public Scene {

public:
	Map1_1Class(std::string _str) : Scene(_str) {}
	~Map1_1Class() {}

	void InitScene();

};