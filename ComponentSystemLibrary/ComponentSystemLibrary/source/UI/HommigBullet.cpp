#include ""

HommingBullet::HommingBullet()
{
	name = "HomingBullet";

	tra = this->transform;

	// ���f���̐ݒ�
	model = this->AddComponent<Component::ModelComponent>(
		"assets/f1/f1.x",			// ���f���f�[�^			// �J�����g�t�@�C������̓ǂݍ���
		"shader/vs.hlsl",			// ���_�V�F�[�_�[
		"shader/ps.hlsl",			// �s�N�Z���V�F�[�_�[
		"assets/f1/");				// �e�N�X�`���̑��݃t�H���_


	this->AddComponent<Component::Rigidbody>();
	rb = this->GetComponent<Component::Rigidbody>();

	Component::SphereCollider* col = this->AddComponent<Component::SphereCollider>();
}

HommingBullet::~HommingBullet()
{

}

void HommingBullet::Start()
{

}

void HommingBullet::Update()
{
	GameObject::Update();

	MoveUpdate();
	//�ۑ����ꂽ
}

void HommingBullet::Draw()
{
	GameObject::Draw();
}

void HommingBullet::MoveUpdate()
{
	Vector3 f = tra->forward();
	Vector3 d = Vector3(0, 0, 100) - transform->position;

	if (f.Dot(d) < 0)
	{
		// �^�[�Q�b�g����
	}
	else {
		transform->LookSlerp(Vector3(0, 0, 100), 0.03f);
	}
	transform->position += tra->forward() * 5.0f;

}

void HommingBullet::Collision()
{
	for (auto& _rigidlist : *rb->GetCollisionObject())
	{
		if (_rigidlist.obj->GetName() == "")
			//_rigidlist.obj->transform->position.x = 1000;
			int a;
	}
}