#include "TestComputeObj.h"
#include "../../DX11System\DX11Settransform.h"
#include "../MainCamera.h"
#include "../../Component/SceneManager.h"
#include <random>


using namespace Component;

TestComputeObj::TestComputeObj(GameObject* _owner) : ComponentBase(_owner)
{
	owner->SetName("player");
	comCompo = 
		owner->AddComponent<Component::ComputeComponent>("assets/Image/Particle/ParticleCircle.png","assets/EfeFile/sample.EFE");
}

TestComputeObj::~TestComputeObj()
{}

// �R���s���[�g
void TestComputeObj::DrawObject()
{	
}

void TestComputeObj::Update()
{
	// ���ɏ������ރf�[�^�̏�������X�V������

	// �R�s�[��dispatch�@�폜�̏���������
	//ImGui::SetNextWindowSize(ImVec2(100, 100), ImGuiCond_Once);
	//ImGui::Begin("�I�u�W�F", 0);
	//ImGui::InputFloat3(u8"POS", (float*)&owner->transform->position);
	//
	//	//owner->transform->RotateLocalXaxis(1);
	//
	//
	//	//owner->transform->RotateLocalYaxis(1);
	//	//
	//	//
	//	owner->transform->RotateLocalZaxis(1);
	//

	//ImGui::End();
	//comCompo->SetSpawnPos(owner->transform->position);
}

