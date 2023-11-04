#pragma once
#include "Collision.h"
#include "DxLib.h"
#include "scrollManager.h"

class Enemy
{
public:
    //�摜���
    int shotImg;
    float shotX;
    float shotY;
    const float shotSpeed = 10.0f;

    int counter;


    static const int animPatternNum = 8;           // �A�j���[�V�����̃p�^�[����
    static const int animTypeNum = 1;              // �A�j���[�V�����̎��

    const float animationFPS = 8.0f;               // �b��8�R�}�̃A�j���[�V����

    const int imageSizeX  = 100;                     // �摜���T�C�Y
    const int imageSizeY  = 80;                     // �摜�c�T�C�Y
    const int hitSizeX    = 100;                     // �����蔻��X�T�C�Y
    const int hitSizeY    = 80;                     // �����蔻��Y�T�C�Y
    const float moveSpeed = 8.0f;                 // �G�ړ����x   

    float ex;                                       // �G�l�~�[X���W
    float ey;                                       // �G�l�~�[Y���W
    float EX;                                       // �G�l�~�[X�ړ����x
    float EY;                                       // �G�l�~�[Y�ړ����x
    float enemyEndposition;                         // �G�l�~�[�̏I�[�ʒu
    float animTimer;                                // �A�j���[�V�����^�C�}�[
    int   animNowType;                              // ���݂̃A�j�����
    int   animNowPattern;                           // ���݂̃A�j���p�^�[��
    int   animNowIndex;                             // �A�j���[�V�����摜�z��̓Y���� 0�`10�܂ł̐��l
    int   drawOffsetX;                              // ���������炵��
    int   drawOffsetY;                              // �c�������炵��

    bool direction; //true�F������E�Afalse�F�E���獶

    int enemyImg[animPatternNum * animTypeNum];     // �v���C���[�̉摜�n���h��
    int Normal;

    sHitRect enemyHit;                              // �v���C���[�����蔻��

    /// <summary>
    /// �摜�ǂݍ���
    /// </summary>
    void Load();

    /// <summary>
    /// �v���C���[������
    /// </summary>
    void Init(Collision& collision);

    /// <summary>
    /// �v���C���[�X�V
    /// </summary>
    /// <param name="deltaTime">1�t���[�����̏�������</param>
    /// <param name="playAreaRect">�v���C��ʂ̃��[���h���W��`�BscreenManager��getPlayArea()�֐��̖߂�l���g�p����</param>
    void Update(float deltaTime, Collision& collision, sHitRect playAreaRect);


    /// <summary>
    /// �v���C���[�`��
    /// </summary>
    /// <param name="scrollOffsetX">�X�N���[���ړ���X</param>
    /// <param name="scrollOffsetY">�X�N���[���ړ���Y</param>
    //-------------------------------------
    //�G�l�~�[�`��
    //-------------------------------------
    void Draw(int scrollOffsetX, int scrollOffsetY, Scroll& scroll)
    {
        scrollOffsetX = scroll.scrollGetDrawOffsetX();

        DrawGraph(static_cast<int>(ex) + drawOffsetX - scrollOffsetX,
                     static_cast<int>(ey) + drawOffsetY - scrollOffsetY, enemyImg[animNowIndex], TRUE);
    }

    void ShotDraw(int scrollOffsetX, int scrollOffsetY, Scroll& scroll)
    {
        scrollOffsetX = scroll.scrollGetDrawOffsetX();

        DrawGraph(static_cast<int>(shotX) + drawOffsetX - scrollOffsetX,
                    static_cast<int>(shotY) + drawOffsetY - scrollOffsetY, shotImg, TRUE);
    }

    /// <summary>
    /// �v���C���[��n��
    /// </summary>
    void Finalize();




};