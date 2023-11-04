#pragma once
#include "collision.h"

class Scroll
{
public:
    float scrollX;      // スクロールX方向値

    float screenXMax;   // スクリーン上の右スクロール開始境界値

    int screenWidth;    // スクリーン幅
    int screenHeight;   // スクリーン高さ
    float scrollXMax;   // 最大横方向スクロール量
    float dx;
    const float scrollSpeed = 2.5f;

    // スクリーンのワールド空間上での矩形範囲
    sHitRect worldScreenRect;
    /// プレイ画面の領域矩形を取得
    sHitRect getPlayArea();
    /// スクロールマネージャ初期化
    void     scrollManagerInit(int screenWidth, int screenHeight, const sHitRect& worldRect, Collision& collision);
    /// スクロールの更新処理
    void     scrollUpdate(const sHitRect& playerRect, float deltaTime, Collision& collision);
    /// スクロールの描画オフセットXを取得
    int      scrollGetDrawOffsetX();
    /// ワールド座標系でのスクリーン矩形を返す
    sHitRect scrollGetWorldScreenRect();
};