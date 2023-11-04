#pragma once
#include "Collision.h"
#include "DxLib.h"
#include "scrollManager.h"

class Player
{
public:
    //�摜���
    static const int animPatternNum = 8;           // �A�j���[�V�����̃p�^�[����
    static const int animTypeNum = 1;              // �A�j���[�V�����̎��
    
    //�W�����v���
    const float animationFPS            = 8.0f  ;   // �b��8�R�}�̃A�j���[�V����
    const float moveSpeed               = 300.0f;   // �ړ����x
    const float maxMoveSpeed            = 350.0f;   // �ő剡�ړ����x
    const float moveAccell              = 2.5f;     // �ړ������x
    const float frictionRatio           = 0.01f;     // ���C�W��
    const float brakeRatio              = 0.1f;     // �i�s�����ɑ΂��ē��͋t�����ɓ��ꂽ�Ƃ��̃u���[�L�W��
    const float inAirMoveAccelRatio     = 3.0f;     // �󒆉��ړ�������
    const float jumpInitialVelocity     = 15.0f  ;  // �W�����v�����x
    const float jumpUpSpeed             = 10.0f ;   // �㏸�X�s�[�h
    const float hitHeadBreakRatio       = -0.1f ;   // ����Փˎ��̌�����

    const int imageSizeX = 50;                      // �摜���T�C�Y
    const int imageSizeY = 50;                      // �摜�c�T�C�Y
    const int hitSizeX   = 50;                      // �����蔻��X�T�C�Y
    const int hitSizeY   = 50;                      // �����蔻��Y�T�C�Y

    const float gravity                 = 25.0f ;   // �d��
    const float maxFallSpeed            = 200.0f;   // �ő嗎���X�s�[�h
    const int colliderOffset            = 10    ;   // �R���C�_�[���炵��
    const float jumpButtonAcceptTime    = 0.95f ;   // �W�����v�{�^����t����

    //���W���
    float px;                                       // �v���C���[X���W
    float py;                                       // �v���C���[Y���W
    float VX;                                       // �v���C���[X�ړ����x
    float VY;                                       // �v���C���[Y�ړ����x
    float playerEndposition;                        // �v���C���[�̏I�[�ʒu
    float animTimer;                                // �A�j���[�V�����^�C�}�[
    int animNowType;                                // ���݂̃A�j�����
    int animNowPattern;                             // ���݂̃A�j���p�^�[��
    int animNowIndex;                               // �A�j���[�V�����摜�z��̓Y���� 0�`10�܂ł̐��l
    int drawOffsetX;                                // ���������炵��
    int drawOffsetY;                                // �c�������炵��

    bool jumpFlg;                                   // �W�����v�t���O
    bool onGround;                                  // �n�ʂɂ����Ă��邩
    bool hitHead;                                   // �����Ԃ�������
    float jumpTimer;

    int playerImg[animPatternNum * animTypeNum]; // �v���C���[�̉摜�n���h��
    int Normal;

    sHitRect playerHit;           // �v���C���[�����蔻��
    sHitRect playerFootCollider;  // �ڒn�R���C�_�[
    sHitRect playerHeadCollider;  // ����R���C�_�[

    bool prevJumpButton;          // �O�̃t���[���ŃW�����v�{�^��������Ă������H
    bool isJumpPush;              // �W�����v�{�^�������ꂽ�u�Ԃ��H

    bool inputMove  (float deltaTime);

    /// <summary>
    /// �摜�ǂݍ���
    /// </summary>
    void Load();

    /// <summary>
    /// �v���C���[������
    /// </summary>
    void Init       (Collision& collision);
   
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
    //�v���C���[�`��
    //-------------------------------------
    void Draw(int scrollOffsetX, int scrollOffsetY, Scroll& scroll)
    {
        scrollOffsetX = scroll.scrollGetDrawOffsetX();

        DrawGraph(static_cast<int>(px) + drawOffsetX - scrollOffsetX,
            static_cast<int>(py) + drawOffsetY - scrollOffsetY, playerImg[animNowIndex], TRUE);
    }

    /// <summary>
    /// �v���C���[��n��
    /// </summary>
    void Finalize   ();
   
    /// <summary>
    /// �����蔻���`����v���C���[�ʒu���C������
    /// </summary>
    /// <param name="hitRect">�����蔻���`</param>
    void playerfixColPosition   (sHitRect& hitRect);

    /// <summary>
    /// �v���C���[�̓����蔻���`��Ԃ�
    /// </summary>
    /// <returns>�v���C���[�̓����蔻���`</returns>
    sHitRect getPlayerHitRect();

    /// <summary>
    /// �n�ʂɗ����Ă��邩�H
    /// </summary>
    /// <param name="groundFlg">�����̓����蔻�茋��</param>
    void playerSetGroundFlg     (bool groundFlg);
    
    /// <summary>
    /// �v���C���[�̑����R���C�_�[�̃Q�b�g
    /// </summary>
    /// <returns>�����R���C�_�[�̋�`���</returns>
    sHitRect playerGetGroundCollider();

    /// <summary>
    /// ����ǂɂԂ�����
    /// </summary>
    /// <param name="headHitFlg">���̓����蔻�茋��</param>
    void playerSetHeadHitFlg(bool headHitFlg);

    /// <summary>
    /// �v���C���[�̓���R���C�_�[�̃Q�b�g
    /// </summary>
    /// <returns>����R���C�_�[�̋�`���</returns>
    sHitRect playerGetHeadCollider();
};