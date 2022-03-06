#include "Scene.h"
#include "SceneManager.h"

Scene::Scene(std::string sceneName) : SceneName(sceneName) { 
	SceneManager::Instance()->AddScene(this); 
}
