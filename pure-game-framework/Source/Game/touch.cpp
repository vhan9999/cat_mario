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

bool inRange(double num, double min, double max) {
	return (min <= num && num <= max);
}

bool isDanger = false;
void CGameStateRun::Touching() {
	CMovingBitmap &PC = player.coll;

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
		else if (CMovingBitmap::IsOverlap(PC, BC)) {//check touch
			int obj_left = BC.GetLeft();
			int obj_right = BC.GetLeft() + BC.GetWidth();
			int obj_top = BC.GetTop();
			int obj_bottom = BC.GetTop() + BC.GetHeight();
			int obj_mid_x = BC.GetLeft() + (BC.GetWidth() / 2);
			int obj_mid_y = BC.GetTop() + (BC.GetHeight() / 2);
			//head touch
			if (inRange(PC.GetTop() - 1, obj_mid_y, obj_bottom) && PC.GetLeft() + 10 <= obj_right && PC.GetLeft() + PC.GetWidth() - 10 >= obj_left) {
				// item brick
				if ((i.coll.GetImageFileName() == "resources/image/object/block1/item_brick.bmp" || i.coll.GetImageFileName() == "resources/image/object/block1/brown_brick2.bmp") && (i.have_coin == true)) {
					if (i.coll.GetFrameIndexOfBitmap() == 0) {
						i.coll.SetFrameIndexOfBitmap(1);
						player.coin_item_brick_audio->Play(3, false);

						// enable coin animation
						coin_animation.SetFrameIndexOfBitmap(0);
						coin_animation.SetTopLeft(i.coll.GetLeft(), i.coll.GetTop() - 168);
						animation_flag = true;
						coin_animation_flag = true;
					}
				}
				player.jumpSpeed = 0;
				PC.SetTopLeft(PC.GetLeft(), obj_bottom);
				player.jumpSpeed += 1;
			}
			//foot touch
			else if (inRange(PC.GetTop() + PC.GetHeight() +1, obj_top, obj_mid_y) && PC.GetLeft() + 2 < obj_right && PC.GetLeft() + PC.GetWidth() - 2 > obj_left) {
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
				player.jumpSpeed = 0;
				player.jumpBonusFrame = 0;
				PC.SetTopLeft(PC.GetLeft(), obj_top - PC.GetHeight());
				double ground = obj_top - PC.GetHeight();
				player.ableToJump(ground); // can jump on block
			}
			//left touch
			else if (inRange(PC.GetLeft(), obj_mid_x, obj_right) && PC.GetTop() <= obj_bottom && PC.GetTop() + PC.GetHeight() - 5 >= obj_top) {
				if (i.coll.GetImageFileName() == "resources/image/object/environment/end_point_flag.bmp") {
					player.coll.SetTopLeft(i.coll.GetLeft() - player.coll.GetWidth(), i.coll.GetTop() + 250);
					player.jumpSpeed = 0;
					animation_flag = true;
					player.isFinish = true;
					return;
				}
				player.moveSpeed = 0;
				PC.SetTopLeft(obj_right, PC.GetTop());
				player.frame += 2;
			}
			//right touch
			else if (inRange(PC.GetLeft() + PC.GetWidth() + 1, obj_left, obj_mid_x) && PC.GetTop() <= obj_bottom && PC.GetTop() + PC.GetHeight() - 5 >= obj_top) {
				if (i.coll.GetImageFileName() == "resources/image/object/environment/end_point_flag.bmp") {
					player.coll.SetTopLeft(i.coll.GetLeft() - player.coll.GetWidth(), i.coll.GetTop()+250);
					player.jumpSpeed = 0;
					animation_flag = true;
					player.isFinish = true;
					return;
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
	bool player_fly = false;
	// check pipe_interact animation
	if (pipe_animation_flag == true && animation_flag == true) {
		if (isDanger == true) {
			pipe_animation.SetAnimation(40, false);
			if (pipe_animation.GetFrameIndexOfBitmap() == 19) {
				pipe_animation.SetAnimation(40, true);
				player_fly = true;
			}

			if (player_fly == true) {
				player.coll.SetFrameIndexOfBitmap(4);
				player.jumpSpeed -= 2;
			}
		}
		else {
			pipe_animation.SetAnimation(40, false);
			if (pipe_animation.GetFrameIndexOfBitmap() == 16) {
				pipe_animation.SetAnimation(40, true);
			}
		}
	}

	//player&enemys touch
	for (auto &i : enemys_arr) {
		if (i.is_dead) { continue;}
		CMovingBitmap &EC = i.coll;

		if (player.jumpSpeed >= 15) {//predict penetrate
			if (PC.GetTop() + PC.GetHeight() <= EC.GetTop() && PC.GetTop() + PC.GetHeight() + player.jumpSpeed >= EC.GetTop() && PC.GetLeft() + 2 < EC.GetLeft() + EC.GetWidth() && PC.GetLeft() + PC.GetWidth() - 2 > EC.GetLeft()) {
				player.jumpSpeed = 0;
				PC.SetTopLeft(PC.GetLeft(), EC.GetTop() - PC.GetHeight());
			}
		}
		else if (i.speed_y >= 15) {//predict penetrate
			if (EC.GetTop() + EC.GetHeight() <= PC.GetTop() && EC.GetTop() + EC.GetHeight() + i.speed_y >= PC.GetTop() && EC.GetLeft() + 2 < PC.GetLeft() + PC.GetWidth() && EC.GetLeft() + EC.GetWidth() - 2 > PC.GetLeft()) {
				player.jumpSpeed = 0;
				PC.SetTopLeft(PC.GetLeft(), EC.GetTop() - PC.GetHeight());
			}
		}
		else if (CMovingBitmap::IsOverlap(PC, EC)) {//check touch
			int obj_left = EC.GetLeft();
			int obj_right = EC.GetLeft() + EC.GetWidth();
			int obj_top = EC.GetTop();
			int obj_bottom = EC.GetTop() + EC.GetHeight();
			int obj_mid_x = EC.GetLeft() + (EC.GetWidth() / 2);
			int obj_mid_y = EC.GetTop() + (EC.GetHeight() / 2);
			//head touch
			if (inRange(PC.GetTop() - 1, obj_mid_y, obj_bottom) && PC.GetLeft() + 10 <= obj_right && PC.GetLeft() + PC.GetWidth() - 10 >= obj_left) {
				player.isDead = true;
			}
			//foot touch
			else if (inRange(PC.GetTop() + PC.GetHeight() + 1, obj_top, obj_mid_y) && PC.GetLeft() + 2 < obj_right && PC.GetLeft() + PC.GetWidth() - 2 > obj_left) {
				if (i.step_enemy_enemy_dead) {
					player.jumpSpeed = -19;
					i.is_dead = true;
					i.coll.UnshowBitmap();
				}
				else if (i.step_enemy_player_dead) {
					player.isDead = true;
				}
				else if (i.step_enemy_jump) {
					player.jumpSpeed = -19;
				}
				else if (i.turtle) {
					player.jumpSpeed = -19;
					int current_bitmap = i.coll.GetFrameIndexOfBitmap();
					if (current_bitmap == 0 || current_bitmap == 1) {
						i.speed_x = 0;
						i.coll.SetFrameIndexOfBitmap(current_bitmap + 2);
					}
					else {
						if (player.coll.GetLeft() > i.coll.GetLeft()) {
							i.speed_x = -5;
							i.coll.SetFrameIndexOfBitmap(3);
						}

						else{
							i.speed_x = 5;
							i.coll.SetFrameIndexOfBitmap(2);
						}
					}
				}
			}
			//left touch
			else if (inRange(PC.GetLeft(), obj_mid_x, obj_right) && PC.GetTop() <= obj_bottom && PC.GetTop() + PC.GetHeight() - 5 >= obj_top) {
				player.moveSpeed = 0;
				PC.SetTopLeft(obj_right, PC.GetTop());
				player.frame += 2;
				
				if (i.turtle) {
					i.speed_x = -5;
					i.coll.SetFrameIndexOfBitmap(3);
				}
				else
					player.isDead = true;
			}
			//right touch
			else if (inRange(PC.GetLeft() + PC.GetWidth() + 1, obj_left, obj_mid_x) && PC.GetTop() <= obj_bottom && PC.GetTop() + PC.GetHeight() - 5 >= obj_top) {
				player.moveSpeed = 0;
				PC.SetTopLeft(obj_left - PC.GetWidth(), PC.GetTop());
				player.frame += 2;
				if (i.turtle) {
					i.speed_x = 5;
					i.coll.SetFrameIndexOfBitmap(2);
				}
				else
					player.isDead = true;
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
					j.speed_y = 0;
					EC.SetTopLeft(EC.GetLeft(), BC.GetTop() - EC.GetHeight());
				}
				//left touch
				else if (inRange(EC.GetLeft(), obj_mid_x, obj_right) && EC.GetTop() <= obj_bottom && EC.GetTop() + EC.GetHeight() - 5 >= obj_top) {
					j.speed_x = j.speed_x * (-1);
					EC.SetTopLeft(obj_right, EC.GetTop());
				}
				//right touch
				else if (inRange(EC.GetLeft() + EC.GetWidth() + 1, obj_left, obj_mid_x) && EC.GetTop() <= obj_bottom && EC.GetTop() + EC.GetHeight() - 5 >= obj_top) {
					j.speed_x = 0;
					EC.SetTopLeft(obj_left - EC.GetWidth(), EC.GetTop());
				}
			}
		}
		
	}

}

