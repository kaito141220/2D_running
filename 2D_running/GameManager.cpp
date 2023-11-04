#include "GameManager.h"

LONGLONG nowCount, prevCount;

void Load(Player& player, Back& back, Scroll& scroll, Collision& collision, Enemy& enemy)
{
	//DxLib������
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
	float fixedDeltaTime = 1.0f / 60.0f;					//60����1�b = 0.1666...�b
	float waitFrameTime = 15500;							//16000�}�C�N���b = 16�~���b = 0.016�b
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
			stageRect.worldRX = 30 * 450;	//30�h�b�g������450�i13500�j
			stageRect.worldRY = 30 * 36;	//30�h�b�g���c��36�i1080�j

			scroll.scrollManagerInit(1980, 1080, stageRect, collision);
			ClearDrawScreen();

			gameStatus = TITLE;

			ScreenFlip();

			break;


		case TITLE:
			ClearDrawScreen();

			//�^�C�g���`��
			back.DrawTitle();

			if (CheckHitKey(KEY_INPUT_RETURN))	//�G���^�[�L�[�Ńv���C��Ԃ�
			{
				WaitTimer(500.0f);
				gameStatus = PLAY;
			}

			if (CheckHitKey(KEY_INPUT_END))		//End�L�[�ŏI��
			{
				DxLib_End();
			}


 			ScreenFlip();

			break;

		case PLAY:
			//deltaTime�v��
			float deltaTime;
			nowCount = GetNowHiPerformanceCount();
			deltaTime = (nowCount - prevCount) / 1000000.0f;

			//�v���C���[�X�V���� 60fps�z��
			player.Update(1.0f / 60.0f, collision, scroll.getPlayArea());
			enemy.Update (1.0f / 60.0f, collision, scroll.getPlayArea());

			// �v���C���[�̓����蔻���`
			sHitRect playerRect;
			playerRect = player.getPlayerHitRect();

			// �����Ԃ������Ȃ瓖���蔻��{�b�N�X����v���C���[�ʒu���C������
			if (back.mapHitCalc(playerRect, collision, back))
			{
				player.playerfixColPosition(playerRect);
			}

			//�����`�F�b�N
			playerRect = player.playerGetGroundCollider();
			player.playerSetGroundFlg(back.mapHitCalc(playerRect, collision, back));

			//����`�F�b�N
			playerRect = player.playerGetHeadCollider();
			player.playerSetHeadHitFlg(back.mapHitCalc(playerRect, collision, back));

			//�X�N���[���X�V����
			scroll.scrollUpdate(playerRect, deltaTime, collision);

			//�X�N���[���ʎ擾
			int scrOffsX, scrOffsY;
			scrOffsX = scroll.scrollGetDrawOffsetX();
			scrOffsY = 0;

			//��ʍX�V����
			ClearDrawScreen();

			back.DrawBackAll();

			// �}�b�v�̕`��
			back.DrawMap(scrOffsX, scrOffsY, scroll);

			//�v���C���[�̕`��
			player.Draw(scrOffsX, scrOffsY, scroll);
			
			enemy.ShotDraw(scrOffsX, scrOffsY, scroll);

			//�G�l�~�[�̕`��
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

			if (CheckHitKey(KEY_INPUT_ESCAPE))	//�G���^�[�L�[�Ń^�C�g����
			{
				gameStatus = INIT;		// ��Ԃ��������ɂ���
			}

			break;

		case CLEAR:
			ClearDrawScreen();

			back.DrawResult();

			if (CheckHitKey(KEY_INPUT_RETURN))	//�G���^�[�L�[�Ń^�C�g����
			{
				WaitTimer(500.0f);
				gameStatus = INIT;
			}
			
			if (CheckHitKey(KEY_INPUT_END))		//End�L�[�ŏI��
			{
				DxLib_End();
			}

			ScreenFlip();

			break;


		case DEFEAT:
			ClearDrawScreen();

			back.DrawDefeat();

			if (CheckHitKey(KEY_INPUT_RETURN))	//�G���^�[�L�[�Ń^�C�g����
			{
				WaitTimer(500.0f);
				gameStatus = INIT;
			}

			if (CheckHitKey(KEY_INPUT_END))		//End�L�[�ŏI��
			{
				DxLib_End();
			}

			ScreenFlip();

			break;

		default:
			break;
		}
	}

	//�f�X�g���N�^
	player.Finalize();
	back.Finelize();
	enemy.Finalize();
	//DX���C�u�����̌�n��
	DxLib_End();
}
