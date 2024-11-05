#include "ChildEnemy.h"
#include "Model.h"
#include "Player.h"
#include "SphereCollider.h"
#include "Engine/Input.h"
#include "SceneManager.h"

ChildEnemy::ChildEnemy(GameObject* parent)
	:GameObject(parent, "ChildEnemy")
{
	hModel_ = -1;
	pl = (Player*)FindObject("Player");
}

ChildEnemy::~ChildEnemy()
{
}

void ChildEnemy::Initialize()
{
	hModel_ = Model::Load("Assets//Models//cat.fbx");
	SphereCollider* col = new SphereCollider(1.0);
	this->AddCollider(col);

	
}

void ChildEnemy::Update()
{
	transform_.position_.y -= 0.1;
	if (transform_.position_.y <-10.0f)
	{
		KillMe();
	}
	
}

void ChildEnemy::Draw()
{
	transform_.rotate_.y = 180;
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void ChildEnemy::Release()
{
}

void ChildEnemy::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Player")
	{
		KillMe();
		pl->MinusTimer();
	}
}
