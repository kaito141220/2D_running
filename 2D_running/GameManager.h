#pragma once

#include "DxLib.h"
#include "Player.h"
#include "Back.h"
#include "Collision.h"
#include "scrollManager.h"
#include "Enemy.h"

void Load(Player& player, Back& back, Scroll& scroll, 
            Collision& collision, Enemy& enemy);

void Loop(Player& player, Back& back, Scroll& scroll, 
            Collision& collision, Enemy& enemy);