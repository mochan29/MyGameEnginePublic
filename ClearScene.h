#pragma once
#include "Engine/GameObject.h"

class Sprite;

class ClearScene:
    public GameObject
{
    Sprite* sp;
public:
    ClearScene(GameObject* parent);
    ~ClearScene();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

