#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "colliders.h"
#include "string"

using namespace game_framework;


void CGameStateRun::EventCtrl() {
	if (current_map == 1) {
		if (player.distance_count >= 314 && player.distance_count <= 535 && !event_list["pipe_normal"]) {
			Enemy normal3 = Enemy(1072, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			event_list["pipe_normal"] = true;
		}
		if (player.distance_count >= 650 && player.distance_count <= 750 && !event_list["up_seal_1"]) {
			Enemy up_seal_1(520, 600, { {"resources/image/enemy/seal.bmp"} }); up_seal_1.steel = false; up_seal_1.able_touch = false; up_seal_1.speed_y = -19; up_seal_1.step_enemy_player_dead = true; enemys_arr.push_back(up_seal_1);
			event_list["up_seal_1"] = true;
		}
		if (player.distance_count >= 1900 && player.distance_count <= 1960 && !event_list["two_normal_1"]) {
			Enemy normal3 = Enemy(1080, 430, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			Enemy normal4 = Enemy(1150, 230, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal4.step_enemy_enemy_dead = true; normal4.speed_x = -2; enemys_arr.push_back(normal4);
			event_list["two_normal_1"] = true;
		}
		if (player.distance_count >= 2790 && player.distance_count <= 2880 && !event_list["down_seal_1"]) {
			Enemy down_seal_1(630, -100, { {"resources/image/enemy/seal_flip.bmp"} }); down_seal_1.steel = false; down_seal_1.able_touch = false; down_seal_1.speed_y = 19; down_seal_1.step_enemy_player_dead = true; enemys_arr.push_back(down_seal_1);
			event_list["down_seal_1"] = true;
		}
		if (player.distance_count >= 2580 && player.distance_count <= 2656 && !event_list["three_fall_bricksss"]) {
			//three fall
			Enemy normal3 = Enemy(1190, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			Enemy normal4 = Enemy(1300, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal4.step_enemy_enemy_dead = true; normal4.speed_x = -2; enemys_arr.push_back(normal4);
			Enemy turtle1 = Enemy(1400, 700, { "resources/image/enemy/turtle_open.bmp","resources/image/enemy/turtle_open_flip.bmp","resources/image/enemy/turtle_close.bmp","resources/image/enemy/turtle_close_flip.bmp" }); turtle1.coll.SetFrameIndexOfBitmap(1); turtle1.speed_x = 2; turtle1.turtle = true; enemys_arr.push_back(turtle1);
			for (auto &i : bricks_arr) {
				if (i.is_fall) {
					i.falling = true;
					i.speed_y = 5;
				}
			}
			event_list["three_fall_bricksss"] = true;
		}
		if (player.distance_count >= 3628 && player.distance_count <= 3728 && !event_list["map7-10"]) {
			int mountain_height = 132;
			int grass_height = 50;
			int pipeline_big_height = 240;
			int pipeline_mid_height = 180;
			int pipeline_short_height = 120;
			int checkpoint_flag_height = 120;
			int endpoint_building_height = 180;
			Brick brick;
			Environment env;
			int ground = 776;
			/* phase7 */
			int startBlock = 1020+player.distance_count-2968;
			
			for (int i = 0; i < 6; i++) {
				brick = Brick(startBlock + (i * 60), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
				brick = Brick(startBlock + (i * 60), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
			}
			for (int i = 0; i < 5; i++) {
				brick = Brick(startBlock+(i * 60)+120, 776-(60*3), { "resources/image/object/block1/invisible_brick.bmp","resources/image/object/block1/brown_brick2.bmp" }); brick.invisible = true;  bricks_arr.push_back(brick);
			}
			brick = Brick(startBlock + 60, 776 - (60 * 7), { "resources/image/object/block1/invisible_brick.bmp","resources/image/object/block1/brown_brick2.bmp" }); brick.invisible = true;  bricks_arr.push_back(brick);
			for (int i = 0; i < 3; i++) {
				brick = Brick(startBlock, 776 - (60 * (1 + i)), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick);
			}
			for (int i = 0; i < 2; i++) {
				brick = Brick(startBlock + 60, 776 - (60 * (1 + i)), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick);
			}

			/* phase 8 */
			startBlock += 420;
			for (int i = 0; i < 13; i++) {
				brick = Brick(startBlock + (i * 60), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
				brick = Brick(startBlock + (i * 60), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
			}

			brick = Brick(startBlock, groundY_up - pipeline_mid_height, { "resources/image/object/block2/pipeline_mid.bmp" }); bricks_arr.push_back(brick);

			for (int i = 0; i < 2; i++) {
				brick = Brick(startBlock + (i * 60)+240, 776 - (60 * 4), { "resources/image/object/block1/brown_brick.bmp", "resources/image/object/block1/brick_break.bmp" });
				bricks_arr.push_back(brick);
			}
			brick = Brick(startBlock + 420, 776 - (60 * 4), { "resources/image/object/block1/brown_brick.bmp", "resources/image/object/block1/brick_break.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(startBlock + 660, groundY_up - pipeline_short_height, { "resources/image/object/block2/pipeline_short.bmp" }); bricks_arr.push_back(brick);
			/* phase 9 */
			startBlock += 780;
			for (int i = 0; i < 9; i++) {
				brick = Brick(startBlock + (i * 60), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
				brick = Brick(startBlock + (i * 60), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
			}
			for (int i = 0; i < 9; i++) {
				brick = Brick(startBlock + (i * 60), 776 - (60 * 1), { "resources/image/object/block1/brown_brick4.bmp" });
				bricks_arr.push_back(brick);
			}
			for (int i = 0; i < 8; i++) {
				brick = Brick(startBlock + (i * 60)+60, 776 - (60 * 2), { "resources/image/object/block1/brown_brick4.bmp" });
				bricks_arr.push_back(brick);

			}
			for (int i = 0; i < 7; i++) {
				brick = Brick(startBlock + (i * 60) + 120, 776 - (60 * 3), { "resources/image/object/block1/brown_brick4.bmp" });
				bricks_arr.push_back(brick);
			}
			for (int i = 0; i < 6; i++) {
				brick = Brick(startBlock + (i * 60) + 180, 776 - (60 * 4), { "resources/image/object/block1/brown_brick4.bmp" });
				bricks_arr.push_back(brick);
			}
			for (int i = 0; i < 5; i++) {
				brick = Brick(startBlock + (i * 60) + 240, 776 - (60 * 5), { "resources/image/object/block1/brown_brick4.bmp" });
				bricks_arr.push_back(brick);
			}
			for (int i = 0; i < 4; i++) {
				brick = Brick(startBlock + (i * 60) + 300, 776 - (60 * 6), { "resources/image/object/block1/brown_brick4.bmp" });
				bricks_arr.push_back(brick);
			}
			for (int i = 0; i < 3; i++) {
				brick = Brick(startBlock + (i * 60) + 360, 776 - (60 * 7), { "resources/image/object/block1/brown_brick4.bmp" });
				bricks_arr.push_back(brick);
			}
			for (int i = 0; i < 2; i++) {
				brick = Brick(startBlock + (i * 60) + 420, 776 - (60 * 8), { "resources/image/object/block1/brown_brick4.bmp" });
				bricks_arr.push_back(brick);
			}
			/* phase 10 */
			startBlock += 540;
			for (int i = 0; i < 7; i++) {
				brick = Brick(startBlock + (i * 60), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
				brick = Brick(startBlock + (i * 60), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
			}
			brick = Brick(startBlock + 240, 776 - (60 * 1), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick);
			// endpoint flag
			brick = Brick(startBlock + 240 + 20, 776 - (60 * 10) - 20, { "resources/image/object/environment/end_point_flag.bmp" }); bricks_arr.push_back(brick);

			/* phase 11 */
			startBlock += 420;
			for (int i = 0; i < 10; i++) {
				brick = Brick(startBlock + (i * 60), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
				brick = Brick(startBlock + (i * 60), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
			}
			env = Environment(startBlock - 120, groundY_up - grass_height, { "resources/image/object/environment/grass.bmp" }); environment_arr.push_back(env);
			env = Environment(startBlock + 120, groundY_up - endpoint_building_height, { "resources/image/object/environment/end_point_building.bmp" }); environment_arr.push_back(env);
			event_list["map7-10"] = true;
		}
		if (player.distance_count >= 4800 && player.distance_count <= 4889 && !event_list["two_normal_2"]) {
			Enemy normal3 = Enemy(1100, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			Enemy normal4 = Enemy(1300, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal4.step_enemy_enemy_dead = true; normal4.speed_x = -2; enemys_arr.push_back(normal4);
			Enemy cloud = Enemy(1500, 300, { "resources/image/enemy/cloud_eye.bmp" ,"resources/image/enemy/cloud_devil.bmp" }); cloud.cloud = true; cloud.steel = false; cloud.step_enemy_player_dead = true; enemys_arr.push_back(cloud);

			event_list["two_normal_2"] = true;
			event_list["cloud_evil1"] = true;
		}
		if (player.distance_count >= 6284 && player.distance_count <= 6384 && !event_list["fall_normals"]) {
			Enemy normal1 = Enemy(530, 0, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal1.step_enemy_enemy_dead = true; normal1.speed_x = -2; enemys_arr.push_back(normal1);
			Enemy normal2 = Enemy(590, 0, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal2.step_enemy_enemy_dead = true; normal2.speed_x = -2; enemys_arr.push_back(normal2);
			Enemy normal3 = Enemy(650, 0, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal3.step_enemy_enemy_dead = true; normal3.speed_x = -2; enemys_arr.push_back(normal3);
			Enemy normal4 = Enemy(710, 0, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal4.step_enemy_enemy_dead = true; normal4.speed_x = -2; enemys_arr.push_back(normal4);	
			event_list["fall_normals"] = true;
		}
		if (player.distance_count >= 6555 && player.distance_count <= 6655 && player.coll.GetTop() <= 200 && !event_list["yellow"]) {
			Enemy yellow1(1000, 50, { {"resources/image/enemy/yellow_comet_flip.bmp"} }); yellow1.steel = false; yellow1.able_touch = false; yellow1.speed_x = -30; yellow1.step_enemy_player_dead = true; enemys_arr.push_back(yellow1);
			event_list["yellow"] = true;
		}
		if (player.distance_count >= 6900 && player.distance_count <= 7000 && !event_list["down_seal_2"]) {
			Enemy down_seal_2(600, -100, { {"resources/image/enemy/seal_flip.bmp"} }); down_seal_2.steel = false; down_seal_2.able_touch = false; down_seal_2.speed_y = 17; down_seal_2.step_enemy_player_dead = true; enemys_arr.push_back(down_seal_2);
			event_list["down_seal_2"] = true;
		}
	}
	if (player.isDead) {
		if (!event_list["player_dead"]) {
			Enemy dead_player(player.coll.GetLeft(), player.coll.GetTop(), { {"resources/image/player/player_dead.bmp"} }); dead_player.steel = true; dead_player.able_touch = false; dead_player.speed_y = -19; enemys_arr.push_back(dead_player);
			player.coll.UnshowBitmap();
			event_list["player_dead"] = true;
			event_list["map7-10"] = false;
		}
		player.dead_frame++;
		if (player.dead_frame >= 100) {
			player.map_audio->Stop(0);
			player.dungeon_audio->Stop(6);
			player.game_over_audio->Play(2);
			GotoGameState(GAME_STATE_OVER);
		}
	}
	// player hit the ending flag
	if (player.isFinish == true) {
		player.coll.SetFrameIndexOfBitmap(4);
		player.moveSpeed = 0; player.jumpSpeed = 0;
		player.jumpSpeed = 5;
		if (player.player_on_air == false) {
			player.coll.SetFrameIndexOfBitmap(0);
			player.coll.SetTopLeft(player.coll.GetLeft()+90, groundY_up-player.coll.GetHeight());
			player.moveSpeed = 2;
			if (player.isDead == true) {
				player.moveSpeed = 0;
				player.player_finish_audio->Stop(5);
			}
			if(player.distance_count >= player.finish_point-2){
				player.coll.SetFrameIndexOfBitmap(6);
			}
			if (player.distance_count >= player.finish_point) { // player meet end point
				player.moveSpeed = 0;
				current_map++;
				if (current_map == 5) {
					current_map = 1;
				}
				player.isEnd = true;
				GotoGameState(GAME_STATE_OVER);
			}
		}
	}

	//delete old
	for (std::vector<Enemy>::iterator it = enemys_arr.begin(); it != enemys_arr.end();) {
		if ((*it).coll.GetLeft() < -1000 || (*it).coll.GetTop() > 1300 || (*it).coll.GetTop() < -200) {
			it = enemys_arr.erase(it);
		}
		else {
			it++;
		}
	}
	for (std::vector<Brick>::iterator it = bricks_arr.begin(); it != bricks_arr.end();) {
		if ((*it).coll.GetLeft() < -1000 || (*it).coll.GetTop() > 1300 || (*it).coll.GetTop() < -200) {
			it = bricks_arr.erase(it);
		}
		else {
			it++;
		}
	}
}
