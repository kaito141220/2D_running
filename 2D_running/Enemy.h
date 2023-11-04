#pragma once
#include "Collision.h"
#include "DxLib.h"
#include "scrollManager.h"

class Enemy
{
public:
    //画像情報
    int shotImg;
    float shotX;
    float shotY;
    const float shotSpeed = 10.0f;

    int counter;


    static const int animPatternNum = 8;           // アニメーションのパターン数
    static const int animTypeNum = 1;              // アニメーションの種類

    const float animationFPS = 8.0f;               // 秒感8コマのアニメーション

    const int imageSizeX  = 100;                     // 画像横サイズ
    const int imageSizeY  = 80;                     // 画像縦サイズ
    const int hitSizeX    = 100;                     // 当たり判定Xサイズ
    const int hitSizeY    = 80;                     // 当たり判定Yサイズ
    const float moveSpeed = 8.0f;                 // 敵移動速度   

    float ex;                                       // エネミーX座標
    float ey;                                       // エネミーY座標
    float EX;                                       // エネミーX移動速度
    float EY;                                       // エネミーY移動速度
    float enemyEndposition;                         // エネミーの終端位置
    float animTimer;                                // アニメーションタイマー
    int   animNowType;                              // 現在のアニメ種類
    int   animNowPattern;                           // 現在のアニメパターン
    int   animNowIndex;                             // アニメーション画像配列の添え字 0～10までの数値
    int   drawOffsetX;                              // 横方向ずらし量
    int   drawOffsetY;                              // 縦方向ずらし量

    bool direction; //true：左から右、false：右から左

    int enemyImg[animPatternNum * animTypeNum];     // プレイヤーの画像ハンドル
    int Normal;

    sHitRect enemyHit;                              // プレイヤー当たり判定

    /// <summary>
    /// 画像読み込み
    /// </summary>
    void Load();

    /// <summary>
    /// プレイヤー初期化
    /// </summary>
    void Init(Collision& collision);

    /// <summary>
    /// プレイヤー更新
    /// </summary>
    /// <param name="deltaTime">1フレーム分の処理時間</param>
    /// <param name="playAreaRect">プレイ画面のワールド座標矩形。screenManagerのgetPlayArea()関数の戻り値を使用する</param>
    void Update(float deltaTime, Collision& collision, sHitRect playAreaRect);


    /// <summary>
    /// プレイヤー描画
    /// </summary>
    /// <param name="scrollOffsetX">スクロール移動量X</param>
    /// <param name="scrollOffsetY">スクロール移動量Y</param>
    //-------------------------------------
    //エネミー描画
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
    /// プレイヤー後始末
    /// </summary>
    void Finalize();




};