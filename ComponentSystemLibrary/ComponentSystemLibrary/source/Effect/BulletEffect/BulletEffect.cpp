#include "BulletEffect.h"

using namespace Component;

BulletEffect::BulletEffect(GameObject *_owner) : ComponentBase(_owner)
{
	owner->SetName("BulletEffect");

	ren = owner->AddComponent<Component::QuadRender>("assets/Image/Effect/bullet.png");
	ren->SetRenderMode(Quad::RenderMode::BILLBOARD);
	ren->SetHeight(5);
	ren->SetWidth(5);


	// ���j�A�j���[�V����
	anim = owner->AddComponent<Component::Animation>();
	anim->fileread("Anim/Bullet/bullet.csv");
	anim->SetSpeed(1);
	anim->SetAnim(anim->GetAnim("bullet"));

	owner->transform->position = Vector3(-10, 0, 10);
}

BulletEffect::~BulletEffect()
{

}

void BulletEffect::Update()
{
	// �I���t���O�ɂ���\���ɂ���
	if (anim->GetEndFlg())
	{
		owner->SetExistState(false);
	}
}



