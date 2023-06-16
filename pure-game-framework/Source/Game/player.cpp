#include "stdafx.h"
#include "colliders.h"
#include <string>
using namespace game_framework;

void Player::load_voice() {
	map_audio->Load(0, "resources/audio/map_song/field.wav");
	player_jump_audio->Load(1, "resources/audio/player_audio/jump.wav");
	game_over_audio->Load(2, "resources/audio/player_audio/death.wav");
	coin_item_brick_audio->Load(3, "resources/audio/interact_audio/coin.wav");
	pipe_interact_audio->Load(4, "resources/audio/interact_audio/dokan.wav");
	player_finish_audio->Load(5, "resources/audio/map_song/goal.wav");
	dungeon_audio->Load(6, "resources/audio/map_song/dungeon.wav");
	player_powerup->Load(7, "resources/audio/player_audio/powerup.wav");
}

void Player::move() {
	ani();
	voice();
	moveHor();
	moveVer();
	if (coll.GetLeft() + coll.GetWidth() > 990) { // right
		int player_posX = 990 - coll.GetWidth();
		coll.SetTopLeft(player_posX, coll.GetTop());
	}
	// gravity and moving
	if (coll.GetTop() + jumpSpeed >= 1500 || coll.GetTop() >= 1000) {// fall down (dead) 
		coll.SetTopLeft(coll.GetLeft() + moveSpeed, 1500); 
		player_fall = true; 
		player_on_air = false; 
		isDead = true;
	}	
	else if (moveSpeed != 0 || jumpSpeed != 0) {// move
		coll.SetTopLeft(coll.GetLeft() + moveSpeed, coll.GetTop() + jumpSpeed);
	}
	if (player_fall == true) {
		isDead = true;
		map_audio->Stop(0);
	}
	if (coll.GetTop() <= -500) {
		isDead = true;
		map_audio->Stop(0);
	}
}

void Player::moveHor() {
	frame += 1;

	if (keyRight == true) {
		if (moveSpeed == 0)
			moveSpeed += 1;
		if (frame % 5 == 0) {//every 10 frame
			moveSpeed += 1;
			if (moveSpeed < 0)
				moveSpeed += 4;
		}
		if (moveSpeed >= 10)//speed max = 6
			moveSpeed = 10;
	}
	if (keyLeft == true) {
		// move backward
		if (moveSpeed == 0)
			moveSpeed -= 1;
		if (frame % 5 == 0) {
			moveSpeed -= 1;
			if (moveSpeed > 0)
				moveSpeed -= 4;
		}
		if (moveSpeed <= -10)
			moveSpeed = -10;
	}
	if ((!keyRight && !keyLeft)) {// stop
		if (frame % 5 == 0) {
			if (moveSpeed >= 1)
				moveSpeed -= 1;
			else if (moveSpeed <= -1)
				moveSpeed += 1;
			else
				moveSpeed = 0;
		}
	}
	if (frame + 1 < 0) {//prevent int overflow
		frame = 0;
	}
	if (coll.GetLeft() <= 0) {
		coll.SetTopLeft(0, coll.GetTop());
	}
}

void Player::moveVer() {
	double fall_gnd = 1500;
	Player::ableToJump(fall_gnd);
}

void Player::ableToJump(double &ground) {
	jumpBonusFrame++;
	
	if (keyUp == true) {
		if (coll.GetFrameIndexOfBitmap() != 7 && coll.GetFrameIndexOfBitmap() != 8) {
			if (coll.GetFrameIndexOfBitmap() == 0 || coll.GetFrameIndexOfBitmap() == 1) {
				coll.SetFrameIndexOfBitmap(4);
			}
			if (coll.GetFrameIndexOfBitmap() == 2 || coll.GetFrameIndexOfBitmap() == 3) {
				coll.SetFrameIndexOfBitmap(5);
			}
		}
		
	}

	if (coll.GetTop() < ground && isFinish == false) {// player in the air
		if (coll.GetFrameIndexOfBitmap() != 7 && coll.GetFrameIndexOfBitmap() != 8) {
			if ((coll.GetFrameIndexOfBitmap() == 0 || coll.GetFrameIndexOfBitmap() == 1) && (player_on_air == true)) {
				coll.SetFrameIndexOfBitmap(4);
			}
			if ((coll.GetFrameIndexOfBitmap() == 2 || coll.GetFrameIndexOfBitmap() == 3) && (player_on_air == true)) {
				coll.SetFrameIndexOfBitmap(5);
			}
		}
		jumpSpeed += 1; // v += a
		player_on_air = true;
	}

	else if (keyUp == true && coll.GetTop() >= ground-1 && coll.GetTop() <= ground + 1) {// jump while on the ground
		jumpBonusFrame = 0; // for big jump
		jumpSpeed = -19;// v0
	}

	if (coll.GetTop() >= ground) {// player touch on the ground
		player_on_air = false;
		if (coll.GetFrameIndexOfBitmap() != 7 && coll.GetFrameIndexOfBitmap() != 8) {
			if (coll.GetFrameIndexOfBitmap() == 4) {
				coll.SetFrameIndexOfBitmap(0);
			}
			if (coll.GetFrameIndexOfBitmap() == 5) {
				coll.SetFrameIndexOfBitmap(2);
			}
		}
	}
	if (jumpBonusFrame == 5 && keyUp) {// jump hold duration (if hold long will higher)
		jumpSpeed -= 5; // v-=5(a)
	}
}

void Player::ani() {
	animate_frame += 0.25;
	if (coll.GetFrameIndexOfBitmap() != 7 && coll.GetFrameIndexOfBitmap() != 8) {
		if (keyRight == true) {
			coll.SetFrameIndexOfBitmap(0);

			// change image while moving
			if ((int(animate_frame) % 2 == 0) && (coll.GetFrameIndexOfBitmap() == 0) && (jumpSpeed == 0)) { // frame moldulus of odd number
				coll.SetFrameIndexOfBitmap(1);

			}
			else if ((int(animate_frame) % 11 == 0) && (coll.GetFrameIndexOfBitmap() == 1) && (jumpSpeed == 0)) {  // frame moldulus of even number 
				coll.SetFrameIndexOfBitmap(0);
			}
		}
	}
	if (coll.GetFrameIndexOfBitmap() != 7 && coll.GetFrameIndexOfBitmap() != 8) {
		if (keyLeft == true) {
			coll.SetFrameIndexOfBitmap(2);

			// change image while moving
			if ((int(animate_frame) % 2 == 0) && (coll.GetFrameIndexOfBitmap() == 2) && (jumpSpeed == 0)) { // frame moldulus of odd number
				coll.SetFrameIndexOfBitmap(3);

			}
			else if ((int(animate_frame) % 11 == 0) && (coll.GetFrameIndexOfBitmap() == 3) && (jumpSpeed == 0)) {  // frame moldulus of even number 
				coll.SetFrameIndexOfBitmap(2);
			}
		}
	}
	if (animate_frame >= 60) {
		animate_frame = 0;
	}

}

void Player::check_finish() {
	if (isFinish == true) {
		finish_audio_flag += 1;
		if (finish_audio_flag == 1) {
			map_audio->Stop(0);
			player_jump_audio->Stop(1);
			player_finish_audio->Play(5, false);
		}

		if (finish_audio_flag == 10) { finish_audio_flag = 5; }
	}
}			

void Player::voice() {
	if ((player_on_air == false) && (keyUp == true)) {
		player_jump_audio->Play(1, false);
	}
}

void Player::resetValue() {
	frame = 0;
	animate_frame = 0;
	jumpBonusFrame = 0;
	dead_frame = 0;
	jumpSpeed = 0;
	moveSpeed = 0;
	dead_audio_flag = 0;
	finish_audio_flag = 0;
	map3frame = 0;

	if (isEnd == true) {
		shift_amount = 0;
		distance_count = 0;
		reach_checkpoint = false;
	}
	coll.SetFrameIndexOfBitmap(0);
	keyUp = false;
	keyDown = false;
	keyLeft = false;
	keyRight = false;
	player_fall = false;
	player_fly = false;
	player_on_air = false;
	isMove = false;
	isDead = false;
	isFinish = false;
	isEnd = false;
}