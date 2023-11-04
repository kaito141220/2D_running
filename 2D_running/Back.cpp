#include "Back.h"
#pragma warning(disable:4996)
#include "DxLib.h"

sMapLayer map;

//--------------------------------------------------------
//壁情報取得 あるブロックの壁の有無を調べ BlockInfoを返す
//--------------------------------------------------------
Collision::BlockInfo mapGetBlockInfo(int ix, int iy, Back& back)
{
    Collision::BlockInfo ret = 0;

    // ブロックの1つ左が空白か？
    if (back.mapGetBlockType(ix - 1, iy) == 0)
	{
		ret |= BLOCK_LEFT;
	}
	// ブロックの一つ右が空白か？
	if (back.mapGetBlockType(ix + 1, iy) == 0)
	{
		ret |= BLOCK_RIGHT;
	}
	// ブロックの一つ上が空白か？
	if (back.mapGetBlockType(ix, iy - 1) == 0)
	{
		ret |= BLOCK_UP;
	}
	//ブロックの一つ下が空白か？
	if (back.mapGetBlockType(ix, iy + 1) == 0)
	{
		ret |= BLOCK_DOWN;
	}

    // retには4方向の壁の有無のビットが重ね合わされている
    return ret;
}

void Back::Load()
{
    //タイトル画像読み込み
    titleImg = LoadGraph("data/title.png");
    resultImg = LoadGraph("data/result.png");
    defeatImg = LoadGraph("data/defeat.png");
    goalImg = LoadGraph("data/goal.png");

    back1 = LoadGraph("data/back1.png");

    //画像総数　横枚数×縦枚数、横方向個数、縦方向個数、チップ1枚の横サイズ、チップ1枚の縦サイズ、画像配列先頭ポインタ
    LoadDivGraph("data/map.png", chipImgXNum * chipImgYNum, chipImgXNum, chipImgYNum, mapChipSize, mapChipSize, mapChipImg);

    LoadCsv(map, "map/map.csv");
}

//-------------------------------------
//コンストラクタ（初期化処理）    
//-------------------------------------
void Back::Init()
{
    //座標
    X = 0;
    Y = 0;

    goalX = 100;
    goalY = 710;
}

//-------------------------------------
//デストラクタ（後処理）
//-------------------------------------
void Back::Finelize()
{

    DeleteGraph(titleImg);
    DeleteGraph(resultImg);
    DeleteGraph(defeatImg);
    DeleteGraph(goalImg);
    for (int i = 0; i < chipImgXNum * chipImgYNum; i++)
    {
        DeleteGraph(mapChipImg[i]);
    }

    mapUnloader(map);
}

//-------------------------------------
//タイトル描画
//-------------------------------------
void Back::DrawTitle()
{
    DrawGraph(X, Y, titleImg, TRUE);
}

//-------------------------------------
//リザルト描画(成功)
//-------------------------------------
void Back::DrawResult()
{
    DrawGraph(X, Y, resultImg, TRUE);
}

//-------------------------------------
//リザルト描画(失敗)
//-------------------------------------
void Back::DrawDefeat()
{
    DrawGraph(X, Y, defeatImg, TRUE);
}

//-------------------------------------
//ゴール描画
//-------------------------------------
void Back::DrawGoal()
{
    DrawGraph(goalX, goalY, goalImg, TRUE);
}

//-------------------------------------
//背景描画
//-------------------------------------
void Back::DrawBackAll()
{
    DrawGraph(X, Y, back1, TRUE);
}

//-------------------------------------
//マップの描画
//-------------------------------------
void Back::DrawCsv(sMapLayer& layer, int scrollOffsetX, int scrollOffsetY, Scroll& scroll)
{
    scrollOffsetX = scroll.scrollGetDrawOffsetX();

    for (int y = 0; y < layer.mapYNum; y++)
    {
        for (int x = 0; x < layer.mapXNum; x++)
        {
            int imgIndex = layer.mapData[y][x];     //  map配列よりブロック種類を取得

            //idが－1の時は描画をスキップする
            if (imgIndex == -1)
            {
                continue;
            }

            int imgHnadle = mapChipImg[imgIndex];   //indexを使用して画像ハンドル配列から画像ハンドルを取得

            //マップチップ幅でブロック画像を敷き詰めて描画する
            //xは0,1,2・・・と変化する。x*mapChipSize の計算は0,30,60,90・・・とブロック幅ごとに増える
            DrawGraph(x * mapChipSize - scrollOffsetX, y * mapChipSize, imgHnadle, TRUE);
        }
    }
}

//-------------------------------------
//マップの描画（まとめたやつ）
//-------------------------------------
void Back::DrawMap(int scrollOffsetX, int scrollOffsetY, Scroll& scroll)
{
    DrawCsv(map, scrollOffsetX, scrollOffsetY, scroll);
}

bool Back::mapHitCalc(sHitRect& checkRect, Collision& collision, Back& back)
{
    bool hitflg = false;
    sHitRect blockRect;

    blockRect.w = mapChipSize;
    blockRect.h = mapChipSize;

    //マップブロックと調査ブロックの衝突を調べる
    for (int iy = 0; iy < map.mapYNum; iy++)
    {
        //マップブロックのY座標
        blockRect.worldLY = static_cast<float>(iy * mapChipSize);
        blockRect.worldRY = static_cast<float>((iy + 1) * mapChipSize);
    
        for (int ix = 0; ix < map.mapXNum; ix++)
        {
            //当たり判定　０→通れる、１→通れない
            if (map.mapData[iy][ix] != 0)
            {
                //マップブロックのX座標
                blockRect.worldLX = static_cast<float>(ix * mapChipSize);
                blockRect.worldRX = (ix + 1) * static_cast<float>(mapChipSize);

                //当たっているか
                if (collision.isHitRect(checkRect, blockRect))
                {
                    //1度でも当たればhitflgをtrueに
                    hitflg = true;

                    //ブロック情報（上下左右の壁の有無）
                    Collision::BlockInfo bi = mapGetBlockInfo(ix, iy, back);

                    //ボックスめり込み量を計算
                    //checkRectにはめり込み戻し量が入る
                    collision.calcFixHitRectPosition(checkRect, blockRect, bi);
                }
            }
        }
    }

    return hitflg;
}

unsigned char Back::mapGetBlockType(int ix, int iy)
{
    if (ix < 0 || iy < 0 || ix >= map.mapXNum || iy >= map.mapYNum)
    {
        return 0;
    }
    return map.mapData[iy][ix];
}

unsigned char Back::mapGetBlockTypeXY(int x, int y)
{
    return mapGetBlockType(x / mapChipSize, y / mapChipSize);
}

//-------------------------------------
//CSVファイルの読み込み
//-------------------------------------
bool Back::LoadCsv(sMapLayer& dst, const char* mapCSVFileName)
{
    dst.mapData = NULL;
    dst.mapXNum = 0;
    dst.mapYNum = 0;

    FILE* fp;
    fp = fopen(mapCSVFileName, "rt");
    if (!fp)
    {
        printf("ファイルが存在しません");                    // ファイルが読み込まれていなければ終了
        return 0;
    }

    //ファイルを1行空読みする
    char c = 0;
    //int cnt = 0;
    while (c != '\n')
    {
        //カンマを読み取るとカウントを+1させる
        c = fgetc(fp);
        if (',' == c)
        {
            dst.mapXNum++;
        }
        //++cnt;
        //printfDx("%d\n", cnt);
    }
    dst.mapXNum++;

    //1つのデータを16bitの最大値「65535」（6文字）として文字列が入ってきたときの1サイズ
    int bufferLen = dst.mapXNum * 6 + 1;

    //1行分のバッファを確保する
    char* buf = new char[bufferLen];
    char* ptmp = buf;

    //ファイルポインタを先頭に戻して再度、行数を読む
    fseek(fp, 0, SEEK_SET);
    bool firstRow = true;
    while (fgets(buf, bufferLen - 1, fp) != NULL)
    {
        dst.mapYNum++;
    }

    //2次元配列を動的確保する（map[roe][colum]）として動的確保する
    dst.mapData = new int* [dst.mapYNum];

    int ix, iy;
    for (iy = 0; iy < dst.mapYNum; iy++)
    {
        dst.mapData[iy] = new int[dst.mapXNum];
    }

    //ファイルポインタを先頭に戻す
    fseek(fp, 0, SEEK_SET);
    //ファイルからデータを配列に移す
    char* readPoint;
    char* firstPoint;
    for (iy = 0; iy < dst.mapYNum; iy++)
    {
        //1行分取り込み
        fgets(buf, bufferLen - 1, fp);
        readPoint = firstPoint = buf;

        for (ix = 0; ix < dst.mapXNum; ix++)
        {
            //カンマまでreadPointの位置を進める
            while (',' != *readPoint && '\n' != *readPoint)
            {
                readPoint++;
            }
            //カンマをNULL文字に置き換える

            //この時点でfirstPoint - readPoint間で文字列が完成するので数値に変換
            dst.mapData[iy][ix] = atoi(firstPoint);

            //次のデータ読み取り位置まで進める
            readPoint++;
            firstPoint = readPoint;
        }

    }

    //ファイルを閉じる
    fclose(fp);
    //バッファ解放
    delete[] buf;

    return true;
}

//-------------------------------------
//マップの削除
//-------------------------------------
void Back::mapUnloader(sMapLayer& dst)
{
    for (int iy = 0; iy < dst.mapYNum; iy++)
    {
        delete[] dst.mapData[iy];
    }
    delete[] dst.mapData;

    // dstの初期化
    dst.mapData = NULL;
    dst.mapXNum = 0;
    dst.mapYNum = 0;
}