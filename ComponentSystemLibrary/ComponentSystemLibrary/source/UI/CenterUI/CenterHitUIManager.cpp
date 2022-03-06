#include "CenterHitUIManager.h"
using namespace Component;

CenterHitUIManager::CenterHitUIManager(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("CenterHitUIManager");

    GameObject* m_centerhit = new GameObject();
    m_centerhit->AddComponent<Component::CenterHitUI>();
    SceneManager::Instance()->AddObject(m_centerhit, false);
    m_centerhit->GetComponent<Component::CenterHitUI>()->SetState(Component::CenterHitUI::State::DOWN_LEFT);
    m_centerhit->SetExistState(false);
    m_centerList.emplace_back(m_centerhit);

    m_centerhit = new GameObject();
    m_centerhit->AddComponent<Component::CenterHitUI>();
    SceneManager::Instance()->AddObject(m_centerhit, false);
    m_centerhit->GetComponent<Component::CenterHitUI>()->SetState(Component::CenterHitUI::State::DOWN_RIGHT);
    m_centerhit->SetExistState(false);
    m_centerList.emplace_back(m_centerhit);

    m_centerhit = new GameObject();
    m_centerhit->AddComponent<Component::CenterHitUI>();
    SceneManager::Instance()->AddObject(m_centerhit, false);
    m_centerhit->GetComponent<Component::CenterHitUI>()->SetState(Component::CenterHitUI::State::UP_LEFT);
    m_centerhit->SetExistState(false);
    m_centerList.emplace_back(m_centerhit);

    m_centerhit = new GameObject();
    m_centerhit->AddComponent<Component::CenterHitUI>();
    SceneManager::Instance()->AddObject(m_centerhit, false);
    m_centerhit->GetComponent<Component::CenterHitUI>()->SetState(Component::CenterHitUI::State::UP_RIGHT);
    m_centerhit->SetExistState(false);
    m_centerList.emplace_back(m_centerhit);
}

CenterHitUIManager::~CenterHitUIManager()
{

}

void CenterHitUIManager::Start()
{

}

void CenterHitUIManager::Update()
{
    // ’†S‚Ì‰Šú‰»
    for (auto list : m_centerList)
    {
        list->SetExistState(true);
    }

    owner->SetExistState(false);
}
