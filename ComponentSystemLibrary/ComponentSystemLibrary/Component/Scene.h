#pragma once

#include <string>

class Scene {
	std::string SceneName;	//�V�[����
public:
	Scene(std::string sceneName);
	~Scene(){}

	virtual void InitScene (){}

	auto GetSceneName() { return SceneName; }
};