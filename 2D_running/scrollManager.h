#pragma once
#include "collision.h"

class Scroll
{
public:
    float scrollX;      // �X�N���[��X�����l

    float screenXMax;   // �X�N���[����̉E�X�N���[���J�n���E�l

    int screenWidth;    // �X�N���[����
    int screenHeight;   // �X�N���[������
    float scrollXMax;   // �ő剡�����X�N���[����
    float dx;
    const float scrollSpeed = 2.5f;

    // �X�N���[���̃��[���h��ԏ�ł̋�`�͈�
    sHitRect worldScreenRect;
    /// �v���C��ʂ̗̈��`���擾
    sHitRect getPlayArea();
    /// �X�N���[���}�l�[�W��������
    void     scrollManagerInit(int screenWidth, int screenHeight, const sHitRect& worldRect, Collision& collision);
    /// �X�N���[���̍X�V����
    void     scrollUpdate(const sHitRect& playerRect, float deltaTime, Collision& collision);
    /// �X�N���[���̕`��I�t�Z�b�gX���擾
    int      scrollGetDrawOffsetX();
    /// ���[���h���W�n�ł̃X�N���[����`��Ԃ�
    sHitRect scrollGetWorldScreenRect();
};