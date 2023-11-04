#include "GameManager.h"

LONGLONG nowCount, prevCount;

void Load(Player& player, Back& back, Scroll& scroll, Collision& collision, Enemy& enemy)
{
	//DxLib初期化
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen(DX_SCREEN_BACK);
	SetGraphMode(1920, 1080, 32);

	if (DxLib_Init() == -1)
	{
		DxLib_End();
	}

	player.Load();
	enemy.Load();
	back.Load();

	nowCount = prevCount = GetNowHiPerformanceCount();
	float fixedDeltaTime = 1.0f / 60.0f;					//60分の1秒 = 0.1666...秒
	float waitFrameTime = 15500;							//16000マイクロ秒 = 16ミリ秒 = 0.016秒
}

void Loop(Player& player, Back& back, Scroll& scroll, Collision& collision, Enemy& enemy)
{
	while (ProcessMessage() == 0)
	{
		switch (gameStatus)
		{
		case INIT:

			player.Init(collision);
			back.Init();
			enemy.Init(collision);


			sHitRect stageRect;
			stageRect.worldLX = 0;
			stageRect.worldLY = 0;
			stageRect.worldRX = 30 * 450;	//30ドットが横に450個（13500）
			stageRect.worldRY = 30 * 36;	//30ドットが縦に36個（1080）

			scroll.scrollManagerInit(1980, 1080, stageRect, collision);
			ClearDrawScreen();

			gameStatus = TITLE;

			ScreenFlip();

			break;


		case TITLE:
			ClearDrawScreen();

			//タイトル描画
			back.DrawTitle();

			if (CheckHitKey(KEY_INPUT_RETURN))	//エンターキーでプレイ状態へ
			{
				WaitTimer(500.0f);
				gameStatus = PLAY;
			}

			if (CheckHitKey(KEY_INPUT_END))		//Endキーで終了
			{
				DxLib_End();
			}


 			ScreenFlip();

			break;

		case PLAY:
			//deltaTime計測
			float deltaTime;
			nowCount = GetNowHiPerformanceCount();
			deltaTime = (nowCount - prevCount) / 1000000.0f;

			//プレイヤー更新処理 60fps想定
			player.Update(1.0f / 60.0f, collision, scroll.getPlayArea());
			enemy.Update (1.0f / 60.0f, collision, scroll.getPlayArea());

			// プレイヤーの当たり判定矩形
			sHitRect playerRect;
			playerRect = player.getPlayerHitRect();

			// もしぶつかったなら当たり判定ボックスからプレイヤー位置を修正する
			if (back.mapHitCalc(playerRect, collision, back))
			{
				player.playerfixColPosition(playerRect);
			}

			//足元チェック
			playerRect = player.playerGetGroundCollider();
			player.playerSetGroundFlg(back.mapHitCalc(playerRect, collision, back));

			//頭上チェック
			playerRect = player.playerGetHeadCollider();
			player.playerSetHeadHitFlg(back.mapHitCalc(playerRect, collision, back));

			//スクロール更新処理
			scroll.scrollUpdate(playerRect, deltaTime, collision);

			//スクロール量取得
			int scrOffsX, scrOffsY;
			scrOffsX = scroll.scrollGetDrawOffsetX();
			scrOffsY = 0;

			//画面更新処理
			ClearDrawScreen();

			back.DrawBackAll();

			// マップの描画
			back.DrawMap(scrOffsX, scrOffsY, scroll);

			//プレイヤーの描画
			player.Draw(scrOffsX, scrOffsY, scroll);
			
			enemy.ShotDraw(scrOffsX, scrOffsY, scroll);

			//エネミーの描画
			enemy.Draw(scrOffsX, scrOffsY, scroll);

			if (player.py >= 1030)
			{
				WaitTimer(500.0f);
				gameStatus = DEFEAT;
			}

			if (player.px >= 13200 && player.py >= 750)
			{
				WaitTimer(500.0f);
				gameStatus = CLEAR;
			}

			ScreenFlip();
			prevCount = nowCount;

			if (CheckHitKey(KEY_INPUT_ESCAPE))	//エンターキーでタイトルへ
			{
				gameStatus = INIT;		// 状態を初期化にする
			}

			break;

		case CLEAR:
			ClearDrawScreen();

			back.DrawResult();

			if (CheckHitKey(KEY_INPUT_RETURN))	//エンターキーでタイトルへ
			{
				WaitTimer(500.0f);
				gameStatus = INIT;
			}
			
			if (CheckHitKey(KEY_INPUT_END))		//Endキーで終了
			{
				DxLib_End();
			}

			ScreenFlip();

			break;


		case DEFEAT:
			ClearDrawScreen();

			back.DrawDefeat();

			if (CheckHitKey(KEY_INPUT_RETURN))	//エンターキーでタイトルへ
			{
				WaitTimer(500.0f);
				gameStatus = INIT;
			}

			if (CheckHitKey(KEY_INPUT_END))		//Endキーで終了
			{
				DxLib_End();
			}

			ScreenFlip();

			break;

		default:
			break;
		}
	}

	//デストラクタ
	player.Finalize();
	back.Finelize();
	enemy.Finalize();
	//DXライブラリの後始末
	DxLib_End();
}
