#pragma once
#include "Collision.h"
#include "DxLib.h"
#include "scrollManager.h"

class Player
{
public:
    //画像情報
    static const int animPatternNum = 8;           // アニメーションのパターン数
    static const int animTypeNum = 1;              // アニメーションの種類
    
    //ジャンプ情報
    const float animationFPS            = 8.0f  ;   // 秒感8コマのアニメーション
    const float moveSpeed               = 300.0f;   // 移動速度
    const float maxMoveSpeed            = 350.0f;   // 最大横移動速度
    const float moveAccell              = 2.5f;     // 移動加速度
    const float frictionRatio           = 0.01f;     // 摩擦係数
    const float brakeRatio              = 0.1f;     // 進行方向に対して入力逆方向に入れたときのブレーキ係数
    const float inAirMoveAccelRatio     = 3.0f;     // 空中横移動加速率
    const float jumpInitialVelocity     = 15.0f  ;  // ジャンプ初速度
    const float jumpUpSpeed             = 10.0f ;   // 上昇スピード
    const float hitHeadBreakRatio       = -0.1f ;   // 頭上衝突時の減速率

    const int imageSizeX = 50;                      // 画像横サイズ
    const int imageSizeY = 50;                      // 画像縦サイズ
    const int hitSizeX   = 50;                      // 当たり判定Xサイズ
    const int hitSizeY   = 50;                      // 当たり判定Yサイズ

    const float gravity                 = 25.0f ;   // 重力
    const float maxFallSpeed            = 200.0f;   // 最大落下スピード
    const int colliderOffset            = 10    ;   // コライダーずらし量
    const float jumpButtonAcceptTime    = 0.95f ;   // ジャンプボタン受付時間

    //座標情報
    float px;                                       // プレイヤーX座標
    float py;                                       // プレイヤーY座標
    float VX;                                       // プレイヤーX移動速度
    float VY;                                       // プレイヤーY移動速度
    float playerEndposition;                        // プレイヤーの終端位置
    float animTimer;                                // アニメーションタイマー
    int animNowType;                                // 現在のアニメ種類
    int animNowPattern;                             // 現在のアニメパターン
    int animNowIndex;                               // アニメーション画像配列の添え字 0～10までの数値
    int drawOffsetX;                                // 横方向ずらし量
    int drawOffsetY;                                // 縦方向ずらし量

    bool jumpFlg;                                   // ジャンプフラグ
    bool onGround;                                  // 地面にたっているか
    bool hitHead;                                   // 頭がぶつかったか
    float jumpTimer;

    int playerImg[animPatternNum * animTypeNum]; // プレイヤーの画像ハンドル
    int Normal;

    sHitRect playerHit;           // プレイヤー当たり判定
    sHitRect playerFootCollider;  // 接地コライダー
    sHitRect playerHeadCollider;  // 頭上コライダー

    bool prevJumpButton;          // 前のフレームでジャンプボタン押されていたか？
    bool isJumpPush;              // ジャンプボタン押された瞬間か？

    bool inputMove  (float deltaTime);

    /// <summary>
    /// 画像読み込み
    /// </summary>
    void Load();

    /// <summary>
    /// プレイヤー初期化
    /// </summary>
    void Init       (Collision& collision);
   
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
    //プレイヤー描画
    //-------------------------------------
    void Draw(int scrollOffsetX, int scrollOffsetY, Scroll& scroll)
    {
        scrollOffsetX = scroll.scrollGetDrawOffsetX();

        DrawGraph(static_cast<int>(px) + drawOffsetX - scrollOffsetX,
            static_cast<int>(py) + drawOffsetY - scrollOffsetY, playerImg[animNowIndex], TRUE);
    }

    /// <summary>
    /// プレイヤー後始末
    /// </summary>
    void Finalize   ();
   
    /// <summary>
    /// 当たり判定矩形からプレイヤー位置を修正する
    /// </summary>
    /// <param name="hitRect">当たり判定矩形</param>
    void playerfixColPosition   (sHitRect& hitRect);

    /// <summary>
    /// プレイヤーの当たり判定矩形を返す
    /// </summary>
    /// <returns>プレイヤーの当たり判定矩形</returns>
    sHitRect getPlayerHitRect();

    /// <summary>
    /// 地面に立っているか？
    /// </summary>
    /// <param name="groundFlg">足元の当たり判定結果</param>
    void playerSetGroundFlg     (bool groundFlg);
    
    /// <summary>
    /// プレイヤーの足元コライダーのゲット
    /// </summary>
    /// <returns>足元コライダーの矩形情報</returns>
    sHitRect playerGetGroundCollider();

    /// <summary>
    /// 頭を壁にぶつけたか
    /// </summary>
    /// <param name="headHitFlg">頭の当たり判定結果</param>
    void playerSetHeadHitFlg(bool headHitFlg);

    /// <summary>
    /// プレイヤーの頭上コライダーのゲット
    /// </summary>
    /// <returns>頭上コライダーの矩形情報</returns>
    sHitRect playerGetHeadCollider();
};