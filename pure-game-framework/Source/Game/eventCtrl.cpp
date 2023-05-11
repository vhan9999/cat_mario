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
		if (player.shift_amount >= 600 && player.shift_amount <= 700 && !event_list["pipe_normal"]) {
			Enemy normal3 = Enemy(890, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			event_list["pipe_normal"] = true;
		}
		if (player.shift_amount >= 650 && player.shift_amount <= 750 && !event_list["up_seal_1"]) {
			Enemy up_seal_1(525, 600, { {"resources/image/enemy/seal.bmp"} }); up_seal_1.steel = false; up_seal_1.able_touch = false; up_seal_1.speed_y = -19; up_seal_1.step_enemy_player_dead = true; enemys_arr.push_back(up_seal_1);
			event_list["up_seal_1"] = true;
		}
		if (player.shift_amount >= 2000 && player.shift_amount <= 2100 && !event_list["two_normal_1"]) {
			Enemy normal3 = Enemy(890, 430, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			Enemy normal4 = Enemy(1090, 230, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal4.step_enemy_enemy_dead = true; normal4.speed_x = -2; enemys_arr.push_back(normal4);
			event_list["two_normal_1"] = true;
		}
		if (player.shift_amount >= 2780 && player.shift_amount <= 2880 && !event_list["down_seal_1"]) {
			Enemy down_seal_1(630, -100, { {"resources/image/enemy/seal_flip.bmp"} }); down_seal_1.steel = false; down_seal_1.able_touch = false; down_seal_1.speed_y = 19; down_seal_1.step_enemy_player_dead = true; enemys_arr.push_back(down_seal_1);
			event_list["down_seal_1"] = true;
		}
		if (player.shift_amount >= 2600 && player.shift_amount <= 2700 && !event_list["three_fall_bricksss"]) {
			//three fall
			Enemy normal3 = Enemy(1190, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			Enemy normal4 = Enemy(1330, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal4.step_enemy_enemy_dead = true; normal4.speed_x = -2; enemys_arr.push_back(normal4);
			Enemy turtle1 = Enemy(1470, 700, { "resources/image/enemy/turtle_open.bmp","resources/image/enemy/turtle_open_flip.bmp","resources/image/enemy/turtle_close.bmp","resources/image/enemy/turtle_close_flip.bmp" }); turtle1.coll.SetFrameIndexOfBitmap(1); turtle1.speed_x = 2; turtle1.turtle = true; enemys_arr.push_back(turtle1);
			for (auto &i : bricks_arr) {
				if (i.is_fall) {
					i.falling = true;
				}
			}
			event_list["three_fall_bricksss"] = true;
		}
		if (player.shift_amount >= 5122 && player.shift_amount <= 5222 && !event_list["two_normal_2"]) {
			Enemy normal3 = Enemy(990, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			Enemy normal4 = Enemy(1140, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal4.step_enemy_enemy_dead = true; normal4.speed_x = -2; enemys_arr.push_back(normal4);
			event_list["two_normal_2"] = true;
		}
		if (player.shift_amount >= 6284 && player.shift_amount <= 6384 && !event_list["fall_normals"]) {
			Enemy normal1 = Enemy(530, 0, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal1.step_enemy_enemy_dead = true; normal1.speed_x = -2; enemys_arr.push_back(normal1);
			Enemy normal2 = Enemy(590, 0, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal2.step_enemy_enemy_dead = true; normal2.speed_x = -2; enemys_arr.push_back(normal2);
			Enemy normal3 = Enemy(650, 0, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			Enemy normal4 = Enemy(710, 0, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal4.step_enemy_enemy_dead = true; normal4.speed_x = -2; enemys_arr.push_back(normal4);	
			event_list["fall_normals"] = true;
		}
		if (player.shift_amount >= 6555 && player.shift_amount <= 6655 && player.coll.GetTop() <= 200 && !event_list["yellow"]) {
			Enemy yellow1(1000, 50, { {"resources/image/enemy/yellow_comet_flip.bmp"} }); yellow1.steel = false; yellow1.able_touch = false; yellow1.speed_x = -30; yellow1.step_enemy_player_dead = true; enemys_arr.push_back(yellow1);
			event_list["yellow"] = true;
		}
		if (player.shift_amount >= 6900 && player.shift_amount <= 7000 && !event_list["down_seal_2"]) {
			Enemy down_seal_2(630, -100, { {"resources/image/enemy/seal_flip.bmp"} }); down_seal_2.steel = false; down_seal_2.able_touch = false; down_seal_2.speed_y = 19; down_seal_2.step_enemy_player_dead = true; enemys_arr.push_back(down_seal_2);
			event_list["down_seal_2"] = true;
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
			GotoGameState(GAME_STATE_OVER);

		}
	}
}
