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
		if (player.coll.GetLeft() >= 350 && !event_list["up_seal_1"]) {
			Enemy up_seal_1(400, 1000, { {"resources/image/enemy/seal.bmp"} }); up_seal_1.steel = false; up_seal_1.able_touch = false; up_seal_1.speed_y = -19; up_seal_1.step_enemy_player_dead = true; enemys_arr.push_back(up_seal_1);
			event_list["up_seal_1"] = true;
		}
	}
	if (player.isDead) {
		if (!event_list["player_dead"]) {
			Enemy dead_player(player.coll.GetLeft(), player.coll.GetTop(), { {"resources/image/player/player_dead.bmp"} }); dead_player.steel = true; dead_player.able_touch = false; dead_player.speed_y = -19; enemys_arr.push_back(dead_player);
			player.coll.UnshowBitmap();
			event_list["player_dead"] = true;
		}
		player.dead_frame++;
		if (player.dead_frame >= 100) {
			player.map_audio->Stop(0);
			GotoGameState(GAME_STATE_OVER);
		}
	}
}
