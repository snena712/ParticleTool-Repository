#pragma once

#include <vector>
#include <tuple>
#include <functional>
#include "GameObject.h"
#include "Scene.h"
#include "CameraComponent.h"
#include "RenderUtility.h"
#include "../source/PostEffect/DirectGraphicsTexture.h"


class GameObject;

struct Object {
	GameObject* obj;
	bool Static;
};


class SceneManager {

	std::vector<Scene*>	SceneList;			// �V�[���̈ꗗ
	std::vector<Object>	EntityObject;		// �I�u�W�F�N�g�̎���
	std::vector<Object>	InitList;			// �ǉ�����I�u�W�F�N�g�̃��X�g

	std::vector<GameObject*> mDFOArray;		// �s�����`�惊�X�g
	std::vector<std::pair<GameObject*, float>>	mDFTArray;		// ���߂���`�惊�X�g
	std::vector<GameObject*>		mDFUArray;		// UI�`�惊�X�g

	Scene*	ThisScene;						// ���݂̃V�[��
	bool	Move = false;
	std::string next;

	bool	debug = false;
	bool	flame = false;

	GameObject* debugCam;
	POINT ClickPos;
	Component::Camera* mainCam;

	SceneManager() {}
	void	RemoveEntityObject();			// ���̃I�u�W�F�N�g���珜��
	void	UnActiveScene();				// �I�u�W�F�N�g��Remove����


	bool ScreenCheck(Vector3 _pos, XMMATRIX _viewmtx,XMMATRIX _promtx);

public:

	static SceneManager* Instance() {
		static SceneManager singleton;
		return &singleton;
	}


	
	
	
	bool		ActiveScene(std::string);	// �V�[�����A�N�e�B�u�ɂ���
	void		UninitScene();				// �V�X�e���I������

	void		AddObject(GameObject* obj, bool staticType);	// ���̂ɉ�����
	void		AddList(std::vector<GameObject*> *list);	// ���X�g�����̂ɉ�����
	void		AddScene(Scene* scene) { SceneList.emplace_back(scene); }

	bool		RemoveObject(GameObject* obj);	// ���̃I�u�W�F�N�g���珜��( �|�C���^ )
	bool		RemoveObject(std::string str);	// ���̃I�u�W�F�N�g���珜��( �I�u�W�F�N�g�� )

	template<typename T, typename... Args>
	T*			InitObject(Args... args) {
		T* hoge = new T(args...);

		InitList.emplace_back(Object{ hoge });
		return hoge;
	}

	auto		GetEntityObject() { return &EntityObject; }	// ���̃I�u�W�F�N�g�z����擾 (�S�V�[������)

	GameObject*	GameObjectFindName(std::string);
	void		GameObjectFindNameList(std::string, std::vector<GameObject*>&);
	GameObject* GameObjectFindTag(std::string);
	void		GameObjectFindTagList(std::string, std::vector<GameObject*>&);
	GameObject* GameObjectFindPoint(Vector3);
	void		GameObjectFindArea(Vector3, float, std::vector<GameObject*>&);

	auto		GetSceneList() { return &SceneList; }		// �V�[���ꗗ�̎擾
	auto		GetActiveScene() { return ThisScene; }		// ���݃V�[���̎擾
	auto		ReLoadThisScene() { ActiveScene(ThisScene->GetSceneName()); }

	static void	AddDrawFunction(RenderingType& _type, GameObject* _obj);
	static void DeleteDrawFunction(RenderingType& _type, GameObject* _obj);

	void		ManagerUpdate();
	void		ManagerDraw();

};


