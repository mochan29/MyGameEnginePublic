#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// ���ł񂻂�2�Ɋւ���N���X
/// </summary>

class Fbx;
class Player;
class Enemy;

class ChildOden :
    public GameObject
{
    int hModel_;
    Player* pl;
    Enemy* ene;
public:
    ChildOden(GameObject* parent);
    ~ChildOden();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnCollision(GameObject* pTarget) override;
};

