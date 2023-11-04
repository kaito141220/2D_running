#include "Enemy.h"

void Enemy::Load()
{
	//�摜�̓ǂݍ���
	Normal = LoadDivGraph("data/enemy.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, imageSizeX, imageSizeY, enemyImg);
    shotImg = LoadGraph("data.shot.png");
}

void Enemy::Init(Collision& collision)
{
    //���W
    ex = 400.0f;
    ey = 50.0f;
    EX = 0.0f;
    EY = 0.0f;
    animTimer = 0.0f;

    direction = true;

    counter = 0;

    shotX = ex + 45;
    shotY = ey + 35;
    //�����蔻�菉����        
    collision.initRect(enemyHit, hitSizeX, hitSizeY);

    //���E���猩�ăZ���^�[�ɂȂ�悤�Ɍv�Z
    drawOffsetX = (hitSizeX - imageSizeX) / 2;
    drawOffsetY = (hitSizeY - imageSizeY);
}

void Enemy::Update(float deltaTime, Collision& collision, sHitRect playAreaRect)
{
    counter++;
    if (counter >= 30)
    {
        shotY += deltaTime*shotSpeed;
        if (shotY < 1095)
        {
            shotY = ey;
            counter = 0;
        }
    }

    if (direction)
    {
        ex += moveSpeed;
        if (ex >= playAreaRect.worldRX - 300)
        {
            direction = false;
        }
    }
    else
    {
        ex -= moveSpeed;
        if (ex < playAreaRect.worldLX + 300)
        {
            direction = true;
        }
    }


    //�����蔻��ʒu�X�V
    collision.updateWorldRect(enemyHit, ex, ey);

    ////////////////////////////
    // �A�j���[�V�����v�Z
    ////////////////////////////
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

    // �ŏI�I�ȓY�������v�Z 
    // �Y���� = ��̃p�^�[���̃R�}�� + �A�j���[�V������� * �P�̃A�j���[�V�����̃p�^�[������
    animNowIndex = animNowPattern + animNowType * animPatternNum;
 
}

void Enemy::Finalize()
{
    if (enemyImg[0] != -1)
    {
        for (int i = 0; i < animPatternNum * animTypeNum; i++)
        {
            DeleteGraph(enemyImg[i]);
        }
    }
    DeleteGraph(Normal);
}
