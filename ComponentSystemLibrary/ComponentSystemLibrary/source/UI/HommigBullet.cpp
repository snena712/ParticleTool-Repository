#include ""

HommingBullet::HommingBullet()
{
	name = "HomingBullet";

	tra = this->transform;

	// モデルの設定
	model = this->AddComponent<Component::ModelComponent>(
		"assets/f1/f1.x",			// モデルデータ			// カレントファイルからの読み込み
		"shader/vs.hlsl",			// 頂点シェーダー
		"shader/ps.hlsl",			// ピクセルシェーダー
		"assets/f1/");				// テクスチャの存在フォルダ


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
	//保存された
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
		// ターゲット消す
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