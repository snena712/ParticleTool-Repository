#pragma once
#include <string>
#include <vector>

#include "ComponentBaseClass.h"
#include "Transform.h"
#include "../DX11System/Vector.h"
#include "../Application.h"

class Transform;
class ComponentBase;

//�I�u�W�F�N�g�^�C�v
enum class ObjectType
{
	Player = 0b00,
	Enemy = 0b01,
	Obstracle = 0b11,
};

//�Q�[���I�u�W�F�N�g�̊��N���X
class GameObject
{
protected:

	std::string		name;				// ���O
	std::string		tag;				// �^�O
	unsigned int	objectID;			// �I�u�W�F�N�gID�ԍ�
	bool			isExist;			// ������
	ObjectType		objectType;			// �I�u�W�F�N�g�^�C�v

	std::vector<ComponentBase*> componentList;

#ifdef DEBUG
	bool			debug = false;		// �f�o�b�O�̉�
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

	virtual void Update();					// �X�V����
	virtual void PhysicsUpdate();			// ��������
	virtual void LateUpdate();				// Update��ɍX�V

	virtual void DrawObject();
	virtual void DrawUI();

	//--------------------------------------------------
	//			�R���|�[�l���g�֌W
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
//			�R���|�[�l���g�֌W
//----------------------------------------------------

//�R���|�[�l���g��ǉ�����
//AddComponent<�ǉ��������R���|�[�l���g�̃N���X��>()�Ƃ����`�Ŏg��
template<class T, class... Args>
T* GameObject::AddComponent(Args... args)
{
	T* newComponent = new T(this, args...);//new�����̂�delete��Y�ꂸ�Ɂi�f�X�g���N�^�ł���Ă�j
	if (dynamic_cast<ComponentBase*>(newComponent) != nullptr)
	{
		componentList.emplace_back(newComponent);
	}

	return newComponent;
}

//�R���|�[�l���g���擾����
//GetComponent<�R���|�[�l���g�̃N���X��>()->�R���|�[�l���g�̃��\�b�h()�Ƃ����`�Ŏg��
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

	return nullptr;//���Y�R���|�[�l���g���Ȃ����nullptr��return
}

//�R���|�[�l���g���폜����B�g�����͏�L��ɏ�����B
template<class T>
void GameObject::RemoveComponent()
{
	for (unsigned int i = 0; i < componentList.size(); i++) //�폜�͂�������for�����₷��
	{
		T* toRemove = dynamic_cast<T*>(componentList[i]);
		if (toRemove != nullptr)
		{
			componentList.erase(componentList.begin() + i);		//�y�ɍ��Ƃ����Ȃ邪�A���X�g�̗v�f�����������erase�͏d���̂ŁA�ʂ̕��@���g���������ǂ��B
			componentList.shrink_to_fit();						//���X�g��capasity��size�̕s��v���C�ɂȂ�Ȃ炱������Ă������B�������ق�̏����d���Ȃ�B

			return;
		}
	}
}

