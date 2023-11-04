#include "Player.h"

//-------------------------------------
//�摜�ǂݍ���
//-------------------------------------
void Player::Load()
{
    //�摜�̓ǂݍ���
    Normal = LoadDivGraph("data/player.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, imageSizeX, imageSizeY, playerImg);
}

//-------------------------------------
//�R���X�g���N�^�i�����������j
//-------------------------------------
void Player::Init(Collision& collision)
{
    //���W
    px = 200.0f;
    py = 740.0f;
    VX = 0.0f;
    VY = 0.0f;
    animTimer = 0.0f;
    jumpFlg = false;
    //�����蔻�菉����
    collision.initRect(playerHit, hitSizeX, hitSizeY);
    collision.initRect(playerFootCollider, hitSizeX - colliderOffset, 1);
    collision.initRect(playerHeadCollider, hitSizeX - colliderOffset, 1);

    //���E���猩�ăZ���^�[�ɂȂ�悤�Ɍv�Z
    drawOffsetX = (hitSizeX - imageSizeX) / 2;
    drawOffsetY = (hitSizeY - imageSizeY);

    onGround = false;
    hitHead = false;
    jumpTimer = jumpButtonAcceptTime;

    isJumpPush = false;
    prevJumpButton = false;
}

//-------------------------------------
//�v���C���[�X�V����
//-------------------------------------
void Player::Update(float deltaTime, Collision& collision, sHitRect playAreaRect)
{
    bool isMove = inputMove(deltaTime);

    //��ʍ��E����L�������͂ݏo���Ă��Ȃ���
    // ���㉺�����͌��ɗ������肷��悤�ɂ��邽�߃`�F�b�N�����B
    if (px < playAreaRect.worldLX)
    {
        px = playAreaRect.worldLX + 2;
    }   
    if (px + hitSizeX > playAreaRect.worldRX)
    {
        px = playAreaRect.worldRX - hitSizeX;
        VX = 0;
    }

    //�ڒn���Ă���H
    if (onGround)
    {
        jumpFlg = false;
        VY = 0.0f;
        jumpTimer = jumpButtonAcceptTime;
    }
    else
    {
        jumpFlg = true;
    }

    //����ɓ��������H
    if (onGround)
    {
        VY = hitHeadBreakRatio * VY;
    }

    //�����蔻��ʒu�X�V
    collision.updateWorldRect(playerHit, px, py);
    collision.updateWorldRect(playerFootCollider, px + colliderOffset / 2, py + playerHit.h);
    collision.updateWorldRect(playerHeadCollider, px + colliderOffset / 2, py - playerHeadCollider.h);

    ////////////////////////////
    // �A�j���[�V�����v�Z
    ////////////////////////////
    if (isMove)
    {
        // animTimer �Ŏ��Ԍo�߂𑪂�A 1.0 / animationFPS �����̃A�j���[�V�����p�^�[���ɐi��ł悢�b����
        // �Ȃ����玟�̃R�}�ɐi�߂�
        animTimer += deltaTime;
        if (animTimer > 1.0f / animationFPS)
        {
            // �^�C�}�[��0�� ���̃p�^�[���ɐi�߂邪�AanimPetternNum�𒴂��Ȃ��悤�Ƀ��[�v������
            animTimer = 0.0f;
            animNowPattern++;
            animNowPattern %= animPatternNum; // �]��Ȃ̂� 0,1,2�̂����ꂩ�ɂȂ�
        }
    }
    else
    {
        animNowPattern = 1; // �Î~���͑������낦���p�^�[���ɋ�������
    }

    // �ŏI�I�ȓY�������v�Z 
    // �Y���� = ��̃p�^�[���̃R�}�� + �A�j���[�V������� * �P�̃A�j���[�V�����̃p�^�[������
    animNowIndex = animNowPattern + animNowType * animPatternNum;
}

//-------------------------------------
//�f�X�g���N�^�i�㏈���j
//-------------------------------------
void Player::Finalize()
{
    if (playerImg[0] != -1)
    {
        for (int i = 0; i < animPatternNum * animTypeNum; i++)
        {
            DeleteGraph(playerImg[i]);
        }
    }
    DeleteGraph(Normal);
}

sHitRect Player::getPlayerHitRect()
{
    return playerHit;
}

void Player::playerfixColPosition(sHitRect& hitRect)
{
    //���E�̕ǂɓ��������̂ŉ��������x���O��
    if (px != hitRect.worldLX)
    {
        VX = 0.0f;
    }
    //�㉺�̕ǂɓ��������̂ŏc�������x��0��
    if (py != hitRect.worldLY)
    {
        VY = 0.0f;
    }

    px = hitRect.worldLX;
    py = hitRect.worldLY;

    playerHit = hitRect;
}

void Player::playerSetGroundFlg(bool groundFlg)
{
    onGround = groundFlg;
}

void Player::playerSetHeadHitFlg(bool headHitFlg)
{
    hitHead = headHitFlg;
}

sHitRect Player::playerGetGroundCollider()
{
    return playerFootCollider;
}

sHitRect Player::playerGetHeadCollider()
{
    return playerHeadCollider;
}

bool Player::inputMove(float deltaTime)
{
    ///////////////////////////////////////////
    // ���͏���
    ///////////////////////////////////////////
    // �W�����v�{�^���������u�Ԃ��H
    if (CheckHitKey(KEY_INPUT_SPACE))
    {
        if (!prevJumpButton)
        {
            isJumpPush = true;
        }
        else
        {
            isJumpPush = false;
        }
        prevJumpButton = true;
    }
    else
    {
        prevJumpButton = false;
    }


    bool isMove = false;
    if (CheckHitKey(KEY_INPUT_LEFT))
    {
        if (!jumpFlg)
        {
            // �����]��
            if (VX > 0.0f)
            {
                VX *= brakeRatio;
            }
            // �n������x
            VX += -moveAccell * deltaTime;
        }
        else
        {
            //�󒆉����x
            VX += -moveAccell * inAirMoveAccelRatio * deltaTime;
        }
        //���x�N���b�v
        if (VX < -maxMoveSpeed)
        {
            VX = -maxMoveSpeed;
        }
        animNowType = Normal;
        isMove = true;
    }
    if (CheckHitKey(KEY_INPUT_RIGHT))
    {
        if (!jumpFlg)
        {
            // �����]��
            if (VX < 0.0f)
            {
                VX *= brakeRatio;
            }
            // �n������x
            VX += +moveAccell * deltaTime;
        }
        else
        {
            // �󒆉����x
            VX += +moveAccell * inAirMoveAccelRatio * deltaTime;
        }

        // ���x�N���b�v
        if (VX > maxMoveSpeed)
        {
            VX = +maxMoveSpeed;
        }
        animNowType = Normal;
        isMove = true;
    }

    //�W�����v�L�[�������ꂽ�u�Ԃł��W�����v�\���H
    if (isJumpPush && !jumpFlg && jumpTimer > 0.0f)
    {
        VY -= jumpInitialVelocity;
        jumpFlg = true;
        onGround = false;
    }

    // �W�����v���������ŏ㏸�^�C�}�[���ԂȂ�㏸
    if (prevJumpButton && jumpTimer > 0.0f)
    {
        VY -= jumpUpSpeed * deltaTime;
    }

    // �W�����v���Ȃ�d�͂𔭐�
    if (jumpFlg)
    {
        jumpTimer -= deltaTime;
        VY += gravity * deltaTime;
    }

    // �L�[���͂Ȃ��Ƃ��n��ł̌���
    if (!isMove && !jumpFlg)
    {
        VY *= frictionRatio;
    }
    // �������x�}��
    if (VY * deltaTime > maxFallSpeed)
    {
        VY = maxFallSpeed;
    }
    // �ʒu�X�V
    px += VX;
    py += VY;

    return isMove;
}