#pragma once
#include "DxLib.h"
#include "Collision.h"
#include "GameStatus.h"
#include "scrollManager.h"

struct sMapLayer
{
    int** mapData;                  //マップチップ2次元配列
    int mapXNum;                    //横に並んでいる数
    int mapYNum;                    //縦に並んでいる数
};

//CSVマップデータ
extern sMapLayer map;

class Back
{
public:
    //////////////////////////////////////////////
    // 定数定義
    //////////////////////////////////////////////
    //タイトル
    int titleImg;                   //タイトル画像ハンドル
    int resultImg;                  //リザルト画像ハンドル（成功）
    int defeatImg;                  //リザルト画像ハンドル（失敗）
    int goalImg;                    //ゴール画像ハンドル
    int goalX = 0;                  //ゴールX座標
    int goalY = 0;                  //ゴールX座標
    int X, Y;                       //タイトルのX,Y座標
    //マップチップ
    const int   mapChipSize = 30;   //マップチップサイズ
    const int   chipImgXNum = 4 ;   //マップ画像横枚数
    const int   chipImgYNum = 1 ;   //マップ画像縦枚数

    int   mapChipImg[30];           //画像ハンドル配列

    //背景画像
    int back1;


    // 画像（CSV）読み込み
    void Load();
    // 初期化処理
    void Init();
    // マップの描画
    void DrawMap(int scrollOffsetX, int scrollOffsetY, Scroll& scroll);
    // 後処理
    void Finelize();
    // マップと衝突しているかtrue/falseで返す
    bool mapHitCalc(sHitRect& checkRect, Collision& collision, Back& back);
    // CSVファイルの読み込み(マップローダー)
    bool LoadCsv    (sMapLayer& dst, const char* mapCSVFileName);
    // マップの破棄（メモリ開放）
    void mapUnloader(sMapLayer& dst);
    // CSVマップ描画
    void DrawCsv(sMapLayer& layer, int scrollOffsetX, int scrollOffsetY, Scroll& scroll);
    // タイトル描画
    void DrawTitle  ();
    // リザルト描画(成功)
    void DrawResult ();
    // リザルト描画(失敗)
    void DrawDefeat();
    //ゴールフラッグ描画
    void DrawGoal();
    //背景描画変数
    void DrawBackAll();
    // マップインデックス位置からブロック番号を返す
    unsigned char mapGetBlockType   (int ix, int iy);
    unsigned char mapGetBlockTypeXY (int x, int y);
};