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
�I������
--------------------------*/
void SceneManager::UninitScene() {
	for (auto& scene : *SceneManager::GetSceneList()) {
		delete scene;
	}
	SceneManager::GetSceneList()->clear();
}

/*------------------------
�w�肵���V�[���̃I�u�W�F�N�g�𐶐�����
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
���݂̃I�u�W�F�N�g���폜����
�ÓI�ȃI�u�W�F�N�g�͎c��
--------------------------*/
void SceneManager::UnActiveScene() {

	for (auto& obj : EntityObject) {
		if (!obj.Static)
			delete obj.obj;
	}

	// �폜�������̃��X�g���œK��
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
�I�u�W�F�N�g�̒ǉ�
--------------------------*/
void SceneManager::AddObject(GameObject* obj, bool staticType) {
	InitList.emplace_back(Object{ obj, staticType });
}

/*------------------------
�I�u�W�F�N�g�̒ǉ�(���X�g)
--------------------------*/
void SceneManager::AddList(std::vector<GameObject*> *list) {
	for (GameObject* obj : *list) {
		InitList.emplace_back(Object{ obj,false });
	}
}

/*------------------------
�w�肵���I�u�W�F�N�g���폜����
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
�w�肵���I�u�W�F�N�g���폜����
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
�I�u�W�F�N�g�폜��̃��T�C�Y
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
����̃I�u�W�F�N�g��T��
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
����̃I�u�W�F�N�g��S�ĒT��
--------------------------*/
void SceneManager::GameObjectFindNameList(std::string str, std::vector<GameObject*> &list) {
	for (auto hoge : EntityObject) {
		if (hoge.obj->GetName() == str) {
			list.emplace_back(hoge.obj);
		}
	}
}

/*------------------------
����̃I�u�W�F�N�g��T��
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
����̃I�u�W�F�N�g��S�ĒT��
--------------------------*/
void SceneManager::GameObjectFindTagList(std::string str, std::vector<GameObject*>& list) {
	for (auto hoge : EntityObject) {
		if (hoge.obj->GetTag() == str) {
			list.emplace_back(hoge.obj);
		}
	}
}

/*------------------------
�ł��߂��I�u�W�F�N�g��T��
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
���͈͂̃I�u�W�F�N�g��S�ĒT��
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
		�`�揈���̓o�^
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
		�`�揈���̍폜
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
Update����
--------------------------
1. ���������I�u�W�F�N�g�̃��X�g�ǉ�
2. ���������I�u�W�F�N�g�� Start()����
3. ���X�g��Update()
4. ���X�g��PhysicsUpdate()
5. ���X�g��LateUpdate()
6. �V�[���J�ڂ̏���
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
���X�g�̕`�揈��
--------------------------*/
void SceneManager::ManagerDraw() {


	DirectX::XMFLOAT4X4 mtx;
	mtx = Component::Camera::GetMainCamera()->GetProjectionMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::PROJECTION, mtx);

	mtx = Component::Camera::GetMainCamera()->GetCameraMatrix();
	DX11SetTransform::GetInstance()->SetTransform(DX11SetTransform::TYPE::VIEW, mtx);

	Transform* cam = Component::Camera::GetMainCamera()->GetOwner()->transform;

	/*---------------------------------------------
	�s�����I�u�W�F�N�g�̃\�[�g�i�A�N�e�B�u���j
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
		�������`��̃\�[�g�i�A�N�e�B�u���EZ�j
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
	UI��Z�\�[�g�i�A�N�e�B�u���EZ�j
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
		�\�[�g�I���҂�
	-------------------------------*/
	th1.join();
	th2.join();
	th3.join();

	/*------------------------------
		�s�����I�u�W�F�N�g�̕`��
	-------------------------------*/
	for (auto dFO : mDFOArray) {
		if(dFO->GetExistState()!=false)
		dFO->DrawObject();
	}

	/*------------------------------
		���߂���I�u�W�F�N�g�̕`��
	-------------------------------*/
	for (auto dFT : mDFTArray) {
		if (dFT.first->GetExistState() != false)
		dFT.first->DrawObject();
	}

	// �R���s���[�g�̕`��
	ComputeManager::Instance()->Draw();

	// �|�X�g�G�t�F�N�g�̕`��
	PostEffect::PostEffectManager::Instance()->ManagerDraw();
	
	// �R���s���[�g�̂��ƕ`��
	ComputeManager::Instance()->AfterDraw();

	/*------------------------------
	Z�\�[�g�I�t
	-------------------------------*/
	CDirectXGraphics::GetInstance()->TurnOffZbuffer();

	/*------------------------------
	UI�̕`��
	-------------------------------*/

	for (auto dFA : mDFUArray) {
		dFA->DrawUI();
	}

	/*------------------------------
	������`��
	-------------------------------*/
	MyString::DrawMyString();

	/*------------------------------
	Z�\�[�g�I��
	-------------------------------*/
	CDirectXGraphics::GetInstance()->TurnOnZBuffer();

}

bool SceneManager::	ScreenCheck(Vector3 _pos, XMMATRIX _viewmtx, XMMATRIX _promtx)
{

	// �s��쐬
	float w = Application::Instance()->CLIENT_WIDTH / 2.0f;
	float h = Application::Instance()->CLIENT_HEIGHT / 2.0f;
	
	XMMATRIX viewport = {
	w , 0, 0, 0,
	0, -h , 0, 0,
	0, 0, 1, 0,
	w , h , 0, 1
	};

	XMVECTOR World_Pos = XMVectorSet(_pos.x, _pos.y, _pos.z, 0);
	
	// �r���[�ϊ��ƃv���W�F�N�V�����ϊ�
	World_Pos = XMVector3Transform(World_Pos, _viewmtx);
	World_Pos = XMVector3Transform(World_Pos, _promtx);
	// float3�ɕϊ�
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, World_Pos);

	// ������
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

