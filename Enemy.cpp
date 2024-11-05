#include "Enemy.h"
#include "Player.h"
#include "Model.h"
#include "SphereCollider.h"
#include "ChildEnemy.h"
#include "Debug.h"

namespace
{
	float KONEKO_TIMER = 300.0f;
}

Enemy::Enemy(GameObject* parent)
	:GameObject(parent, "Enemy")
{
	hModel_ = -1;
	konekoTimer_ = KONEKO_TIMER;
	srand((unsigned int)time(nullptr));
	speed_ = 1.0f;
	
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	hModel_ = Model::Load("Assets/Models/cat.fbx");
	SetPosition(0.0f, 2.0f, 1.0f);
	transform_.rotate_.y = 180.0f;
	transform_.scale_.x = 2.0;
	transform_.scale_.y = 2.0;
	transform_.scale_.z = 2.0;
	SphereCollider* col = new SphereCollider(0.5f);
	this->AddCollider(col);
}

void Enemy::Update()
{
	transform_.rotate_.y += 2;
	static float t;
	t+=1.0f;

	//ねこの左右スピード

	float time =t / (10.0f * 10.0f)- 2.0f;
	transform_.position_.x = 4.0 * sin(time); 

	float r = rand() % 100;
	r = 1.0 + 0.02 * r;
	//定期的にこねこだす
	if (konekoTimer_ > 0)
	{
		konekoTimer_ -= r;
	}
	else
	{
		GameObject* e = Instantiate<ChildEnemy>(this);
		e->SetPosition(this->transform_.position_);
		float a = float(rand() %200); //大きさは100パーセントから200パーセントくらいまで
		if (a < 100)
		{
			a += 100;
		}
		e->SetScale(0.7f*a/100,0.7f*a / 100,0.7f*a / 100);
		konekoTimer_ =KONEKO_TIMER;
	}
	
}

void Enemy::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Enemy::Release()
{
}