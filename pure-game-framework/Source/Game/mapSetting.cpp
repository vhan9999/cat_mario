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

int mountain_height = 132;
int grass_height = 50;
int pipeline_big_height = 240;
int pipeline_mid_height = 180;
int pipeline_short_height = 120;
int checkpoint_flag_height = 120;
int endpoint_building_height = 180;
int pipeline_big_hor_height = 120;

// shift map image
void CGameStateRun::shiftMapImage() {
	if (player.coll.GetLeft() + player.coll.GetWidth() > 512) {
		int player_posX = 512 - player.coll.GetWidth();
		player.coll.SetTopLeft(player_posX, player.coll.GetTop());
		
		// shift image left
		for (auto &i : bricks_arr) {
			int obj_pos = i.coll.GetLeft() - player.moveSpeed;
			i.coll.SetTopLeft(obj_pos, i.coll.GetTop());
		}
		for (auto &i : enemys_arr) {
			int obj_pos = i.coll.GetLeft() - player.moveSpeed;
			i.coll.SetTopLeft(obj_pos, i.coll.GetTop());
		}
		for (auto &i : environment_arr) {
			int obj_pos = i.coll.GetLeft() - player.moveSpeed;
			i.coll.SetTopLeft(obj_pos, i.coll.GetTop());
		}
		if (animation_flag == true) {
			// coin 
			int coin_pos = coin_animation.GetLeft() - player.moveSpeed;
			coin_animation.SetTopLeft(coin_pos, coin_animation.GetTop());
			// interact pipe
			int pipe_pos = pipe_animation.GetLeft() - player.moveSpeed;
			pipe_animation.SetTopLeft(pipe_pos, pipe_animation.GetTop());
			// interact horiontal pipe
			int pipe_hor_pos = pipe_hor_animation.GetLeft() - player.moveSpeed;
			pipe_hor_animation.SetTopLeft(pipe_hor_pos, pipe_hor_animation.GetTop());
		}
		player.distance_count += player.moveSpeed;
	}
}

// high from ground
int high_from_ground(int blockCount) { return  776 - (60 * blockCount); }

// far from start
int far_from_start(int blockCount) { return 60 * blockCount; }

void CGameStateRun::MapSetting(){
	if (current_map == 1) {
		player.finish_point = 7200-90;
		event_list = { {"player_dead",false} , {"up_seal_1",false} , {"down_seal_1",false} , {"two_normal_1",false} , {"two_normal_2",false} , {"three_fall_bricksss",false} , {"down_seal_2",false} , {"yellow",false} , {"fall_normals",false} ,{"pipe_normal",false} , {"cloud_evil1",false} , {"map7-10",false} };

		Enemy normal1 = Enemy(600, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal1.step_enemy_enemy_dead = true; normal1.speed_x = -2; enemys_arr.push_back(normal1);
		Enemy normal2 = Enemy(920, 700, { "resources/image/enemy/normal.bmp","resources/image/enemy/normal_flip.bmp" }); normal2.step_enemy_enemy_dead = true; normal2.speed_x = -2; enemys_arr.push_back(normal2);
		
		int currentGroundBlock = 0; // track how many ground blocks were build
		Brick brick;
		Environment env;
		
		/* phase1 */
		for (int i = 0; i < 17; i++) {
			brick = Brick(far_from_start(i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		env = Environment(far_from_start(7), high_from_ground(9), { "resources/image/object/environment/cloud_eye.bmp" }); environment_arr.push_back(env);
		env = Environment(far_from_start(1), groundY_up - 132, { "resources/image/object/environment/mountain.bmp" }); environment_arr.push_back(env);

		// brick = Brick(far_from_start(9), high_from_ground(4), { "resources/image/object/block1/item_brick.bmp",  "resources/image/object/block1/brown_brick2.bmp" }); brick.have_coin = true; bricks_arr.push_back(brick);
		// brick = Brick(far_from_start(8), high_from_ground(4), { "resources/image/object/block1/invisible_brick.bmp", "resources/image/object/block1/brown_brick2.bmp" }); brick.invisible = true; bricks_arr.push_back(brick);

		for (int i = 0; i < 5; i++) {
			brick = Brick(far_from_start(9 + 3 + i), high_from_ground(4), { "resources/image/object/block1/brown_brick.bmp", "resources/image/object/block1/brick_break.bmp" });
			bricks_arr.push_back(brick);
		}
		brick = Brick(far_from_start(9 + 5), high_from_ground(4 + 3), { "resources/image/object/block1/item_brick.bmp",  "resources/image/object/block1/brown_brick2.bmp" }); brick.have_coin = true; bricks_arr.push_back(brick);

		/* phase2 */
		currentGroundBlock += 17;
		for (int i = 0; i < 15; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
		}

		brick = Brick(far_from_start(currentGroundBlock + 2), groundY_up - pipeline_mid_height+5, { "resources/image/object/block2/pipeline_mid.bmp" }); bricks_arr.push_back(brick);
		env = Environment(far_from_start(currentGroundBlock + 3), high_from_ground(10), { "resources/image/object/environment/cloud_eye.bmp" }); environment_arr.push_back(env);
		env = Environment(far_from_start(currentGroundBlock + 1), groundY_up - grass_height, { "resources/image/object/environment/grass.bmp" }); environment_arr.push_back(env);
		brick = Brick(far_from_start(currentGroundBlock + 10), groundY_up - pipeline_big_height+5, { "resources/image/object/block2/pipeline_big.bmp" }); brick.is_danger = true; bricks_arr.push_back(brick);

		/* phase3 */
		currentGroundBlock += 15;
		for (int i = 0; i < 9; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		brick = Brick(far_from_start(currentGroundBlock + 9), high_from_ground(4), { "resources/image/object/block1/invisible_brick.bmp","resources/image/object/block1/brown_brick2.bmp" }); brick.invisible = true;  bricks_arr.push_back(brick);
		env = Environment(far_from_start(currentGroundBlock + 2), groundY_up - mountain_height, { "resources/image/object/environment/mountain.bmp" }); environment_arr.push_back(env);

		/* phase4 */
		currentGroundBlock += 12;
		for (int i = 0; i < 11; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
		}

		env = Environment(far_from_start(currentGroundBlock + 3), groundY_up - grass_height, { "resources/image/object/environment/grass.bmp" }); environment_arr.push_back(env);

		for (int i = 0; i < 3; i++) {
			brick = Brick(far_from_start(currentGroundBlock + 3 + i), high_from_ground(4), { "resources/image/object/block1/brown_brick.bmp", "resources/image/object/block1/brick_break.bmp" });
			bricks_arr.push_back(brick);
		}

		for (int i = 0; i < 5; i++) {
			brick = Brick(far_from_start(currentGroundBlock + 6 + i), high_from_ground(7), { "resources/image/object/block1/brown_brick.bmp", "resources/image/object/block1/brick_break.bmp" });
			if (i <= 2)
				brick.is_fall = 1;
			bricks_arr.push_back(brick);
		}

		/* phase5 */
		currentGroundBlock += 14;
		for (int i = 0; i < 11; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
		}

		for (int i = 0; i < 3; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), high_from_ground(9), { "resources/image/object/block1/brown_brick.bmp", "resources/image/object/block1/brick_break.bmp" });
			bricks_arr.push_back(brick);
		}
		brick = Brick(far_from_start(currentGroundBlock + 2), high_from_ground(4), { "resources/image/object/block1/brown_brick.bmp", "resources/image/object/block1/brick_break.bmp" });
		for (int i = 0; i < 2; i++) {
			brick = Brick(far_from_start(currentGroundBlock + 9 + i), high_from_ground(4), { "resources/image/object/block1/brown_brick.bmp", "resources/image/object/block1/brick_break.bmp" });
			bricks_arr.push_back(brick);
		}

		env = Environment(far_from_start(currentGroundBlock + 7), high_from_ground(11), { "resources/image/object/environment/cloud_eye.bmp" }); environment_arr.push_back(env);
		if(player.reach_checkpoint == false){
			brick = Brick(far_from_start(currentGroundBlock + 9), high_from_ground(4) - checkpoint_flag_height, { "resources/image/object/environment/checkpoint_reached.bmp", "resources/image/object/environment/blank.bmp" }); bricks_arr.push_back(brick);
		}
		/* phase6 */
		currentGroundBlock += 11;
		for (int i = 0; i < 17; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); 
			if (i >= 4 && i <= 8) {
				brick.foot_touch_fall = true;
				brick.is_fall = 2;
			}
			bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); 
			if (i >= 4 && i <= 8)
				brick.is_fall = 2;
			bricks_arr.push_back(brick);
		}

		brick = Brick(far_from_start(currentGroundBlock + 3), high_from_ground(4), { "resources/image/object/block1/item_brick.bmp", "resources/image/object/block1/brown_brick2.bmp" }); brick.have_coin = true; bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 6), high_from_ground(4), { "resources/image/object/block1/item_brick.bmp", "resources/image/object/block1/brown_brick2.bmp" }); brick.have_coin = true; bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 9), high_from_ground(4), { "resources/image/object/block1/item_brick.bmp", "resources/image/object/block1/brown_brick2.bmp" }); brick.have_coin = true; bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 6), high_from_ground(8), { "resources/image/object/block1/item_brick.bmp", "resources/image/object/block1/brown_brick2.bmp" }); brick.have_coin = true; bricks_arr.push_back(brick);

		env = Environment(far_from_start(currentGroundBlock + 10), groundY_up - grass_height, { "resources/image/object/environment/grass.bmp" }); environment_arr.push_back(env);

		brick = Brick(far_from_start(currentGroundBlock + 14), high_from_ground(1), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick);
		for (int i = 0; i < 2; i++) {
			brick = Brick(far_from_start(currentGroundBlock + 15), high_from_ground(1 + i), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick);
		}
		for (int i = 0; i < 3; i++) {
			brick = Brick(far_from_start(currentGroundBlock + 16), high_from_ground(1 + i), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick);
		}

		
	}
	// map2
	else if(current_map == 2) {
		/* upper_phase1 */
		Brick brick;
		Environment env;
		for (int i = 0; i < 17; i++) {
			brick = Brick(far_from_start(i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		env = Environment(far_from_start(1), groundY_up - endpoint_building_height, { "resources/image/object/environment/end_point_building.bmp" }); environment_arr.push_back(env);
		brick = Brick(far_from_start(12)-10, groundY_up - pipeline_big_hor_height + 10, { "resources/image/object/block2/pipeline_big_hor.bmp" }); brick.is_danger = true; bricks_arr.push_back(brick);
		brick = Brick(far_from_start(14), groundY_up - pipeline_big_height, { "resources/image/object/block2/pipeline_big.bmp" }); brick.is_danger = false; bricks_arr.push_back(brick);
	}
	// map2 dungeon
	else if (current_map == 3) {
		/* phase1 */
		int currentGroundBlock = 0; // track how many ground blocks were build
		Brick brick;
		Environment env;

		env = Environment(0, 0, { "resources/image/object/environment/dungeon_background.bmp" }); environment_arr.push_back(env);
			// up 
		for (int i = 0; i < 13; i++) { brick = Brick(far_from_start(4+i), 0, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);}
			// down
		for (int i = 0; i < 17; i++) {
			brick = Brick(far_from_start(i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); 
			if (i == 2) { brick.foot_touch_fall = true; }
			bricks_arr.push_back(brick);

			brick = Brick(far_from_start(i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); 
			if (i == 2) { brick.foot_touch_fall = true; }
			bricks_arr.push_back(brick);
		}

		brick = Brick(far_from_start(2), high_from_ground(1), { "resources/image/object/block1/invisible_brick.bmp","resources/image/object/block1/green_brick2.bmp" }); brick.invisible = true;  bricks_arr.push_back(brick);

		for (int i = 0; i < 13; i++) {
			brick = Brick(0, i*60, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);
		}
		for (int i = 0; i < 4; i++) {
			brick = Brick(far_from_start(7+i), high_from_ground(4), { "resources/image/object/block1/item_brick.bmp",  "resources/image/object/block1/brown_brick2.bmp" }); brick.have_coin = true; bricks_arr.push_back(brick);
		}

		brick = Brick(far_from_start(14), groundY_up-60, { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick);

		for (int i = 0; i < 2; i++) {
			brick = Brick(far_from_start(16), high_from_ground(i+1), { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick);
		}

		/* phase 2 */
		currentGroundBlock += 17;
		env = Environment(far_from_start(currentGroundBlock), 0, { "resources/image/object/environment/dungeon_background.bmp" }); environment_arr.push_back(env);
			// up
		for (int i = 0; i < 17; i++) { brick = Brick(far_from_start(currentGroundBlock + i), 0, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
			// down
		for (int i = 0; i < 17; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); 
			if (i == 7) { brick.foot_touch_fall = true;}
			bricks_arr.push_back(brick);

			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); 
			if (i == 7) { brick.foot_touch_fall = true;}
			bricks_arr.push_back(brick);
		}

		for (int i = 0; i < 3; i++) { brick = Brick(far_from_start(currentGroundBlock+1), high_from_ground(i + 1), { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick);}
		for (int i = 0; i < 4; i++) { brick = Brick(far_from_start(currentGroundBlock + 3), high_from_ground(i + 1), { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick);}

		for (int i = 0; i < 4; i++) {
			brick = Brick(far_from_start(currentGroundBlock+4+i), high_from_ground(4), { "resources/image/object/block1/invisible_brick.bmp","resources/image/object/block1/green_brick2.bmp" }); brick.invisible = true;  bricks_arr.push_back(brick);
		}

		for (int i = 0; i < 4; i++) { brick = Brick(far_from_start(currentGroundBlock + 8), high_from_ground(i + 1), { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick);}
		for (int i = 0; i < 3; i++) { brick = Brick(far_from_start(currentGroundBlock + 10), high_from_ground(i + 1), { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 2; i++) { brick = Brick(far_from_start(currentGroundBlock + 12), high_from_ground(i + 1), { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick); }
		brick = Brick(far_from_start(currentGroundBlock + 14), high_from_ground(1), { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick);

		/* phase 3 */
		currentGroundBlock += 17;
		env = Environment(far_from_start(currentGroundBlock), 0, { "resources/image/object/environment/dungeon_background.bmp" }); environment_arr.push_back(env);
			// up
		for (int i = 0; i < 18; i++) { brick = Brick(far_from_start(currentGroundBlock + i), 0, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
			// down
		for (int i = 0; i < 9; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		for (int i = 0; i < 3; i++) {
			brick = Brick(far_from_start(currentGroundBlock + 9+3 + i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + 9+3 + i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		
		/* phase 4 */
		currentGroundBlock += 18;
		env = Environment(far_from_start(currentGroundBlock), 0, { "resources/image/object/environment/dungeon_background.bmp" }); environment_arr.push_back(env);

		for (int i = 0; i < 6; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		brick = Brick(far_from_start(currentGroundBlock + 2), groundY_up - pipeline_short_height, { "resources/image/object/block2/pipeline_short.bmp" }); bricks_arr.push_back(brick);
		for (int i = 0; i < 2; i++) {
			brick = Brick(far_from_start(currentGroundBlock + 8 + i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + 8 + i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		if (player.reach_checkpoint == false) {
			brick = Brick(far_from_start(currentGroundBlock + 9), high_from_ground(0) - checkpoint_flag_height, { "resources/image/object/environment/checkpoint_reached.bmp", "resources/image/object/environment/blank.bmp" }); bricks_arr.push_back(brick);
		}

		/* phase 5 */
		currentGroundBlock += 10;
		env = Environment(far_from_start(currentGroundBlock), 0, { "resources/image/object/environment/dungeon_background.bmp" }); environment_arr.push_back(env);

			// up
		for (int i = 0; i < 4; i++) { brick = Brick(far_from_start(currentGroundBlock + 2 + i), 0, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 6; i++) { brick = Brick(far_from_start(currentGroundBlock + 6 + i), 0, { "resources/image/object/block1/brown_brick.bmp" }); bricks_arr.push_back(brick); }
			// down
		for (int i = 0; i < 6; i++) {
			brick = Brick(far_from_start(currentGroundBlock + 3 + i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + 3 + i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		/* phase 6 */
		currentGroundBlock += 12;
		env = Environment(far_from_start(currentGroundBlock), 0, { "resources/image/object/environment/dungeon_background.bmp" }); environment_arr.push_back(env);

			// up
		brick = Brick(far_from_start(currentGroundBlock), 0, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);
		for (int i = 0; i < 8; i++) { brick = Brick(far_from_start(currentGroundBlock + 1 + i), 0, { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 6; i++) { brick = Brick(far_from_start(currentGroundBlock + 9), i*60, { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 6; i++) { brick = Brick(far_from_start(currentGroundBlock + 10 + i), 0, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
			// down
		for (int i = 0; i < 15; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		for (int i = 0; i < 3; i++) {
			brick = Brick(far_from_start(currentGroundBlock + 3+i), high_from_ground(8), { "resources/image/object/block1/item_brick.bmp",  "resources/image/object/block1/brown_brick2.bmp" }); brick.have_coin = true; bricks_arr.push_back(brick);
		}
		
		brick = Brick(far_from_start(currentGroundBlock+3), groundY_up-60, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);
		for (int i = 0; i < 2; i++) { brick = Brick(far_from_start(currentGroundBlock + 4), high_from_ground(1*(i+1)), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 3; i++) { brick = Brick(far_from_start(currentGroundBlock + 5), high_from_ground(1*(i+1)), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 5; i++) { brick = Brick(far_from_start(currentGroundBlock + 6), high_from_ground(1*(i+1)), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
		
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				brick = Brick(far_from_start(currentGroundBlock + 7+j), high_from_ground(1+i), { "resources/image/object/block1/green_brick4.bmp" }); bricks_arr.push_back(brick);
			}
		}
		for (int i = 0; i < 5; i++) { brick = Brick(far_from_start(currentGroundBlock + 11), high_from_ground(1 * (i + 1)), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 3; i++) { brick = Brick(far_from_start(currentGroundBlock + 12), high_from_ground(1 * (i + 1)), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 2; i++) { brick = Brick(far_from_start(currentGroundBlock + 13), high_from_ground(1 * (i + 1)), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
		brick = Brick(far_from_start(currentGroundBlock + 14), groundY_up - 60, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);

		/* phase 7 */
		currentGroundBlock += 15;
		env = Environment(far_from_start(currentGroundBlock), 0, { "resources/image/object/environment/dungeon_background.bmp" }); environment_arr.push_back(env);

			// up
		for (int i = 0; i < 15; i++) { brick = Brick(far_from_start(currentGroundBlock + i), 0, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
			// down
		for (int i = 0; i < 15; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		for (int i = 0; i < 3; i++) { brick = Brick(far_from_start(currentGroundBlock + 4 + i), high_from_ground(1), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
		brick = Brick(far_from_start(currentGroundBlock + 5), high_from_ground(2), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 11), groundY_up - pipeline_mid_height + 5, { "resources/image/object/block2/pipeline_mid.bmp" }); bricks_arr.push_back(brick);

		/* phase 8 */
			// up
		currentGroundBlock += 15;
		env = Environment(far_from_start(currentGroundBlock), 0, { "resources/image/object/environment/dungeon_background.bmp" }); environment_arr.push_back(env);

		for (int i = 0; i < 3; i++) { brick = Brick(far_from_start(currentGroundBlock + i), 0, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
			// down
		for (int i = 0; i < 3; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/green_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/green_brick5.bmp" }); bricks_arr.push_back(brick);
		}

		for (int i = 0; i < 3; i++) { brick = Brick(far_from_start(currentGroundBlock + i), high_from_ground(1), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);}
		for (int i = 0; i < 2; i++) { brick = Brick(far_from_start(currentGroundBlock + i+1), high_from_ground(2), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
		brick = Brick(far_from_start(currentGroundBlock + 2), high_from_ground(3), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);
		
		brick = Brick(far_from_start(currentGroundBlock + 5), high_from_ground(6), { "resources/image/object/block1/yellow_brick.bmp" }); brick.foot_touch_fall = true; bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 11), high_from_ground(4), { "resources/image/object/block1/yellow_brick.bmp" }); brick.foot_touch_fall = true; bricks_arr.push_back(brick);


		/* phase 9 */
		currentGroundBlock += 16;
		env = Environment(far_from_start(currentGroundBlock), 0, { "resources/image/object/environment/dungeon_background.bmp" }); environment_arr.push_back(env);

			// up
		for (int i = 0; i < 8; i++) { brick = Brick(far_from_start(currentGroundBlock + i), 0, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick); }
			// down
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				brick = Brick(far_from_start(currentGroundBlock+j), 600+(60*i), { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);
			}
		}
		brick = Brick(far_from_start(currentGroundBlock+5), 600 - pipeline_big_height + 5, { "resources/image/object/block2/pipeline_big.bmp" }); bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 5)+10, 600, { "resources/image/object/environment/green_rect_hor.bmp" }); bricks_arr.push_back(brick);
		

		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 8; j++) {
				brick = Brick(far_from_start(currentGroundBlock + 8 + j), i*60, { "resources/image/object/block1/green_brick.bmp" }); bricks_arr.push_back(brick);
			}
		}
		brick = Brick(far_from_start(currentGroundBlock + 8), 600, { "resources/image/object/environment/green_rect_hor.bmp" }); bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 12), 600, { "resources/image/object/environment/green_rect_hor.bmp" }); bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 14), 600-pipeline_big_height, { "resources/image/object/environment/green_rect_ver.bmp" }); bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 14), 600 - (pipeline_big_height*2), { "resources/image/object/environment/green_rect_ver.bmp" }); bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock + 14), 600 - (pipeline_big_height * 3), { "resources/image/object/environment/green_rect_ver.bmp" }); bricks_arr.push_back(brick);
}
	else if (current_map == 4) {
		player.finish_point = 1531-90;

		// upper map2
		int currentGroundBlock = 0; // track how many ground blocks were build
		Brick brick;
		Environment env;

		// phase 2
		for (int i = 0; i < 14; i++) {
			brick = Brick(far_from_start(i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
		}

		brick = Brick(far_from_start(2), groundY_up - pipeline_short_height + 5, { "resources/image/object/block2/pipeline_short.bmp" }); bricks_arr.push_back(brick);
		env = Environment(far_from_start(1), high_from_ground(12), { "resources/image/object/environment/cloud_eye.bmp" }); environment_arr.push_back(env);

		for (int i = 0; i < 9; i++) { brick = Brick(far_from_start(5+i), high_from_ground(1), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick);}
		for(int i=0; i<8; i++) { brick = Brick(far_from_start(6 + i), high_from_ground(2), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 7; i++) { brick = Brick(far_from_start(7 + i), high_from_ground(3), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 6; i++) { brick = Brick(far_from_start(8 + i), high_from_ground(4), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 5; i++) { brick = Brick(far_from_start(9 + i), high_from_ground(5), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 4; i++) { brick = Brick(far_from_start(10 + i), high_from_ground(6), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 3; i++) { brick = Brick(far_from_start(11 + i), high_from_ground(7), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick); }
		for (int i = 0; i < 2; i++) { brick = Brick(far_from_start(12 + i), high_from_ground(8), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick); }

		// phase 3 
		currentGroundBlock += 14;
		for (int i = 0; i < 13; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		for (int i = 0; i < 7; i++) { brick = Brick(far_from_start(currentGroundBlock + i), high_from_ground(1), { "resources/image/object/block1/brown_brick4.bmp" }); bricks_arr.push_back(brick); }
		brick = Brick(far_from_start(currentGroundBlock + 12)+20, groundY_up-560-60, { "resources/image/object/environment/end_point_flag.bmp" }); bricks_arr.push_back(brick);
		brick = Brick(far_from_start(currentGroundBlock+12), groundY_up-60, { "resources/image/object/block1/brown_brick.bmp" }); bricks_arr.push_back(brick);

		// phase 4
		currentGroundBlock += 13;
		for (int i = 0; i < 16; i++) {
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_up, { "resources/image/object/block1/brown_brick3.bmp" }); bricks_arr.push_back(brick);
			brick = Brick(far_from_start(currentGroundBlock + i), groundY_down, { "resources/image/object/block1/brown_brick5.bmp" }); bricks_arr.push_back(brick);
		}
		env = Environment(far_from_start(currentGroundBlock + 5), groundY_up - endpoint_building_height, { "resources/image/object/environment/end_point_building.bmp" }); environment_arr.push_back(env);
		env = Environment(far_from_start(currentGroundBlock + 11), groundY_up - grass_height, { "resources/image/object/environment/grass.bmp" }); environment_arr.push_back(env);
		env = Environment(far_from_start(currentGroundBlock + 12), high_from_ground(12), { "resources/image/object/environment/cloud_eye.bmp" }); environment_arr.push_back(env);
	}
}