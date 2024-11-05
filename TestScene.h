#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// TestSceneを管理するクラス
/// </summary>

class Sprite;

class TestScene :
    public GameObject
{
    Sprite* sp;
    float noRendaTimer_; //連打防止タイマー
public:
    TestScene(GameObject* parent);
    ~TestScene();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

