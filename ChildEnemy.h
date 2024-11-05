#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// �˂�����2�Ɋւ���N���X
/// </summary>

class Fbx;
class Player;

class ChildEnemy :
    public GameObject
{
    int hModel_;
    Player* pl;
public:
    ChildEnemy(GameObject* parent);
    ~ChildEnemy();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void OnCollision(GameObject* pTarget) override;
};

