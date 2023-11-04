#include "scrollManager.h"
#include "DxLib.h"
#pragma warning (disable:4996)

void Scroll::scrollManagerInit(int screenwidth, int screenheight, const sHitRect& worldRect, Collision& collision)
{	
	screenWidth  = screenwidth ;					// �X�N���[����
	screenHeight = screenheight;					// �X�N���[������

	scrollXMax = worldRect.worldRX - screenWidth;	// �ő�E�����X�N���[����

	scrollX = 0;
	collision.initRect(worldScreenRect, static_cast<float>(screenWidth), static_cast<float>(screenHeight));
}

void Scroll::scrollUpdate(const sHitRect& playerRect, float deltaTime, Collision& collision)
{
	scrollX += scrollSpeed;

	if (scrollX > scrollXMax)
	{
		scrollX = scrollXMax;
	}

	// �X�N���[���̃��[���h���W�X�V
	collision.updateWorldRect(worldScreenRect, scrollX, 0.0f);
}

int Scroll::scrollGetDrawOffsetX()
{
	return static_cast<int>(scrollX);
}

sHitRect Scroll::scrollGetWorldScreenRect()
{
	return worldScreenRect;
}

sHitRect Scroll::getPlayArea()
{
	// ���݂̃X�N���[���̃��[���h���W���v�Z����
	// ���4���̃��[���h���W����`������
	sHitRect rect;
	rect.worldLX = scrollX;
	rect.worldRX = scrollX + screenWidth;

	return rect;
}
