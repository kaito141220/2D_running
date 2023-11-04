#include "GameManager.h"

Player		player;
Back		back;
Scroll		scroll;
Collision	collision;
Enemy		enemy;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Load(player, back, scroll, collision, enemy);
	Loop(player, back, scroll, collision, enemy);

	return 0;
}