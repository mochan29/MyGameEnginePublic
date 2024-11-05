#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// TestScene���Ǘ�����N���X
/// </summary>

class Sprite;

class TestScene :
    public GameObject
{
    Sprite* sp;
    float noRendaTimer_; //�A�Ŗh�~�^�C�}�[
public:
    TestScene(GameObject* parent);
    ~TestScene();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

