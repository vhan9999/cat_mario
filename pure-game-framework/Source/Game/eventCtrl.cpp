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

void CGameStateRun::EventCtrl() {
	if (current_map == 1) {

	}
	if (player.isDead && !event_list["player_dead"]) {
		player.coll.UnshowBitmap();
		event_list["player_dead"] = true;
		Enemy dead_player(player.coll.GetLeft(), player.coll.GetTop(), { {"resources/image/player/player_dead.bmp"} }); dead_player.steel = true; dead_player.able_touch = false; dead_player.speed_y = -19; enemys_arr.push_back(dead_player);
	}
}
