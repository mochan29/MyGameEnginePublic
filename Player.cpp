#include "Player.h"
#include "ChildOden.h"
#include "Engine/Input.h"
#include "Model.h"
#include "Debug.h"
#include "Engine/Sprite.h"
#include <string>
#include "SphereCollider.h"
#include "SceneManager.h"
#include "Enemy.h"

namespace
{
	const int ODENNUM = 2; //同時に存在できるおでんの個数
	const int TIMER =60; //全体のタイマー
	const float FLAME_TIMER = 60; //タイマー専用タイマー
	const int CLEAR_TIME = 222; //
}

Player::Player(GameObject* parent)
	:GameObject(parent, "Player")
{
	hModel_ = -1;
	odeNum_ = 0;
	point_ = 0;
	timer_ = TIMER;
	oneFlameTimer_= FLAME_TIMER;
	for (int i = 0; i < NUM; i++)
	{
		num_[i] = -1;
	}
	drawTimer_ = 0;
	isPlus_ = false;
	clearTimer_ = FLAME_TIMER;
	cleared_ = false;
	overTimer_ = FLAME_TIMER;
	overed_ = false;
}

Player::~Player()
{
}

void Player::Initialize()
{
	hModel_ = Model::Load("Assets/Models/odenn.fbx");
	SetPosition(0.0f, -5.0f, 1.0f);
	for (int i = 0; i < NUM; i++)
	{
		num_[i] = Model::Load("Assets/Models/" + std::to_string(i) + ".fbx");
	}

	InitTimer();

	SphereCollider* col = new SphereCollider(1.0);
	this->AddCollider(col);
}

void Player::Update()
{
	if (!cleared_)
	{
		//スペース押したらこおでんだす
		if (Input::IsKeyDown(DIK_SPACE))
		{
			if (odeNum_ < ODENNUM)
			{
				GameObject* p = Instantiate<ChildOden>(this);
				p->SetPosition(this->transform_.position_);
				p->SetScale(0.5f, 0.5f, 0.5f);
				odeNum_++;
			}
		}
		if (transform_.position_.x < 6.5)
		{
			if (Input::IsKey(DIK_D) || Input::IsKey(DIK_RIGHT))
			{
				transform_.position_.x += 0.1;
			}
		}
		if (transform_.position_.x > -6.5)
		{
			if (Input::IsKey(DIK_A) || Input::IsKey(DIK_LEFT))
			{
				transform_.position_.x -= 0.1;
			}
		}
		if (transform_.position_.y < -4.0)
		{
			if (Input::IsKey(DIK_W) || Input::IsKey(DIK_UP))
			{
				transform_.position_.y += 0.1f;
			}
		}
		if (transform_.position_.y > -7.5)
		{
			if (Input::IsKey(DIK_S) || Input::IsKey(DIK_DOWN))
			{
				transform_.position_.y -= 0.1f;
			}
		}

		//タイマー
		if (oneFlameTimer_ < 0)
		{
			if (timer_ > 0)
			{
				timer_--;
			}
			else
			{
				timer_ = 0;
			}
			oneFlameTimer_ = FLAME_TIMER;
		}
		else
		{
			oneFlameTimer_--;
		}
		if (drawTimer_ > 0)
		{
			drawTimer_--;
		}
	}
}

void Player::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	//タイマー
	int iti = 0;
	int ni = 0;
	int san = 0;
	if (timer_ > 0)
	{
		san = timer_ / 100;
		ni = (timer_ / 10) % 10;
		iti = timer_ % 10;
		if (timer_ > CLEAR_TIME)
		{
			san = 2;
			ni = 2;
			iti = 2;
			clearTimer_--;
			if (clearTimer_ < 0)
			{
				SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
				pSceneManager->ChangeScene(SCENE_ID_CLEAR);
			}
		}
	}
	else
	{
		overTimer_--;
		if (overTimer_ < 0)
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_OVER);
		}
	}

	//三桁目
	Model::SetTransform(num_[san], timeTr_[0]);
	Model::Draw(num_[san]);

	//二桁目
	Model::SetTransform(num_[ni], timeTr_[1]);
	Model::Draw(num_[ni]);

	//一桁目
	Model::SetTransform(num_[iti], timeTr_[2]);
	Model::Draw(num_[iti]);

	//+-片方のみ場にいるように
	if (isPlus_)
	{
		DrawPlus();
	}
	else
	{
		DrawMinus();
	}
}

void Player::InitTimer()
{
	//タイマー本体
	for (int i = 0; i < 3; i++)
	{
		timeTr_[i].position_.x = -4.8f + 0.4f * i;
		timeTr_[i].position_.y = 3.5f;
		timeTr_[i].position_.z = 0.0f;
		timeTr_[i].scale_.x = 0.7f;
		timeTr_[i].scale_.y = 0.7f;
		timeTr_[i].scale_.z = 0.7f;
		timeTr_[i].rotate_.x = 12.0f;
		timeTr_[i].rotate_.y = 0.0f;
		timeTr_[i].rotate_.z = 0.0f;
	}

	//+-
	for (int i = 0; i < 3; i++)
	{
		
		points_[i].position_.x = -4.8f + 0.4f * 3 + 0.2f * i;
		points_[i].position_.y = 3.5f;
		points_[i].position_.z = 0.0f;
		points_[i].scale_.x = 0.5f;
		points_[i].scale_.y = 0.5f;
		points_[i].scale_.z = 0.5f;
		points_[i].rotate_.x = 12.0f;
		points_[i].rotate_.y = 0.0f;
		points_[i].rotate_.z = 0.0f;
	}
}

void Player::Release()
{

}

void Player::PlusTimer()
{
	timer_ += 22;
	isPlus_ = true;
	drawTimer_ = TIMER;
}

void Player::MinusTimer()
{
	timer_ -= 11;
	isPlus_ = false;
	drawTimer_ = TIMER;
}

void Player::DrawPlus()
{
	//+22
	if (drawTimer_ > 0)
	{
		Model::SetTransform(num_[10], points_[0]);
		Model::Draw(num_[10]);
		Model::SetTransform(num_[12], points_[1]);
		Model::Draw(num_[12]);
		Model::SetTransform(num_[12], points_[2]);
		Model::Draw(num_[12]);
	}
}

void Player::DrawMinus()
{
	//-11
	if (drawTimer_ > 0)
	{
		Model::SetTransform(num_[13], points_[0]);
		Model::Draw(num_[13]);
		Model::SetTransform(num_[11], points_[1]);
		Model::Draw(num_[11]);
		Model::SetTransform(num_[11], points_[2]);
		Model::Draw(num_[11]);
	}
}