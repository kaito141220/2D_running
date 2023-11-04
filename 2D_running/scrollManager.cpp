#include "scrollManager.h"
#include "DxLib.h"
#pragma warning (disable:4996)

void Scroll::scrollManagerInit(int screenwidth, int screenheight, const sHitRect& worldRect, Collision& collision)
{	
	screenWidth  = screenwidth ;					// スクリーン幅
	screenHeight = screenheight;					// スクリーン高さ

	scrollXMax = worldRect.worldRX - screenWidth;	// 最大右方向スクロール量

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

	// スクリーンのワールド座標更新
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
	// 現在のスクロールのワールド座標を計算する
	// 画面4隅のワールド座標を矩形化する
	sHitRect rect;
	rect.worldLX = scrollX;
	rect.worldRX = scrollX + screenWidth;

	return rect;
}
