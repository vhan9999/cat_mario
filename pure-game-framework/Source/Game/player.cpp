#include "stdafx.h"
#include "colliders.h"

using namespace game_framework;

void Player::load_voice() {
	map_audio->Load(0, "resources/audio/map_song/field.wav");
	player_jump_audio->Load(1, "resources/audio/player_audio/jump.wav");
	player_dead_audio->Load(2, "resources/audio/player_audio/death.wav");
	coin_item_brick_audio->Load(3, "resources/audio/interact_audio/coin.wav");
	pipe_interact_audio->Load(4, "resources/audio/interact_audio/dokan.wav");
	player_finish_audio->Load(5, "resources/audio/map_song/goal.wav");
}

void Player::move() {
	ani();
	voice();
	moveHor();
	moveVer();
	// gravity and moving
	if (coll.GetTop() + jumpSpeed >= 1500) {// fall down (dead) 
		coll.SetTopLeft(coll.GetLeft() + moveSpeed, 1500); 
		player_fall = true; 
		player_on_air = false; 
	}	
	else if (moveSpeed != 0 || jumpSpeed != 0) {// move
		coll.SetTopLeft(coll.GetLeft() + moveSpeed, coll.GetTop() + jumpSpeed);
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
				moveSpeed += 3;
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
				moveSpeed -= 3;
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
	if (coll.GetLeft() + coll.GetWidth() > 512) { // right
		int player_posX = 512 - coll.GetWidth();
		coll.SetTopLeft(player_posX, coll.GetTop());
	}
}
void Player::moveVer() {
	double fall_gnd = 1500;
	Player::ableToJump(fall_gnd);
}
void Player::ableToJump(double &ground) {
	jumpBonusFrame++;

	if (keyUp == true) {
		if (coll.GetFrameIndexOfBitmap() == 0 || coll.GetFrameIndexOfBitmap() == 1) {
			coll.SetFrameIndexOfBitmap(4);
		}
		if (coll.GetFrameIndexOfBitmap() == 2 || coll.GetFrameIndexOfBitmap() == 3) {
			coll.SetFrameIndexOfBitmap(5);
		}
	}

	if (coll.GetTop() < ground && isFinish == false) {// player in the air
		if ((coll.GetFrameIndexOfBitmap() == 0 || coll.GetFrameIndexOfBitmap() == 1) && (player_on_air == true)) {
			coll.SetFrameIndexOfBitmap(4);
		}
		if ((coll.GetFrameIndexOfBitmap() == 2 || coll.GetFrameIndexOfBitmap() == 3) && (player_on_air == true)) {
			coll.SetFrameIndexOfBitmap(5);
		}
		jumpSpeed += 1; // v += a
		player_on_air = true;
	}

	else if (keyUp == true && coll.GetTop() == ground) {// jump while on the ground
		jumpBonusFrame = 0; // for big jump
		jumpSpeed = -19;// v0
	}

	if (coll.GetTop() >= ground) {// player touch on the ground
		player_on_air = false;
		if (coll.GetFrameIndexOfBitmap() == 4) {
			coll.SetFrameIndexOfBitmap(0);
		}
		if (coll.GetFrameIndexOfBitmap() == 5) {
			coll.SetFrameIndexOfBitmap(2);
		}
	}
	// when player hit the finish flag
	if (isFinish == true) { 
		coll.SetFrameIndexOfBitmap(4);
		moveSpeed = 0; jumpSpeed = 0;
		jumpSpeed += 5;
		if (jumpSpeed >= 5) { jumpSpeed = 5;} // fix jump speed 
		if (player_on_air == false) {
			coll.SetFrameIndexOfBitmap(0);
			coll.SetTopLeft(560, coll.GetTop());
			moveSpeed += 2;
			if (Player::shift_amount >= 7239) { // player meet end point
				moveSpeed = 0;
				coll.SetFrameIndexOfBitmap(6);
			}
		}
	}
	if (jumpBonusFrame == 5 && keyUp) {// jump hold duration (if hold long will higher)
		jumpSpeed -= 5; // v-=5(a)
	}
	
}

void Player::ani() {
	animate_frame += 0.25;
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

/*
// player dead check
void Player::dead_check() {
	
	if (player_fall == true) { isDead = true; }

	if (isDead == true) {
		jumpSpeed = 0; moveSpeed = 0;
		dead_audio_flag += 1;
		if (dead_audio_flag == 1) {
			map_audio->Stop(0);
			player_jump_audio->Stop(1);
			player_dead_audio->Play(2, false);
		}
		if (dead_audio_flag == 10) { dead_audio_flag = 5; }
	}
}
*/

void Player::voice() {
	if ((player_on_air == false) && (keyUp == true)) {
		player_jump_audio->Play(1, false);
	}
}
