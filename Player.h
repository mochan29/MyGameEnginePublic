#pragma once
#include "Engine/GameObject.h"

/// <summary>
/// �v���C���[�Ɋւ���N���X
/// </summary>

class Fbx; //�O���錾
class Enemy;

namespace
{
    const int NUM = 14; //�^�C�}�[���f���p
}

class Player:
    public GameObject
{
    int hModel_; //���f���ԍ�
    int odeNum_; //���ł�̐�
    int point_; //�|�C���g
    int num_[NUM]; //���f���̔z��
    Transform timeTr_[3]; //�^�C�}�[�O����
    int timer_; //�^�C�}�[�̖{��
    float oneFlameTimer_; //�^�C�}�[�p�^�C�}�[
    void InitTimer(); //�^�C�}�[������
    Transform points_[3];
    float drawTimer_;//+22
    bool isPlus_; //+-���Ԃ����Ƃ��̏㏑���p
    float clearTimer_; //�N���A��ʂɑJ�ڂ���܂ł̃^�C�}�[
    bool cleared_; //�N���A�������ǂ���
    float overTimer_; //�I�[�o�[��ʂɑJ�ڂ���܂ł̃^�C�}�[
    bool overed_; //�I�[�o�[�������ǂ���
public:
    Player(GameObject* parent);
    ~Player();
    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;
    void MinusOdeNum() { odeNum_--; } //���ł�̐�����
    void PlusTimer(); //�^�C�}�[�𑝂₷
    void MinusTimer(); //�^�C�}�[�����炷
    void DrawPlus(); //+22
    void DrawMinus(); //-11
};

