#include "Back.h"
#pragma warning(disable:4996)
#include "DxLib.h"

sMapLayer map;

//--------------------------------------------------------
//�Ǐ��擾 ����u���b�N�̕ǂ̗L���𒲂� BlockInfo��Ԃ�
//--------------------------------------------------------
Collision::BlockInfo mapGetBlockInfo(int ix, int iy, Back& back)
{
    Collision::BlockInfo ret = 0;

    // �u���b�N��1�����󔒂��H
    if (back.mapGetBlockType(ix - 1, iy) == 0)
	{
		ret |= BLOCK_LEFT;
	}
	// �u���b�N�̈�E���󔒂��H
	if (back.mapGetBlockType(ix + 1, iy) == 0)
	{
		ret |= BLOCK_RIGHT;
	}
	// �u���b�N�̈�オ�󔒂��H
	if (back.mapGetBlockType(ix, iy - 1) == 0)
	{
		ret |= BLOCK_UP;
	}
	//�u���b�N�̈�����󔒂��H
	if (back.mapGetBlockType(ix, iy + 1) == 0)
	{
		ret |= BLOCK_DOWN;
	}

    // ret�ɂ�4�����̕ǂ̗L���̃r�b�g���d�ˍ��킳��Ă���
    return ret;
}

void Back::Load()
{
    //�^�C�g���摜�ǂݍ���
    titleImg = LoadGraph("data/title.png");
    resultImg = LoadGraph("data/result.png");
    defeatImg = LoadGraph("data/defeat.png");
    goalImg = LoadGraph("data/goal.png");

    back1 = LoadGraph("data/back1.png");

    //�摜�����@�������~�c�����A���������A�c�������A�`�b�v1���̉��T�C�Y�A�`�b�v1���̏c�T�C�Y�A�摜�z��擪�|�C���^
    LoadDivGraph("data/map.png", chipImgXNum * chipImgYNum, chipImgXNum, chipImgYNum, mapChipSize, mapChipSize, mapChipImg);

    LoadCsv(map, "map/map.csv");
}

//-------------------------------------
//�R���X�g���N�^�i�����������j    
//-------------------------------------
void Back::Init()
{
    //���W
    X = 0;
    Y = 0;

    goalX = 100;
    goalY = 710;
}

//-------------------------------------
//�f�X�g���N�^�i�㏈���j
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
//�^�C�g���`��
//-------------------------------------
void Back::DrawTitle()
{
    DrawGraph(X, Y, titleImg, TRUE);
}

//-------------------------------------
//���U���g�`��(����)
//-------------------------------------
void Back::DrawResult()
{
    DrawGraph(X, Y, resultImg, TRUE);
}

//-------------------------------------
//���U���g�`��(���s)
//-------------------------------------
void Back::DrawDefeat()
{
    DrawGraph(X, Y, defeatImg, TRUE);
}

//-------------------------------------
//�S�[���`��
//-------------------------------------
void Back::DrawGoal()
{
    DrawGraph(goalX, goalY, goalImg, TRUE);
}

//-------------------------------------
//�w�i�`��
//-------------------------------------
void Back::DrawBackAll()
{
    DrawGraph(X, Y, back1, TRUE);
}

//-------------------------------------
//�}�b�v�̕`��
//-------------------------------------
void Back::DrawCsv(sMapLayer& layer, int scrollOffsetX, int scrollOffsetY, Scroll& scroll)
{
    scrollOffsetX = scroll.scrollGetDrawOffsetX();

    for (int y = 0; y < layer.mapYNum; y++)
    {
        for (int x = 0; x < layer.mapXNum; x++)
        {
            int imgIndex = layer.mapData[y][x];     //  map�z����u���b�N��ނ��擾

            //id���|1�̎��͕`����X�L�b�v����
            if (imgIndex == -1)
            {
                continue;
            }

            int imgHnadle = mapChipImg[imgIndex];   //index���g�p���ĉ摜�n���h���z�񂩂�摜�n���h�����擾

            //�}�b�v�`�b�v���Ńu���b�N�摜��~���l�߂ĕ`�悷��
            //x��0,1,2�E�E�E�ƕω�����Bx*mapChipSize �̌v�Z��0,30,60,90�E�E�E�ƃu���b�N�����Ƃɑ�����
            DrawGraph(x * mapChipSize - scrollOffsetX, y * mapChipSize, imgHnadle, TRUE);
        }
    }
}

//-------------------------------------
//�}�b�v�̕`��i�܂Ƃ߂���j
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

    //�}�b�v�u���b�N�ƒ����u���b�N�̏Փ˂𒲂ׂ�
    for (int iy = 0; iy < map.mapYNum; iy++)
    {
        //�}�b�v�u���b�N��Y���W
        blockRect.worldLY = static_cast<float>(iy * mapChipSize);
        blockRect.worldRY = static_cast<float>((iy + 1) * mapChipSize);
    
        for (int ix = 0; ix < map.mapXNum; ix++)
        {
            //�����蔻��@�O���ʂ��A�P���ʂ�Ȃ�
            if (map.mapData[iy][ix] != 0)
            {
                //�}�b�v�u���b�N��X���W
                blockRect.worldLX = static_cast<float>(ix * mapChipSize);
                blockRect.worldRX = (ix + 1) * static_cast<float>(mapChipSize);

                //�������Ă��邩
                if (collision.isHitRect(checkRect, blockRect))
                {
                    //1�x�ł��������hitflg��true��
                    hitflg = true;

                    //�u���b�N���i�㉺���E�̕ǂ̗L���j
                    Collision::BlockInfo bi = mapGetBlockInfo(ix, iy, back);

                    //�{�b�N�X�߂荞�ݗʂ��v�Z
                    //checkRect�ɂ͂߂荞�ݖ߂��ʂ�����
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
//CSV�t�@�C���̓ǂݍ���
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
        printf("�t�@�C�������݂��܂���");                    // �t�@�C�����ǂݍ��܂�Ă��Ȃ���ΏI��
        return 0;
    }

    //�t�@�C����1�s��ǂ݂���
    char c = 0;
    //int cnt = 0;
    while (c != '\n')
    {
        //�J���}��ǂݎ��ƃJ�E���g��+1������
        c = fgetc(fp);
        if (',' == c)
        {
            dst.mapXNum++;
        }
        //++cnt;
        //printfDx("%d\n", cnt);
    }
    dst.mapXNum++;

    //1�̃f�[�^��16bit�̍ő�l�u65535�v�i6�����j�Ƃ��ĕ����񂪓����Ă����Ƃ���1�T�C�Y
    int bufferLen = dst.mapXNum * 6 + 1;

    //1�s���̃o�b�t�@���m�ۂ���
    char* buf = new char[bufferLen];
    char* ptmp = buf;

    //�t�@�C���|�C���^��擪�ɖ߂��čēx�A�s����ǂ�
    fseek(fp, 0, SEEK_SET);
    bool firstRow = true;
    while (fgets(buf, bufferLen - 1, fp) != NULL)
    {
        dst.mapYNum++;
    }

    //2�����z��𓮓I�m�ۂ���imap[roe][colum]�j�Ƃ��ē��I�m�ۂ���
    dst.mapData = new int* [dst.mapYNum];

    int ix, iy;
    for (iy = 0; iy < dst.mapYNum; iy++)
    {
        dst.mapData[iy] = new int[dst.mapXNum];
    }

    //�t�@�C���|�C���^��擪�ɖ߂�
    fseek(fp, 0, SEEK_SET);
    //�t�@�C������f�[�^��z��Ɉڂ�
    char* readPoint;
    char* firstPoint;
    for (iy = 0; iy < dst.mapYNum; iy++)
    {
        //1�s����荞��
        fgets(buf, bufferLen - 1, fp);
        readPoint = firstPoint = buf;

        for (ix = 0; ix < dst.mapXNum; ix++)
        {
            //�J���}�܂�readPoint�̈ʒu��i�߂�
            while (',' != *readPoint && '\n' != *readPoint)
            {
                readPoint++;
            }
            //�J���}��NULL�����ɒu��������

            //���̎��_��firstPoint - readPoint�Ԃŕ����񂪊�������̂Ő��l�ɕϊ�
            dst.mapData[iy][ix] = atoi(firstPoint);

            //���̃f�[�^�ǂݎ��ʒu�܂Ői�߂�
            readPoint++;
            firstPoint = readPoint;
        }

    }

    //�t�@�C�������
    fclose(fp);
    //�o�b�t�@���
    delete[] buf;

    return true;
}

//-------------------------------------
//�}�b�v�̍폜
//-------------------------------------
void Back::mapUnloader(sMapLayer& dst)
{
    for (int iy = 0; iy < dst.mapYNum; iy++)
    {
        delete[] dst.mapData[iy];
    }
    delete[] dst.mapData;

    // dst�̏�����
    dst.mapData = NULL;
    dst.mapXNum = 0;
    dst.mapYNum = 0;
}