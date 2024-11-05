#include "ChildOden.h"
#include "Model.h"
#include "Enemy.h"
#include "SphereCollider.h"
#include "Engine/Input.h"
#include "SceneManager.h"
#include "Debug.h"
#include "Player.h"

ChildOden::ChildOden(GameObject* parent)
	:GameObject(parent, "ChildOden")
{
	hModel_ = -1;
	pl = (Player*)FindObject("Player");
	ene = (Enemy*)FindObject("Enemy");
}

ChildOden::~ChildOden()
{
}

void ChildOden::Initialize()
{
	hModel_ = Model::Load("Assets//Models//odenn.fbx");
	SphereCollider* col = new SphereCollider(0.5);
	this->AddCollider(col);
}

void ChildOden::Update()
{
	transform_.position_.y += 0.1;
	
	
	if (transform_.position_.y > 8.0f)
	{
		KillMe();
		pl->MinusOdeNum();
	}
	
}

void ChildOden::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void ChildOden::Release()
{

}

void ChildOden::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Enemy")
	{
		KillMe();
		pl->MinusOdeNum();
		pl->PlusTimer();
	}	
}
