#pragma once
#include "Engine/GameObject.h"

class Enemy :
    public GameObject
{
    int hModel_; //モデル番号
	int konekoTimer_; //こねこのでるたいまー
	float speed_; //ねこのはやさ
public:
    Enemy(GameObject* parent);
    ~Enemy();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
	XMFLOAT3 GetPosition()
	{
		return transform_.position_;
	}

	XMFLOAT3 GetRotate()
	{
		return transform_.rotate_;
	}

	XMFLOAT3 GetScale()
	{
		return transform_.scale_;
	}

	void SetNekoSpeed(float &s) { speed_ = s;}
 
};

