#include "EnemyPosFrame.h"

using namespace Component;

EnemyPosFrame::EnemyPosFrame(GameObject *_owner) :ComponentBase(_owner)
{
	owner->SetName("EnemyPosFrame");

	tra = owner->transform;

	ren = owner->AddComponent<Component::Quad2DRender>("assets/Image/UI/square.png");
	ren->SetAnchorType(AnchorType::LeftTop);
	ren->SetHeight(70);
	ren->SetWidth(70);

	owner->AddComponent<Component::StartUIColorComponent>();
	owner->GetComponent<Component::StartUIColorComponent>()->SetType(Component::StartUIColorComponent::RnderState::TWO);
}

EnemyPosFrame::~EnemyPosFrame()
{

}

void EnemyPosFrame::Start()
{
	m_camera = SceneManager::Instance()->GameObjectFindName("MainCamera");
	cameraCompo = m_camera->GetComponent<Component::Camera>();
}

void EnemyPosFrame::Update()
{
}

// isExist�̃t���O��false�ɂ���ƑS�Ă̏������͂���
void EnemyPosFrame::DrawObject()
{

}

// �r���[���W�ɕϊ�
XMVECTOR EnemyPosFrame::Screenpos(XMVECTOR World_Pos) {

	m_drawflg = false;

	// �r���[�ƃX�N���[���ϊ�
	XMMATRIX g_View = XMLoadFloat4x4(&cameraCompo->GetCameraMatrix());
	XMMATRIX g_Projection = XMLoadFloat4x4(&cameraCompo->GetProjectionMatrix());

	// �s��쐬
	float w = Application::Instance()->CLIENT_WIDTH / 2.0f;
	float h = Application::Instance()->CLIENT_HEIGHT / 2.0f;
	XMMATRIX viewport = {
	w, 0, 0, 0,
	0, -h, 0, 0,
	0, 0, 1, 0,
	w, h, 0, 1
	};

	// �r���[�ϊ��ƃv���W�F�N�V�����ϊ�
	World_Pos = XMVector3Transform(World_Pos, g_View);
	World_Pos = XMVector3Transform(World_Pos, g_Projection);

	// float3�ɕϊ�
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, World_Pos);

	// ������
	XMVECTOR view_vec = XMVECTOR{ 0, 0, 0 };

	// z�Ŋ�����-1~1�͈̔͂Ɏ��߂�

	XMFLOAT4X4 a;

	DX11MtxMultiply(a,  Component::Camera::GetMainCamera()->GetCameraMatrix(), Component::Camera::GetMainCamera()->GetProjectionMatrix());

	// ��ʓ����H
	if (IsInFrustum(Ene->transform->position, a))
	{
		owner->SetExistState(true);
		view_vec = XMVectorSet(temp.x / temp.z, temp.y / temp.z, -1.0f, 1.0f);
	}
	else {
		// false�ɂ��邱�Ƃŕ`��j�~�@�e����true�ɂ��Ă�����Ă���
		owner->SetExistState(false);
	}
	return XMVector3Transform(view_vec, viewport);
}


void EnemyPosFrame::CheckScreen()
{
	XMVECTOR a = XMVectorSet(Ene->transform->position.x, Ene->transform->position.y, Ene->transform->position.z, 0);
	XMVECTOR b = Screenpos(a);
	if (owner->GetExistState() == true)
	{
		XMFLOAT3* c = new XMFLOAT3;
		XMStoreFloat3(c, b);

		// �e���W�ɕϊ�

		owner->transform->position.x = c->x;
		owner->transform->position.y = -c->y;
		owner->transform->position.z = 3;
	}
}
