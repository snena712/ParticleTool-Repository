#include "SceneManager.h"
#include "Transform.h"
#include "CameraComponent.h"
#include "ImGuiComponent.h"
#include <DirectXMath.h>
#include <utility>
#include <thread>
#include "../DX11System/mystring.h"
#include "../source/PostEffect/DirectGraphicsTexture.h"
#include "../DX11System/DX11Settransform.h"
#include "../source/Compute/ComputeManager.h"
#include "../source/PostEffect/PostEffectManager.h"


/*------------------------
終了処理
--------------------------*/
void SceneManager::UninitScene() {
	for (auto& scene : *SceneManager::GetSceneList()) {
		delete scene;
	}
	SceneManager::GetSceneList()->clear();
}

/*------------------------
指定したシーンのオブジェクトを生成する
--------------------------*/
bool SceneManager::ActiveScene(std::string str) {
	for (Scene* scene : SceneList)
	{
		if (scene->GetSceneName() == str) {

			next = str;
			Move = true;

			return true;
		}
	}
	return false;
}

/*------------------------
現在のオブジェクトを削除する
静的なオブジェクトは残す
--------------------------*/
void SceneManager::UnActiveScene() {

	for (auto& obj : EntityObject) {
		if (!obj.Static)
			delete obj.obj;
	}

	// 削除した実体リストを最適化
	auto itr = EntityObject.begin();
	while (itr != EntityObject.end()) {
		if (!(*itr).Static) {
			itr = EntityObject.erase(itr);
		}
		else {
			itr++;
		}
	}
	EntityObject.shrink_to_fit();
}

/*------------------------
オブジェクトの追加
--------------------------*/
void SceneManager::AddObject(GameObject* obj, bool staticType) {
	InitList.emplace_back(Object{ obj, staticType });
}

/*------------------------
オブジェクトの追加(リスト)
--------------------------*/
void SceneManager::AddList(std::vector<GameObject*> *list) {
	for (GameObject* obj : *list) {
		InitList.emplace_back(Object{ obj,false });
	}
}

/*------------------------
指定したオブジェクトを削除する
--------------------------*/
bool SceneManager::RemoveObject(GameObject* obj) {

	auto itr = EntityObject.begin();
	while (itr != EntityObject.end()) {
		if ((*itr).obj == obj) {
			delete (*itr).obj;
			(*itr).obj = nullptr;
			return true;
		}
		else {
			itr++;
		}
	}
	return false;
}

/*------------------------
指定したオブジェクトを削除する
--------------------------*/
bool SceneManager::RemoveObject(std::string str) {

	auto itr = EntityObject.begin();
	while (itr != EntityObject.end()) {
		if ((*itr).obj->GetName() == str) {
			delete (*itr).obj;
			(*itr).obj = nullptr;
			return true;
		}
		else {
			itr++;
		}
	}
	return false;
}

/*------------------------
オブジェクト削除後のリサイズ
--------------------------*/
void SceneManager::RemoveEntityObject() {

	auto itr = EntityObject.begin();
	while (itr != EntityObject.end()) {
		if ((*itr).obj == nullptr) {
			itr = EntityObject.erase(itr);
		}
		else {
			itr++;
		}
	}
}

/*------------------------
特定のオブジェクトを探す
--------------------------*/
GameObject* SceneManager::GameObjectFindName(std::string str) {
	for (auto hoge : EntityObject) {
		if (hoge.obj->GetName() == str) {
			return hoge.obj;
		}
	}
	return NULL;
}

/*------------------------
特定のオブジェクトを全て探す
--------------------------*/
void SceneManager::GameObjectFindNameList(std::string str, std::vector<GameObject*> &list) {
	for (auto hoge : EntityObject) {
		if (hoge.obj->GetName() == str) {
			list.emplace_back(hoge.obj);
		}
	}
}

/*------------------------
特定のオブジェクトを探す
--------------------------*/
GameObject* SceneManager::GameObjectFindTag(std::string str) {
	for (auto hoge : EntityObject) {
		if (hoge.obj->GetTag() == str) {
			return hoge.obj;
		}
	}
	return NULL;
}

/*------------------------
特定のオブジェクトを全て探す
--------------------------*/
void SceneManager::GameObjectFindTagList(std::string str, std::vector<GameObject*>& list) {
	for (auto hoge : EntityObject) {
		if (hoge.obj->GetTag() == str) {
			list.emplace_back(hoge.obj);
		}
	}
}

/*------------------------
最も近いオブジェクトを探す
--------------------------*/
GameObject* SceneManager::GameObjectFindPoint(Vector3 pos) {

	auto itr = EntityObject.begin();
	auto ret = itr;
	float distance = pos.Distance((*itr).obj->transform->position);
	float num;

	itr++;
	while (itr != EntityObject.end())
	{
		num = pos.Distance((*itr).obj->transform->position);

		if (num < distance) {
			ret = itr;
			distance = num;
		}
		itr++;
	}

	return (*ret).obj;
}

/*------------------------
一定範囲のオブジェクトを全て探す
--------------------------*/
void SceneManager::GameObjectFindArea(Vector3 pos, float distance, std::vector<GameObject*>& list) {

	auto itr = EntityObject.begin();
	while (itr != EntityObject.end()) {
		Vector3 p = (*itr).obj->transform->position;
		if ((pos.x - p.x) * (pos.x - p.x) +
			(pos.y - p.y) * (pos.y - p.y) +
			(pos.z - p.z) * (pos.z - p.z)
			< distance * distance) {
			list.emplace_back((*itr).obj);
		}

		itr++;
	}
}

/*------------------------------------
		描画処理の登録
-------------------------------------*/
void SceneManager::AddDrawFunction(RenderingType& _type, GameObject* _obj)
{
	auto ins = Instance();

	switch (_type)
	{
	case RenderingType::Opaque:


		ins->mDFOArray.emplace_back(_obj);

		break;

	case RenderingType::Transparent:


		ins->mDFTArray.emplace_back(_obj, 0.0f);

		break;

	case RenderingType::UI2D:
		ins->mDFUArray.emplace_back(_obj);
		break;

	default:
		break;
	}
}

/*------------------------------------
		描画処理の削除
-------------------------------------*/
void SceneManager::DeleteDrawFunction(RenderingType& _type, GameObject* _obj)
{
	auto ins = Instance();

	switch (_type)
	{
	case RenderingType::Opaque:
		for (int i = 0; i < ins->mDFOArray.size(); i++)
		{
			if (ins->mDFOArray[i] == _obj)
			{
				ins->mDFOArray.erase(ins->mDFOArray.begin() + i);
			}
		}
		break;

	case RenderingType::Transparent:
		for (int i = 0; i < ins->mDFTArray.size(); i++)
		{
			if (ins->mDFTArray[i].first == _obj)
			{
				ins->mDFTArray.erase(ins->mDFTArray.begin() + i);
			}
		}
		break;

	case RenderingType::UI2D:
		for (int i = 0; i < ins->mDFUArray.size(); i++)
		{
			if (ins->mDFUArray[i] == _obj)
			{
				ins->mDFUArray.erase(ins->mDFUArray.begin() + i);
			}
		}
		break;

	default:
		break;
	}
}

/*------------------------
Update処理
--------------------------
1. 生成したオブジェクトのリスト追加
2. 生成したオブジェクトの Start()処理
3. リストのUpdate()
4. リストのPhysicsUpdate()
5. リストのLateUpdate()
6. シーン遷移の処理
--------------------------*/
void SceneManager::ManagerUpdate() {

#ifdef DEBUG
	if (ImGuiManager::Instance()->GetPause()) return;
#endif

	for (auto& obj : InitList) {
		EntityObject.emplace_back(obj);
	}
	for (auto& obj : InitList) {
		obj.obj->Start();
	}
	InitList.clear();

	for (auto& obj : EntityObject) {
		if (obj.obj->GetExistState() && obj.obj != nullptr)
			obj.obj->Update();
	}
	for (auto& obj : EntityObject) {
		if (obj.obj->GetExistState() && obj.obj != nullptr)
			obj.obj->PhysicsUpdate();
	}
	for (auto& obj : EntityObject) {
		if (obj.obj->GetExistState() && obj.obj != nullptr)
			obj.obj->LateUpdate();
	}
	RemoveEntityObject();

	if (Move) {
		for (Scene* scene : SceneList)
		{
			if (scene->GetSceneName() == next) {

				UnActiveScene();
				ThisScene = scene;
				scene->InitScene();
				Move = false;
				break;
			}
		}
	}
}

/*------------------------
リストの描画処理
--------------------------*/
void SceneManager::ManagerDraw() {


	DirectX::XMFLOAT4X4 mtx;
	mtx = Component::Camera::GetMainCamera()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	mtx = Component::Camera::GetMainCamera()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	Transform* cam = Component::Camera::GetMainCamera()->GetOwner()->transform;

	/*---------------------------------------------
	不透明オブジェクトのソート（アクティブか）
----------------------------------------------*/
	std::thread th1([&]()
	{
		for (int i = 0; i < mDFOArray.size(); i++) {
			for (int j = 1; j < mDFOArray.size() - i; j++) {
				if (mDFOArray[j]->GetExistState()) {
					std::swap(mDFOArray[j], mDFOArray[j - 1]);
				}
			}
		}
	});

	/*-----------------------------------------
		半透明描画のソート（アクティブか・Z）
	-----------------------------------------*/
	std::thread th2([&]()
	{
		Vector3 norm = cam->forward() * -1.0f;

		for (auto& dfa : mDFTArray)
		{
			Vector3 dist = cam->position - dfa.first->transform->GetWorldPosition();
			float f = norm.Dot(dist);
			dfa.second = f;
		}

		for (int i = 0; i < mDFTArray.size(); i++)
		{
			for (int j = 1; j < mDFTArray.size() - i; j++)
			{
				if (mDFTArray[j].second > mDFTArray[j - 1].second
					&& mDFTArray[j].first->GetExistState())
				{
					std::swap(mDFTArray[j], mDFTArray[j - 1]);
				}
			}
		}
	});

	/*-----------------------------------
	UIのZソート（アクティブか・Z）
	------------------------------------*/
	std::thread th3([&]()
	{
		for (int i = 0; i < mDFUArray.size(); i++)
		{
			for (int j = 1; j < mDFUArray.size() - i; j++)
			{
				if (mDFUArray[j]->transform->position.z > mDFUArray[j - 1]->transform->position.z
					&& mDFUArray[j]->GetExistState())
				{
					std::swap(mDFUArray[j], mDFUArray[j - 1]);
				}
			}
		}
	});

	/*------------------------------
		ソート終了待ち
	-------------------------------*/
	th1.join();
	th2.join();
	th3.join();

	/*------------------------------
		不透明オブジェクトの描画
	-------------------------------*/
	for (auto dFO : mDFOArray) {
		if(dFO->GetExistState()!=false)
		dFO->DrawObject();
	}

	/*------------------------------
		透過ありオブジェクトの描画
	-------------------------------*/
	for (auto dFT : mDFTArray) {
		if (dFT.first->GetExistState() != false)
		dFT.first->DrawObject();
	}

	// コンピュートの描画
	ComputeManager::Instance()->Draw();

	// ポストエフェクトの描画
	PostEffect::PostEffectManager::Instance()->ManagerDraw();
	
	// コンピュートのあと描画
	ComputeManager::Instance()->AfterDraw();

	/*------------------------------
	Zソートオフ
	-------------------------------*/
	CDirectXGraphics::GetInstance()->TurnOffZbuffer();

	/*------------------------------
	UIの描画
	-------------------------------*/

	for (auto dFA : mDFUArray) {
		dFA->DrawUI();
	}

	/*------------------------------
	文字列描画
	-------------------------------*/
	MyString::DrawMyString();

	/*------------------------------
	Zソートオン
	-------------------------------*/
	CDirectXGraphics::GetInstance()->TurnOnZBuffer();

}

bool SceneManager::	ScreenCheck(Vector3 _pos, XMMATRIX _viewmtx, XMMATRIX _promtx)
{

	// 行列作成
	float w = Application::Instance()->CLIENT_WIDTH / 2.0f;
	float h = Application::Instance()->CLIENT_HEIGHT / 2.0f;
	
	XMMATRIX viewport = {
	w , 0, 0, 0,
	0, -h , 0, 0,
	0, 0, 1, 0,
	w , h , 0, 1
	};

	XMVECTOR World_Pos = XMVectorSet(_pos.x, _pos.y, _pos.z, 0);
	
	// ビュー変換とプロジェクション変換
	World_Pos = XMVector3Transform(World_Pos, _viewmtx);
	World_Pos = XMVector3Transform(World_Pos, _promtx);
	// float3に変換
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, World_Pos);

	// 初期化
	XMVECTOR view_vec = XMVECTOR{ 0, 0, 0 };
	view_vec = XMVectorSet(temp.x/ temp.z, temp.y/ temp.z, -1.0f, 1.0f);

	XMVector3Transform(view_vec, viewport);
	
	XMFLOAT3* c = new XMFLOAT3;
	XMStoreFloat3(c, view_vec);


	if (c->x > -1 && c->y > -1 &&
		c->x < 1 && c->y < 1)
	{
		return true;
	}

	return false;
}

