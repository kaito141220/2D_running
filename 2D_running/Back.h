#pragma once
#include "DxLib.h"
#include "Collision.h"
#include "GameStatus.h"
#include "scrollManager.h"

struct sMapLayer
{
    int** mapData;                  //�}�b�v�`�b�v2�����z��
    int mapXNum;                    //���ɕ���ł��鐔
    int mapYNum;                    //�c�ɕ���ł��鐔
};

//CSV�}�b�v�f�[�^
extern sMapLayer map;

class Back
{
public:
    //////////////////////////////////////////////
    // �萔��`
    //////////////////////////////////////////////
    //�^�C�g��
    int titleImg;                   //�^�C�g���摜�n���h��
    int resultImg;                  //���U���g�摜�n���h���i�����j
    int defeatImg;                  //���U���g�摜�n���h���i���s�j
    int goalImg;                    //�S�[���摜�n���h��
    int goalX = 0;                  //�S�[��X���W
    int goalY = 0;                  //�S�[��X���W
    int X, Y;                       //�^�C�g����X,Y���W
    //�}�b�v�`�b�v
    const int   mapChipSize = 30;   //�}�b�v�`�b�v�T�C�Y
    const int   chipImgXNum = 4 ;   //�}�b�v�摜������
    const int   chipImgYNum = 1 ;   //�}�b�v�摜�c����

    int   mapChipImg[30];           //�摜�n���h���z��

    //�w�i�摜
    int back1;


    // �摜�iCSV�j�ǂݍ���
    void Load();
    // ����������
    void Init();
    // �}�b�v�̕`��
    void DrawMap(int scrollOffsetX, int scrollOffsetY, Scroll& scroll);
    // �㏈��
    void Finelize();
    // �}�b�v�ƏՓ˂��Ă��邩true/false�ŕԂ�
    bool mapHitCalc(sHitRect& checkRect, Collision& collision, Back& back);
    // CSV�t�@�C���̓ǂݍ���(�}�b�v���[�_�[)
    bool LoadCsv    (sMapLayer& dst, const char* mapCSVFileName);
    // �}�b�v�̔j���i�������J���j
    void mapUnloader(sMapLayer& dst);
    // CSV�}�b�v�`��
    void DrawCsv(sMapLayer& layer, int scrollOffsetX, int scrollOffsetY, Scroll& scroll);
    // �^�C�g���`��
    void DrawTitle  ();
    // ���U���g�`��(����)
    void DrawResult ();
    // ���U���g�`��(���s)
    void DrawDefeat();
    //�S�[���t���b�O�`��
    void DrawGoal();
    //�w�i�`��ϐ�
    void DrawBackAll();
    // �}�b�v�C���f�b�N�X�ʒu����u���b�N�ԍ���Ԃ�
    unsigned char mapGetBlockType   (int ix, int iy);
    unsigned char mapGetBlockTypeXY (int x, int y);
};