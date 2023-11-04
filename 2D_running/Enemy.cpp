#include "Enemy.h"

void Enemy::Load()
{
	//画像の読み込み
	Normal = LoadDivGraph("data/enemy.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, imageSizeX, imageSizeY, enemyImg);
    shotImg = LoadGraph("data.shot.png");
}

void Enemy::Init(Collision& collision)
{
    //座標
    ex = 400.0f;
    ey = 50.0f;
    EX = 0.0f;
    EY = 0.0f;
    animTimer = 0.0f;

    direction = true;

    counter = 0;

    shotX = ex + 45;
    shotY = ey + 35;
    //当たり判定初期化        
    collision.initRect(enemyHit, hitSizeX, hitSizeY);

    //左右から見てセンターになるように計算
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


    //当たり判定位置更新
    collision.updateWorldRect(enemyHit, ex, ey);

    ////////////////////////////
    // アニメーション計算
    ////////////////////////////
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

    // 最終的な添え字を計算 
    // 添え字 = 一つのパターンのコマ数 + アニメーション種類 * １つのアニメーションのパターン枚数
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
