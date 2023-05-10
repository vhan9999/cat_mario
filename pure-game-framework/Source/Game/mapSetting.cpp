#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "colliders.h"

using namespace game_framework;

void CGameStateRun::MapSetting(){
	if (current_map == 1) {
		event_list = { {"player_dead",false} };
		for (int i = 0; i < 10; i++) {
			Brick a = Brick(i*60, 800, { "resources/image/object/block1/brown_brick.bmp" }); a.foot_touch_break = true; bricks_arr.push_back(a);
		}
		Enemy b = Enemy(0, 500, { "resources/image/enemy/normal.bmp" }); b.step_enemy_enemy_dead = true; b.speed_x = 2; enemys_arr.push_back(b);
		
	}
	else if(current_map == 2) {

	}
	else if (current_map == 3) {

	}
	else if (current_map == 4) {

	}
}