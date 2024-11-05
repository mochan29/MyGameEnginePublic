#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// プレイヤーに関するクラス
/// </summary>

class Fbx; //前方宣言
class Enemy;

namespace
{
    const int NUM = 14; //タイマーモデル用
}

class Player:
    public GameObject
{
    int hModel_; //モデル番号
    int odeNum_; //おでんの数
    int point_; //ポイント
    int num_[NUM]; //モデルの配列
    Transform timeTr_[3]; //タイマー三桁分
    int timer_; //タイマーの本体
    float oneFlameTimer_; //タイマー用タイマー
    void InitTimer(); //タイマー初期化
    Transform points_[3];
    float drawTimer_;//+22
    bool isPlus_; //+-かぶったときの上書き用
    float clearTimer_; //クリア画面に遷移するまでのタイマー
    bool cleared_; //クリアしたかどうか
    float overTimer_; //オーバー画面に遷移するまでのタイマー
    bool overed_; //オーバーしたかどうか
public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void MinusOdeNum() { odeNum_--; } //おでんの数制御
    void PlusTimer(); //タイマーを増やす
    void MinusTimer(); //タイマーを減らす
    void DrawPlus(); //+22
    void DrawMinus(); //-11
};

