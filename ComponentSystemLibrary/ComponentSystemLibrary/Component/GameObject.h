#pragma once
#include <string>
#include <vector>

#include "ComponentBaseClass.h"
#include "Transform.h"
#include "../DX11System/Vector.h"
#include "../Application.h"

class Transform;
class ComponentBase;

//オブジェクトタイプ
enum class ObjectType
{
	Player = 0b00,
	Enemy = 0b01,
	Obstracle = 0b11,
};

//ゲームオブジェクトの基底クラス
class GameObject
{
protected:

	std::string		name;				// 名前
	std::string		tag;				// タグ
	unsigned int	objectID;			// オブジェクトID番号
	bool			isExist;			// 生存可否
	ObjectType		objectType;			// オブジェクトタイプ

	std::vector<ComponentBase*> componentList;

#ifdef DEBUG
	bool			debug = false;		// デバッグの可否
#endif

public:
	Transform* transform;

	GameObject();
	virtual ~GameObject();

	std::string GetName() const { return name; }
	void SetName(std::string newName) { name = newName; }

	std::string GetTag() const { return tag; }
	void SetTag(std::string newTag) { tag = newTag; }

	bool GetExistState() const { return isExist; }
	void SetExistState(bool newState) { isExist = newState; }

	void SetObjectType(ObjectType newType) { objectType = newType; }
	ObjectType GetObjectType() { return objectType; }

#ifdef DEBUG
	void SetDebug(bool _flg) { debug = _flg; }
	bool GetDebug() { return debug; }
	bool *GetDebugAdress() { return &debug; }
#endif

	virtual void Start();

	virtual void Update();					// 更新処理
	virtual void PhysicsUpdate();			// 物理挙動
	virtual void LateUpdate();				// Update後に更新

	virtual void DrawObject();
	virtual void DrawUI();

	//--------------------------------------------------
	//			コンポーネント関係
	//--------------------------------------------------
	std::vector<ComponentBase*> GetComponentList()const { return componentList; }

	template<class T, class... Args>
	T* AddComponent(Args...);

	template<class T>
	T* GetComponent();

	template<class T>
	void RemoveComponent();
};

//----------------------------------------------------
//			コンポーネント関係
//----------------------------------------------------

//コンポーネントを追加する
//AddComponent<追加したいコンポーネントのクラス名>()という形で使う
template<class T, class... Args>
T* GameObject::AddComponent(Args... args)
{
	T* newComponent = new T(this, args...);//newしたのでdeleteを忘れずに（デストラクタでやってる）
	if (dynamic_cast<ComponentBase*>(newComponent) != nullptr)
	{
		componentList.emplace_back(newComponent);
	}

	return newComponent;
}

//コンポーネントを取得する
//GetComponent<コンポーネントのクラス名>()->コンポーネントのメソッド()という形で使う
template<class T>
T* GameObject::GetComponent()
{
	for (auto& com : componentList)
	{
		T* sp = dynamic_cast<T*>(com);
		if (sp != nullptr)
		{
			return sp;
		}
	}

	return nullptr;//当該コンポーネントがなければnullptrをreturn
}

//コンポーネントを削除する。使い方は上記二つに準ずる。
template<class T>
void GameObject::RemoveComponent()
{
	for (unsigned int i = 0; i < componentList.size(); i++) //削除はこっちのforが作りやすい
	{
		T* toRemove = dynamic_cast<T*>(componentList[i]);
		if (toRemove != nullptr)
		{
			componentList.erase(componentList.begin() + i);		//楽に作るとこうなるが、リストの要素数が増えるとeraseは重いので、別の方法を使った方が良い。
			componentList.shrink_to_fit();						//リストのcapasityとsizeの不一致が気になるならこれを入れてもいい。ただしほんの少し重くなる。

			return;
		}
	}
}

