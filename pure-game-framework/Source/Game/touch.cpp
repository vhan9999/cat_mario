#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "colliders.h"
#include "string.h"

using namespace game_framework;

bool inRange(double num, double min, double max) {
	return (min <= num && num <= max);
}

/*
	*** NOTE ***
	game map phase image are all in resource/game_map
*/

bool isDanger = false;
void CGameStateRun::Touching() {
	CMovingBitmap &PC = player.coll;
	//big player
	if (player.coll.GetFrameIndexOfBitmap() == 7 || player.coll.GetFrameIndexOfBitmap() == 8) {
		for (std::vector<Brick>::iterator brick = bricks_arr.begin(); brick != bricks_arr.end();) {
			if (CMovingBitmap::IsOverlap((*brick).coll, player.coll)) {
				brick = bricks_arr.erase(brick);
			}
			else {
				brick++;
			}
		}
	}
	for (std::vector<Enemy>::iterator enemy = enemys_arr.begin(); enemy != enemys_arr.end(); enemy++) {
		if ((*enemy).coll.GetImageFileName() == "resources/image/enemy/big_normal.bmp" || (*enemy).coll.GetImageFileName() == "resources/image/enemy/big_normal_flip.bmp") {
			for (std::vector<Brick>::iterator brick = bricks_arr.begin(); brick != bricks_arr.end();) {
				if (CMovingBitmap::IsOverlap((*brick).coll, (*enemy).coll)) {
					brick = bricks_arr.erase(brick);
				}
				else {
					brick++;
				}
			}
		}
	}
	
	//player&bricks touch
	for (auto &i : bricks_arr) {
		CMovingBitmap &BC = i.coll;

		if (player.jumpSpeed >= 28) {//predict penetrate
			if (PC.GetTop() + PC.GetHeight() <= BC.GetTop() && PC.GetTop() + PC.GetHeight() + player.jumpSpeed >= BC.GetTop() && PC.GetLeft() + 2 < BC.GetLeft() + BC.GetWidth() && PC.GetLeft() + PC.GetWidth() - 2 > BC.GetLeft()) {
				player.jumpSpeed = 0;
				player.jumpBonusFrame = 0;
				PC.SetTopLeft(PC.GetLeft(), BC.GetTop() - PC.GetHeight());
				double ground = BC.GetTop() - PC.GetHeight();
				player.ableToJump(ground); // can jump on block
			}
		}
		if (i.speed_y >= 15) {//predict penetrate
			if (BC.GetTop() + BC.GetHeight() <= PC.GetTop() && BC.GetTop() + BC.GetHeight() + i.speed_y >= PC.GetTop() && BC.GetLeft() + 2 < PC.GetLeft() + PC.GetWidth() && BC.GetLeft() + BC.GetWidth() - 2 > PC.GetLeft()) {
				player.isDead = true;
			}
		}
		if (i.falling && cheat_mode==false) {
			if (inRange(PC.GetTop() + PC.GetHeight() + 1, BC.GetTop() - 6, BC.GetTop() + (BC.GetHeight() / 2)) && PC.GetLeft() + 2 < BC.GetLeft() + BC.GetWidth() && PC.GetLeft() + PC.GetWidth() - 2 > BC.GetLeft()) {
				player.jumpSpeed = i.speed_y;
				player.jumpBonusFrame = 0;
				PC.SetTopLeft(PC.GetLeft(), BC.GetTop() - PC.GetHeight());
				double ground = BC.GetTop() - PC.GetHeight();
				player.ableToJump(ground); // can jump on block

			}
		}
		else if (CMovingBitmap::IsOverlap(PC, BC)) {//check touch
			int obj_left = BC.GetLeft();
			int obj_right = BC.GetLeft() + BC.GetWidth();
			int obj_top = BC.GetTop();
			int obj_bottom = BC.GetTop() + BC.GetHeight();
			int obj_mid_x = BC.GetLeft() + (BC.GetWidth() / 2);
			int obj_mid_y = BC.GetTop() + (BC.GetHeight() / 2);
			//head touch
			if (inRange(PC.GetTop() - 1, obj_mid_y, obj_bottom) && PC.GetLeft() + 8 <= obj_right && PC.GetLeft() + PC.GetWidth() - 8 >= obj_left) {
				// invisible
				if ((i.invisible == true) && (cheat_mode == true)) { continue; }
				if ((i.invisible == true) && (i.coll.GetImageFileName() != "resources/image/object/block1/brick_break.bmp") && (cheat_mode == false)) {
					if (player.jumpSpeed > 0)
						continue;
					i.invisible = false;
					i.coll.SetFrameIndexOfBitmap(1);
				}
				if (i.falling && cheat_mode == false) {
					player.isDead = true;
				}
				// item brick
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/item_brick.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/brown_brick2.bmp") && (i.coll.GetFrameSizeOfBitmap() == 2)) {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						if (i.have_coin == true) {
							player.coin_item_brick_audio->Play(3, false);

							// enable coin animation
							coin_animation.SetFrameIndexOfBitmap(0);
							coin_animation.SetTopLeft(i.coll.GetLeft(), i.coll.GetTop() - 168);
							animation_flag = true;
							coin_animation_flag = true;
						}
						else if (i.item.compare("") != 0) {
							if (i.item.compare("red_mushroom_big") == 0) {
								Enemy mushroom = Enemy(i.coll.GetLeft(), i.coll.GetTop() - 60, { "resources/image/items/red_mushroom.bmp","resources/image/items/red_mushroom.bmp","resources/image/object/block1/brick_break.bmp" }); mushroom.big_mushroom = true; mushroom.speed_x = 3; 
								enemys_arr.push_back(mushroom);
							}
							else if (i.item.compare("purple_mushroom") == 0) {
								Enemy mushroom = Enemy(i.coll.GetLeft(), i.coll.GetTop() - 60, { "resources/image/items/purple_mushroom.bmp","resources/image/items/purple_mushroom.bmp" }); mushroom.dead_mushroom = true; mushroom.speed_x = 3;
								enemys_arr.push_back(mushroom);
							}
						}
					}
				}
				// invisible item block 
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/invisible_brick.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/item_brick.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/brown_brick2.bmp") && (i.coll.GetFrameSizeOfBitmap() == 3)) {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
					}
					if (i.coll.GetFrameIndexOfBitmap() == 1) {
						if (i.item.compare("") != 0) {
							if (i.item.compare("red_mushroom_big") == 0) {
								Enemy mushroom = Enemy(i.coll.GetLeft(), i.coll.GetTop() - 60, { "resources/image/items/red_mushroom.bmp","resources/image/items/red_mushroom.bmp","resources/image/object/block1/brick_break.bmp" }); mushroom.dead_mushroom = true; mushroom.speed_x = 3;
								enemys_arr.push_back(mushroom);
							}
							else if (i.item.compare("purple_mushroom") == 0) {
								Enemy mushroom = Enemy(i.coll.GetLeft(), i.coll.GetTop() - 60, { "resources/image/items/purple_mushroom.bmp","resources/image/items/purple_mushroom.bmp" }); mushroom.dead_mushroom = true; mushroom.speed_x = 3;
								enemys_arr.push_back(mushroom);
							}
						}
						i.coll.SetFrameIndexOfBitmap(2);
					}
					if (i.coll.GetFrameIndexOfBitmap() == 2) {
						i.invisible = false;
					}
				}
				// head touch break
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/brown_brick.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/brick_break.bmp")) {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						i.invisible = true;
						// player.player_break_brick->Play(2, false);
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) {
						continue;
					}
				}
				// checkpoint
				if (i.coll.GetImageFileName() == "resources/image/object/environment/checkpoint_reached.bmp" || i.coll.GetImageFileName() == "resources/image/object/environment/blank.bmp") {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						if (current_map == 1) { player.shift_amount = 3600; }
						else if (current_map == 3) { player.shift_amount = 3157; }
						player.reach_checkpoint = true;
						player.current_checkpoint_x = 120;
						player.current_checkpoint_y = groundY_up - player.coll.GetHeight();
						return;
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) {
						return;
					}
				}
				// brick4 trap
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/green_brick4_trap1.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/green_brick4_trap2.bmp") && (cheat_mode == false)) {
					i.coll.SetFrameIndexOfBitmap(1);
					player.isDead = true;
					return;
				}
				// item brick trap
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/item_brick_trap1.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/item_brick_trap2.bmp") && (cheat_mode == false)) {
					i.coll.SetFrameIndexOfBitmap(1);
					player.isDead = true;
					return;
				}
				player.jumpSpeed = 0;
				PC.SetTopLeft(PC.GetLeft(), obj_bottom);
				player.jumpSpeed += 1;
			}
			else if (i.invisible)
				continue;
			//foot touch
			else if (inRange(PC.GetTop() + PC.GetHeight() +1, obj_top, obj_top+30) && PC.GetLeft() + 2 < obj_right && PC.GetLeft() + PC.GetWidth() - 2 > obj_left) {
				if (player.keyDown == true && i.coll.GetImageFileName() == "resources/image/object/block2/pipeline_big.bmp") {
					player.coll.SetFrameIndexOfBitmap(6);
					player.moveSpeed = 0;
					// enable animation
					player.pipe_interact_audio->Play(4, false);
					pipe_animation.SetFrameIndexOfBitmap(0);
					player.coll.SetTopLeft(i.coll.GetLeft() + 30, i.coll.GetTop() - 68);
					pipe_animation.SetTopLeft(i.coll.GetLeft() + 30, i.coll.GetTop() - 68);
					animation_flag = true;
					pipe_animation_flag = true;
					isDanger = i.is_danger;
					return;
				}
				if (i.foot_touch_fall && cheat_mode == false) {
					for (auto &j : bricks_arr) {
						if (j.is_fall == i.is_fall)
							j.falling = true;
					}
					i.falling = true;
				}
				/*if (i.foot_touch_break && i.coll.GetImageFileName() != "resources/image/object/block1/invisible_brick.bmp" && i.coll.GetImageFileName() != "resources/image/object/block1/brick_break.bmp") {
					i.foot_touch_break = false;
					i.coll.SetFrameIndexOfBitmap(2);
					i.invisible = true;
				}*/
				// checkpoint
				if (i.coll.GetImageFileName() == "resources/image/object/environment/checkpoint_reached.bmp" || i.coll.GetImageFileName() == "resources/image/object/environment/blank.bmp") {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						if (current_map == 1) { player.shift_amount = 3600; }
						else if (current_map == 3) { player.shift_amount = 3157; }
						player.reach_checkpoint = true;
						player.current_checkpoint_x = 120;
						player.current_checkpoint_y = groundY_up - player.coll.GetHeight();
						return;
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) {
						return;
					}
				}
				// brick4 trap
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/green_brick4_trap1.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/green_brick4_trap2.bmp") && (cheat_mode == false)) {
					i.coll.SetFrameIndexOfBitmap(1);
					player.isDead = true;
					return;
				}
				// question brick
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/question_block.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/invisible_brick.bmp")) {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						i.invisible = true;
						event_list["boolean_question"] = true;
						return;
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) {
						return;
					}
				}
				player.jumpSpeed = 0;
				player.jumpBonusFrame = 0;
				PC.SetTopLeft(PC.GetLeft(), obj_top - PC.GetHeight());
				double ground = obj_top - PC.GetHeight();
				player.ableToJump(ground); // can jump on block
			}
			//left touch
			else if (inRange(PC.GetLeft(), obj_mid_x, obj_right) && PC.GetTop() <= obj_bottom && PC.GetTop() + PC.GetHeight() - 5 >= obj_top) {
				// end point flag
				if (i.coll.GetImageFileName() == "resources/image/object/environment/end_point_flag.bmp" || i.coll.GetImageFileName() == "resources/image/object/environment/end_point_flag_blank.bmp") {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						if (player.isDead == true) {
							player.moveSpeed = 0;
							player.player_finish_audio->Stop(5);
						}
						player.coll.SetTopLeft(i.coll.GetLeft() - player.coll.GetWidth(), i.coll.GetTop());
						player.jumpSpeed = 0;
						animation_flag = true;
						player.isFinish = true;
						i.invisible = true;
						return;
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) { return; }
				}
				// checkpoint
				if (i.coll.GetImageFileName() == "resources/image/object/environment/checkpoint_reached.bmp" || i.coll.GetImageFileName() == "resources/image/object/environment/blank.bmp") {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						if (current_map == 1) { 
							if (player.moveSpeed > -10) { player.moveSpeed = -10; }
							player.shift_amount = 3600; 
						}
						else if (current_map == 3) { player.shift_amount = 3157; }
						player.reach_checkpoint = true;
						player.current_checkpoint_x = 120;
						player.current_checkpoint_y = groundY_up - player.coll.GetHeight();
						return;
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) {
						return;
					}
				}
				// brick4 trap
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/green_brick4_trap1.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/green_brick4_trap2.bmp") && (cheat_mode == false)) {
					i.coll.SetFrameIndexOfBitmap(1);
					player.isDead = true;
					return;
				}
				// question brick
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/question_block.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/invisible_brick.bmp")) {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						i.invisible = true;
						event_list["boolean_question"] = true;
						return;
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) {
						return;
					}
				}
				player.moveSpeed = 0;
				PC.SetTopLeft(obj_right, PC.GetTop());
				player.frame += 2;
			}
			//right touch
			else if (inRange(PC.GetLeft() + PC.GetWidth() + 1, obj_left, obj_mid_x) && PC.GetTop() <= obj_bottom && PC.GetTop() + PC.GetHeight() - 5 >= obj_top) {
				// end point flag
				if (i.coll.GetImageFileName() == "resources/image/object/environment/end_point_flag.bmp" || i.coll.GetImageFileName() == "resources/image/object/environment/end_point_flag_blank.bmp") {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						if (player.isDead == true) {
							player.moveSpeed = 0;
							player.player_finish_audio->Stop(5);
						}
						player.coll.SetTopLeft(i.coll.GetLeft() - player.coll.GetWidth(), i.coll.GetTop());
						player.jumpSpeed = 0;
						animation_flag = true;
						player.isFinish = true;
						i.invisible = true;
						return;
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) { return; }
				}
				// checkpoint
				if (i.coll.GetImageFileName() == "resources/image/object/environment/checkpoint_reached.bmp" || i.coll.GetImageFileName() == "resources/image/object/environment/blank.bmp") {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						if (current_map == 1) { 
							if (player.moveSpeed < 10) { player.moveSpeed = 10; }
							player.shift_amount = 3600; }
						else if (current_map == 3) { player.shift_amount = 3157; }
						player.reach_checkpoint = true;
						player.current_checkpoint_x = 120;
						player.current_checkpoint_y = groundY_up - player.coll.GetHeight();
						return;
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) {
						return;
					}
				}
				// brick4 trap
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/green_brick4_trap1.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/green_brick4_trap2.bmp") && (cheat_mode == false)) {
					i.coll.SetFrameIndexOfBitmap(1);
					player.isDead = true;
					return;
				}
				// horizontal pipeline
				if (player.keyRight == true && i.coll.GetImageFileName() == "resources/image/object/block2/pipeline_big_hor.bmp") {
					player.coll.SetFrameIndexOfBitmap(6);
					player.moveSpeed = 0; player.jumpSpeed = 0;
					// enable animation
					player.pipe_interact_audio->Play(4, false);
					pipe_hor_animation.SetFrameIndexOfBitmap(0);
					pipe_hor_animation.SetTopLeft(i.coll.GetLeft()-player.coll.GetWidth(), player.coll.GetTop());
					animation_flag = 
					pipe_hor_animation_flag = true;
					isDanger = i.is_danger;
					return;
				}
				// question brick
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/question_block.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/invisible_brick.bmp")) {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						i.invisible = true;
						event_list["boolean_question"] = true;
						return;
					}
					else if (i.coll.GetFrameIndexOfBitmap() == 1) {
						return;
					}
				}
				player.moveSpeed = 0;
				PC.SetTopLeft(obj_left - PC.GetWidth(), PC.GetTop());
				player.frame += 2;
			}
			
		}
	}
	// check coin animation
	if (coin_animation_flag == true && animation_flag == true) {
		coin_animation.SetAnimation(20, false);
		if (coin_animation.GetFrameIndexOfBitmap() == 7) {
			coin_animation.SetAnimation(20, true);
		}
	}
	// check pipe_interact animation
	if (pipe_animation_flag == true && animation_flag == true) {
		if (isDanger == true) {
			pipe_animation.SetAnimation(40, false);
			if (pipe_animation.GetFrameIndexOfBitmap() == 19) {
				pipe_animation.SetAnimation(40, true);
				player.player_fly = true;
			}

			if (player.player_fly == true) {
				player.coll.SetFrameIndexOfBitmap(4);
				player.jumpSpeed -= 2;
			}
		}
		else {
			pipe_animation.SetAnimation(40, false);
			if (pipe_animation.GetFrameIndexOfBitmap() == 19) {
				pipe_animation.SetAnimation(40, true);
				current_map += 1;
				player.isEnd = true;
				GotoGameState(GAME_STATE_RUN);
			}
		}
	}
	// check pipe_horizontal_interact animation
	if (pipe_hor_animation_flag == true && animation_flag == true) {
		if (isDanger == true) {
			pipe_hor_animation.SetAnimation(40, false);
			if (pipe_hor_animation.GetFrameIndexOfBitmap() == 5) {
				pipe_hor_animation.SetAnimation(40, true);
			}
			// bounce back
			player.coll.SetFrameIndexOfBitmap(0);
			player.moveSpeed -= 10;
		}
	}
	//player&enemys touch
	for (auto &enemy : enemys_arr) {
		if (enemy.is_dead) { continue;}
		CMovingBitmap &EC = enemy.coll;
		if (cheat_mode == false) {
			if (player.jumpSpeed >= 28) {//predict penetrate
				if (PC.GetTop() + PC.GetHeight() <= EC.GetTop() && PC.GetTop() + PC.GetHeight() + player.jumpSpeed >= EC.GetTop() && PC.GetLeft() + 2 < EC.GetLeft() + EC.GetWidth() && PC.GetLeft() + PC.GetWidth() - 2 > EC.GetLeft()) {
					player.jumpSpeed = 0;
					PC.SetTopLeft(PC.GetLeft(), EC.GetTop() - PC.GetHeight());
				}
			}
			else if (enemy.speed_y >= 28) {//predict penetrate
				if (EC.GetTop() + EC.GetHeight() <= PC.GetTop() && EC.GetTop() + EC.GetHeight() + enemy.speed_y >= PC.GetTop() && EC.GetLeft() + 2 < PC.GetLeft() + PC.GetWidth() && EC.GetLeft() + EC.GetWidth() - 2 > PC.GetLeft()) {
					player.isDead = true;
				}
			}
			else if (CMovingBitmap::IsOverlap(PC, EC)) {//check touch
				int obj_left = EC.GetLeft();
				int obj_right = EC.GetLeft() + EC.GetWidth();
				int obj_top = EC.GetTop();
				int obj_bottom = EC.GetTop() + EC.GetHeight();
				int obj_mid_x = EC.GetLeft() + (EC.GetWidth() / 2);
				int obj_mid_y = EC.GetTop() + (EC.GetHeight() / 2);
				if (enemy.cloud) {
					enemy.coll.SetFrameIndexOfBitmap(1);
				}
				if (enemy.big_mushroom) {
					player.player_powerup->Play(7, false);
					enemy.is_dead = true;
					if (PC.GetFrameIndexOfBitmap() == 0 || PC.GetFrameIndexOfBitmap() == 1 || PC.GetFrameIndexOfBitmap() == 3) {
						PC.SetFrameIndexOfBitmap(7);
					}
					else {
						PC.SetFrameIndexOfBitmap(8);
					}
					continue;
				}
				if (enemy.dead_mushroom) {
					enemy.is_dead = true;
					player.isDead = true;
				}
				//head touch
				if (inRange(PC.GetTop() - 1, obj_mid_y, obj_bottom) && PC.GetLeft() + 10 <= obj_right && PC.GetLeft() + PC.GetWidth() - 10 >= obj_left) {
					player.isDead = true;
				}
				//foot touch
				else if (inRange(PC.GetTop() + PC.GetHeight() + 1, obj_top, obj_mid_y) && PC.GetLeft() + 2 < obj_right && PC.GetLeft() + PC.GetWidth() - 2 > obj_left) {
					if (enemy.step_enemy_enemy_dead) {
						player.jumpSpeed = -19;
						enemy.is_dead = true;
						enemy.coll.UnshowBitmap();
					}
					else if (enemy.step_enemy_player_dead) {
						player.isDead = true;
					}
					else if (enemy.step_enemy_jump) {
						if (enemy.coll.GetImageFileName() == "resources/image/object/block2/flag_trap.bmp") {
							enemy.speed_x = 5;
						}
						player.jumpSpeed = -19;
					}
					else if (enemy.turtle) {
						player.jumpSpeed = -19;
						int current_bitmap = enemy.coll.GetFrameIndexOfBitmap();
						if (current_bitmap == 0 || current_bitmap == 1) {
							enemy.speed_x = 0;
							enemy.coll.SetFrameIndexOfBitmap(current_bitmap + 2);
						}
						else {
							if (player.coll.GetLeft() > enemy.coll.GetLeft()) {
								enemy.speed_x = -5;
								enemy.coll.SetFrameIndexOfBitmap(2);
							}

							else{
								enemy.speed_x = 5;
								enemy.coll.SetFrameIndexOfBitmap(3);
							}
						}
					}
				}
				//left touch
				else if (inRange(PC.GetLeft(), obj_mid_x, obj_right) && PC.GetTop() <= obj_bottom && PC.GetTop() + PC.GetHeight() - 5 >= obj_top) {
					player.moveSpeed = 0;
					PC.SetTopLeft(obj_right, PC.GetTop());
					player.frame += 2;
					player.isDead = true;
				}
				//right touch
				else if (inRange(PC.GetLeft() + PC.GetWidth() + 1, obj_left, obj_mid_x) && PC.GetTop() <= obj_bottom && PC.GetTop() + PC.GetHeight() - 5 >= obj_top) {
					player.moveSpeed = 0;
					PC.SetTopLeft(obj_left - PC.GetWidth(), PC.GetTop());
					player.frame += 2;
					player.isDead = true;
				}
			}
		}
	}

	//enemy&block touch
	for (auto &i : bricks_arr) {
		CMovingBitmap &BC = i.coll;
		for (auto &j : enemys_arr) {
			if (!j.able_touch)
				continue;
			CMovingBitmap &EC = j.coll;
			if (j.speed_y >= 15) {//predict penetrate
				if (EC.GetTop() + EC.GetHeight() <= BC.GetTop() && EC.GetTop() + EC.GetHeight() + j.speed_y >= BC.GetTop() && EC.GetLeft() + 2 < BC.GetLeft() + BC.GetWidth() && EC.GetLeft() + EC.GetWidth() - 2 > BC.GetLeft()) {
					j.speed_y = 0;
					EC.SetTopLeft(EC.GetLeft(), BC.GetTop() - EC.GetHeight());
				}
			}
			else if (CMovingBitmap::IsOverlap(EC, BC)) {//check touch
				int obj_left = BC.GetLeft();
				int obj_right = BC.GetLeft() + BC.GetWidth();
				int obj_top = BC.GetTop();
				int obj_bottom = BC.GetTop() + BC.GetHeight();
				int obj_mid_x = BC.GetLeft() + (BC.GetWidth() / 2);
				int obj_mid_y = BC.GetTop() + (BC.GetHeight() / 2);
				//head touch
				if (inRange(EC.GetTop() - 1, obj_mid_y, obj_bottom) && EC.GetLeft() + 10 <= obj_right && EC.GetLeft() + EC.GetWidth() - 10 >= obj_left) {
					j.speed_y = 0;
					EC.SetTopLeft(EC.GetLeft(), obj_bottom);
					j.speed_y += 1;
				}
				//foot touch
				else if (inRange(EC.GetTop() + EC.GetHeight() + 1, obj_top, obj_mid_y) && EC.GetLeft() + 2 < obj_right && EC.GetLeft() + EC.GetWidth() - 2 > obj_left) {
					if (i.coll.GetImageFileName() == "resources/image/object/block1/brick_break.bmp" && i.coll.GetFrameIndexOfBitmap() == 1) {
						continue;
					}
					j.speed_y = 0;
					EC.SetTopLeft(EC.GetLeft(), BC.GetTop() - EC.GetHeight());
				}
				//left touch
				else if (inRange(EC.GetLeft(), obj_mid_x, obj_right) && EC.GetTop() <= obj_bottom && EC.GetTop() + EC.GetHeight() - 5 >= obj_top) {
					j.speed_x = j.speed_x * (-1);
					EC.SetTopLeft(obj_right, EC.GetTop());
					if (j.coll.GetFrameIndexOfBitmap() % 2 == 0) {
						j.coll.SetFrameIndexOfBitmap(j.coll.GetFrameIndexOfBitmap() + 1);
					}
				}
				//right touch
				else if (inRange(EC.GetLeft() + EC.GetWidth() + 1, obj_left, obj_mid_x) && EC.GetTop() <= obj_bottom && EC.GetTop() + EC.GetHeight() - 5 >= obj_top) {
					j.speed_x = j.speed_x * (-1);
					EC.SetTopLeft(obj_left - EC.GetWidth(), EC.GetTop());
					if (j.coll.GetFrameIndexOfBitmap() % 2 == 1) {
						j.coll.SetFrameIndexOfBitmap(j.coll.GetFrameIndexOfBitmap() - 1);
					}
				}
			}
		}
		
	}
	//enemy&enemy touch
	for (auto &i : enemys_arr) {
		for (auto &j : enemys_arr) {
			if (i.coll.GetImageFileName().compare(j.coll.GetImageFileName()) == 0) {
				continue;
			}
			if (CMovingBitmap::IsOverlap(i.coll, j.coll)) {
				if ((i.big_mushroom) && (!j.big_mushroom)) {
					Enemy enemy = Enemy(j.coll.GetLeft(), j.coll.GetTop(), { "resources/image/enemy/big_normal.bmp" , "resources/image/enemy/big_normal_flip.bmp" }); enemy.speed_x = j.speed_x; enemy.speed_y = j.speed_y; enemy.step_enemy_player_dead = true;
					player.player_powerup->Play(7, false);
					if (j.coll.GetFrameIndexOfBitmap() != 0) {
						enemy.coll.SetFrameIndexOfBitmap(1);
					}
					enemys_arr.push_back(enemy);
					i.coll.SetTopLeft(i.coll.GetLeft(), 950);
					j.is_dead = true;
				}

			}
			if (CMovingBitmap::IsOverlap(i.coll, j.coll) && (i.turtle == true)) {
				if ((i.turtle) && (!j.turtle)) {
					j.is_dead = true;
				}

			}
		}
	}
}

