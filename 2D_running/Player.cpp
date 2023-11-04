#include "Player.h"

//-------------------------------------
//画像読み込み
//-------------------------------------
void Player::Load()
{
    //画像の読み込み
    Normal = LoadDivGraph("data/player.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, imageSizeX, imageSizeY, playerImg);
}

//-------------------------------------
//コンストラクタ（初期化処理）
//-------------------------------------
void Player::Init(Collision& collision)
{
    //座標
    px = 200.0f;
    py = 740.0f;
    VX = 0.0f;
    VY = 0.0f;
    animTimer = 0.0f;
    jumpFlg = false;
    //当たり判定初期化
    collision.initRect(playerHit, hitSizeX, hitSizeY);
    collision.initRect(playerFootCollider, hitSizeX - colliderOffset, 1);
    collision.initRect(playerHeadCollider, hitSizeX - colliderOffset, 1);

    //左右から見てセンターになるように計算
    drawOffsetX = (hitSizeX - imageSizeX) / 2;
    drawOffsetY = (hitSizeY - imageSizeY);

    onGround = false;
    hitHead = false;
    jumpTimer = jumpButtonAcceptTime;

    isJumpPush = false;
    prevJumpButton = false;
}

//-------------------------------------
//プレイヤー更新処理
//-------------------------------------
void Player::Update(float deltaTime, Collision& collision, sHitRect playAreaRect)
{
    bool isMove = inputMove(deltaTime);

    //画面左右からキャラがはみ出していないか
    // ※上下方向は穴に落ちたりするようにするためチェックせず。
    if (px < playAreaRect.worldLX)
    {
        px = playAreaRect.worldLX + 2;
    }   
    if (px + hitSizeX > playAreaRect.worldRX)
    {
        px = playAreaRect.worldRX - hitSizeX;
        VX = 0;
    }

    //接地している？
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

    //頭上に当たった？
    if (onGround)
    {
        VY = hitHeadBreakRatio * VY;
    }

    //当たり判定位置更新
    collision.updateWorldRect(playerHit, px, py);
    collision.updateWorldRect(playerFootCollider, px + colliderOffset / 2, py + playerHit.h);
    collision.updateWorldRect(playerHeadCollider, px + colliderOffset / 2, py - playerHeadCollider.h);

    ////////////////////////////
    // アニメーション計算
    ////////////////////////////
    if (isMove)
    {
        // animTimer で時間経過を測り、 1.0 / animationFPS →次のアニメーションパターンに進んでよい秒数に
        // なったら次のコマに進める
        animTimer += deltaTime;
        if (animTimer > 1.0f / animationFPS)
        {
            // タイマーを0に 次のパターンに進めるが、animPetternNumを超えないようにループさせる
            animTimer = 0.0f;
            animNowPattern++;
            animNowPattern %= animPatternNum; // 余りなので 0,1,2のいずれかになる
        }
    }
    else
    {
        animNowPattern = 1; // 静止時は足をそろえたパターンに強制する
    }

    // 最終的な添え字を計算 
    // 添え字 = 一つのパターンのコマ数 + アニメーション種類 * １つのアニメーションのパターン枚数
    animNowIndex = animNowPattern + animNowType * animPatternNum;
}

//-------------------------------------
//デストラクタ（後処理）
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
    //左右の壁に当たったので横方向速度を０に
    if (px != hitRect.worldLX)
    {
        VX = 0.0f;
    }
    //上下の壁に当たったので縦方向速度を0に
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
    // 入力処理
    ///////////////////////////////////////////
    // ジャンプボタン押した瞬間か？
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
            // 方向転換
            if (VX > 0.0f)
            {
                VX *= brakeRatio;
            }
            // 地上加速度
            VX += -moveAccell * deltaTime;
        }
        else
        {
            //空中加速度
            VX += -moveAccell * inAirMoveAccelRatio * deltaTime;
        }
        //速度クリップ
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
            // 方向転換
            if (VX < 0.0f)
            {
                VX *= brakeRatio;
            }
            // 地上加速度
            VX += +moveAccell * deltaTime;
        }
        else
        {
            // 空中加速度
            VX += +moveAccell * inAirMoveAccelRatio * deltaTime;
        }

        // 速度クリップ
        if (VX > maxMoveSpeed)
        {
            VX = +maxMoveSpeed;
        }
        animNowType = Normal;
        isMove = true;
    }

    //ジャンプキーが押された瞬間でかつジャンプ可能か？
    if (isJumpPush && !jumpFlg && jumpTimer > 0.0f)
    {
        VY -= jumpInitialVelocity;
        jumpFlg = true;
        onGround = false;
    }

    // ジャンプ長押し中で上昇タイマー期間なら上昇
    if (prevJumpButton && jumpTimer > 0.0f)
    {
        VY -= jumpUpSpeed * deltaTime;
    }

    // ジャンプ中なら重力を発生
    if (jumpFlg)
    {
        jumpTimer -= deltaTime;
        VY += gravity * deltaTime;
    }

    // キー入力ないとき地上での減速
    if (!isMove && !jumpFlg)
    {
        VY *= frictionRatio;
    }
    // 落下速度抑制
    if (VY * deltaTime > maxFallSpeed)
    {
        VY = maxFallSpeed;
    }
    // 位置更新
    px += VX;
    py += VY;

    return isMove;
}