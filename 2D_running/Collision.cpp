#include "collision.h"
#include "math.h"
#pragma warning (disable:4996)

void Collision::initRect(sHitRect& dstRect, float w, float h)
{
	dstRect.w = w;
	dstRect.h = h;

	dstRect.worldLX = 0.0f;
	dstRect.worldRX = w;
	dstRect.worldLY = 0.0f;
	dstRect.worldRY = h;
}

void Collision::updateWorldRect(sHitRect& dstRect, float x, float y)
{
	dstRect.worldLX = x;
	dstRect.worldRX = x + dstRect.w;
	dstRect.worldLY = y;
	dstRect.worldRY = y + dstRect.h;
}

bool Collision::isHitRect(const sHitRect& rect1, const sHitRect& rect2)
{
	// ��̋�`�̓����蔻��̌������
	return (rect1.worldLX < rect2.worldRX&&
			rect2.worldLX < rect1.worldRX&&
			rect1.worldLY < rect2.worldRY&&
			rect2.worldLY < rect1.worldRY);
}

void Collision::calcFixHitRectPosition(sHitRect& movableRect, const sHitRect& staticRect, BlockInfo blockInfo)
{
    //�㉺�̉����߂��������v�Z
    float left  = staticRect.worldLX - movableRect.worldRX;
    float right = staticRect.worldRX - movableRect.worldLX;
    float up    = staticRect.worldLY - movableRect.worldRY;
    float down  = staticRect.worldRY - movableRect.worldLY;

    //�u���b�N��񂩂�ǂ����݂��Ȃ��Ƃ��A�킴�Ƒ傫�ȉ����߂��ʂ�ݒ肵
    //�ȍ~�̍ŏ��l��I�Ԍ�₩��O�����悤�ɂ���
    if ((!blockInfo & BLOCK_LEFT))
    {
        left  = -staticRect.w;
    }
    if ((!blockInfo & BLOCK_RIGHT))
    {
        right = staticRect.w;
    }
    if ((!blockInfo & BLOCK_UP))
    {
        up    = -staticRect.h;
    }
    if ((!blockInfo & BLOCK_DOWN))
    {
        down  = staticRect.h;
    }

    //fabs() �̓J�b�R���̕ϐ��̐�Βl�����߂�֐�
    //dx ��x�����̍ŒZ�����߂��ړ���
    //dy ��y�����̍ŒZ�����߂��ړ������߂�
    float dx = (fabs(left) < fabs(right)) ? left : right;
    float dy = (fabs(up)   < fabs(down) ) ?  up  : down ;

    //�G���[�� �����Ƃ��ǂ��Ȃ����̏Փ˂������H
    if (fabs(dx) == fabs(dy))
    {
        return;
    }

    //x���� y�����̍ŒZ�����߂��������r���A�ŒZ�Ŗ߂�����@�Ŗ߂�
    if (fabs(dx) < fabs(dy))
    {
        movableRect.worldLX += dx;
        movableRect.worldRX += dx;
    }
    else
    {
        movableRect.worldLY += dy;
        movableRect.worldRY += dy;
    }
}
