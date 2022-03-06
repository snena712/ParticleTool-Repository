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

	std::vector<Scene*>	SceneList;			// シーンの一覧
	std::vector<Object>	EntityObject;		// オブジェクトの実体
	std::vector<Object>	InitList;			// 追加するオブジェクトのリスト

	std::vector<GameObject*> mDFOArray;		// 不透明描画リスト
	std::vector<std::pair<GameObject*, float>>	mDFTArray;		// 透過あり描画リスト
	std::vector<GameObject*>		mDFUArray;		// UI描画リスト

	Scene*	ThisScene;						// 現在のシーン
	bool	Move = false;
	std::string next;

	bool	debug = false;
	bool	flame = false;

	GameObject* debugCam;
	POINT ClickPos;
	Component::Camera* mainCam;

	SceneManager() {}
	void	RemoveEntityObject();			// 実体オブジェクトから除く
	void	UnActiveScene();				// オブジェクトをRemoveする


	bool ScreenCheck(Vector3 _pos, XMMATRIX _viewmtx,XMMATRIX _promtx);

public:

	static SceneManager* Instance() {
		static SceneManager singleton;
		return &singleton;
	}


	
	
	
	bool		ActiveScene(std::string);	// シーンをアクティブにする
	void		UninitScene();				// システム終了処理

	void		AddObject(GameObject* obj, bool staticType);	// 実体に加える
	void		AddList(std::vector<GameObject*> *list);	// リストを実体に加える
	void		AddScene(Scene* scene) { SceneList.emplace_back(scene); }

	bool		RemoveObject(GameObject* obj);	// 実体オブジェクトから除く( ポインタ )
	bool		RemoveObject(std::string str);	// 実体オブジェクトから除く( オブジェクト名 )

	template<typename T, typename... Args>
	T*			InitObject(Args... args) {
		T* hoge = new T(args...);

		InitList.emplace_back(Object{ hoge });
		return hoge;
	}

	auto		GetEntityObject() { return &EntityObject; }	// 実体オブジェクト配列を取得 (全シーン共通)

	GameObject*	GameObjectFindName(std::string);
	void		GameObjectFindNameList(std::string, std::vector<GameObject*>&);
	GameObject* GameObjectFindTag(std::string);
	void		GameObjectFindTagList(std::string, std::vector<GameObject*>&);
	GameObject* GameObjectFindPoint(Vector3);
	void		GameObjectFindArea(Vector3, float, std::vector<GameObject*>&);

	auto		GetSceneList() { return &SceneList; }		// シーン一覧の取得
	auto		GetActiveScene() { return ThisScene; }		// 現在シーンの取得
	auto		ReLoadThisScene() { ActiveScene(ThisScene->GetSceneName()); }

	static void	AddDrawFunction(RenderingType& _type, GameObject* _obj);
	static void DeleteDrawFunction(RenderingType& _type, GameObject* _obj);

	void		ManagerUpdate();
	void		ManagerDraw();

};


