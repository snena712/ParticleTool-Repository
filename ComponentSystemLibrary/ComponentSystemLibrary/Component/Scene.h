#pragma once

#include <string>

class Scene {
	std::string SceneName;	//ÉVÅ[Éìñº
public:
	Scene(std::string sceneName);
	~Scene(){}

	virtual void InitScene (){}

	auto GetSceneName() { return SceneName; }
};