#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// Scene全体を管理するクラス
/// </summary>

enum SCENE_ID
{
    SCENE_ID_TEST=0, SCENE_ID_PLAY, SCENE_ID_CLEAR, SCENE_ID_OVER,SCENE_ID_MAX,
};

class SceneManager :
    public GameObject
{
    SCENE_ID currentSceneID_;
    SCENE_ID nextSceneID_;
public:
    SceneManager(GameObject* parent);
    ~SceneManager();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    /// <summary>
    /// シーンを切り替える
    /// </summary>
    /// <param name="nextScene">次のシーン先</param>
    void ChangeScene(SCENE_ID nextScene) { nextSceneID_ = nextScene; }
};

