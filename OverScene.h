#pragma once
#include "Engine/GameObject.h"

class Sprite;

class OverScene:
    public GameObject
{
    Sprite* sp;
public:
    OverScene(GameObject* parent);
    ~OverScene();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
};

