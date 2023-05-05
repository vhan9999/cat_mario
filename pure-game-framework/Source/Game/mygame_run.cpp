#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "stdio.h"
#include <vector>
#include <crtdbg.h>
#include <string>
using namespace game_framework;


/* ----NOTE---- */
/*-----------------------------------------------------------------------------------------------------*/
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// This class is the game execution object of the game, and the main game programs are here
// select multiple line = alt + shift + .
/*
	bmp image background
	player = (255, 242, 0)
	object, enemy = (163, 73, 164)
*/
/////////////////////////////////////////////////////////////////////////////
/*-----------------------------------------------------------------------------------------------------*/

/* ----CLASS---- */
/*-----------------------------------------------------------------------------------------------------*/
// Audio
/*-----------------------------------------------------------------------------------------------------*/
CAudio *audio = CAudio::Instance();
/*-----------------------------------------------------------------------------------------------------*/

int current_checkpoint_x;
int current_checkpoint_y;

// Image
class ImageFactory {
public:
	static CMovingBitmap createBrick(int type, int x, int y) {
		CMovingBitmap new_brick;
		switch (type) {
		case 1: // brick1
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick.bmp" }, RGB(163, 73, 164));
		case 2: // brick2
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick2.bmp" }, RGB(163, 73, 164));
		case 3: // brick3
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick3.bmp" }, RGB(163, 73, 164));
		case 4: //brick4
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick4.bmp" }, RGB(163, 73, 164));
		case 5: // brick5
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick5.bmp" }, RGB(163, 73, 164));
		default:
			break;
		}
		new_brick.SetFrameIndexOfBitmap(0);
		new_brick.SetTopLeft(x, y);
		return new_brick;
	}

	static CMovingBitmap createInteractBlock(std::string name, int x, int y) {
		CMovingBitmap new_obj;
		if (name == "pipeline_short") {
			new_obj.LoadBitmapByString({ "resources/image/object/block2/pipeline_short.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "pipeline_mid") {
			new_obj.LoadBitmapByString({ "resources/image/object/block2/pipeline_mid.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "pipeline_big") {
			new_obj.LoadBitmapByString({ "resources/image/object/block2/pipeline_big.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "item_block") {
			new_obj.LoadBitmapByString({ "resources/image/object/block1/item_brick.bmp", "resources/image/object/block1/brown_brick2.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "cloud_eye") {
			new_obj.LoadBitmapByString({ "resources/image/enemy/cloud_eye.bmp", "resources/image/enemy/cloud_devil.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "checkpoint_flag") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/checkpoint_reached.bmp" , "resources/image/object/environment/blank.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "endpoint_flag") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/end_point_flag.bmp" }, RGB(163, 73, 164));
		}
		new_obj.SetTopLeft(x, y);
		return new_obj;
	}

	static CMovingBitmap createEnemy(std::string name, int x, int y) {
		CMovingBitmap enemy;
		if (name == "normal") {
			enemy.LoadBitmapByString({ "resources/image/enemy/normal.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "star_smile") {
			enemy.LoadBitmapByString({ "resources/image/enemy/star_smile.bmp" }, RGB(163, 73, 164));
		}
		enemy.SetFrameIndexOfBitmap(0);
		enemy.SetTopLeft(x, y);
		return enemy;
	}

	static CMovingBitmap createEnvironment(std::string name, int x, int y) {
		CMovingBitmap new_obj;
		if (name == "mountain") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/mountain.bmp", }, RGB(163, 73, 164));
		}
		else if (name == "tree1") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/tree1.bmp", }, RGB(163, 73, 164));
		}
		else if (name == "tree2") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/tree2.bmp", }, RGB(163, 73, 164));
		}
		else if (name == "grass") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/grass.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "cloud") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/cloud.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "cloud_eye") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/cloud_eye.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "endpoint_building") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/end_point_building.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "coin") {
			new_obj.LoadBitmapByString({ "resources/image/object/environment/coin.bmp" }, RGB(163, 73, 164));
		}

		new_obj.SetFrameIndexOfBitmap(0);
		new_obj.SetTopLeft(x, y);
		return new_obj;
	}

	
};

// Interact block
class InteractBlock {
private:
	std::string _image_name;
	CMovingBitmap _image_object;
	int _x, _y;
	bool _isDangerous;
	std::string _spawn_name;
public:
	static std::vector<CMovingBitmap> interact_block_arr;
	InteractBlock(std::string image_name, int x, int y, bool isDangerous, std::string spawn) : _image_name(image_name), _x(x), _y(y), _isDangerous(isDangerous), _spawn_name(spawn) {
		_image_object = ImageFactory::createInteractBlock(_image_name, _x, _y);
		_image_object.SetDanger(_isDangerous);
		_image_object.SetSpawn(_spawn_name);
		_image_object.SetFrameIndexOfBitmap(0);
		interact_block_arr.push_back(_image_object);
	}
	~InteractBlock() {}
	// getter
	int getObjectHeight() { return _image_object.GetHeight(); }
	int getObjectWidth() { return _image_object.GetWidth(); }
	//function
	static void showImage() {
		for (auto i : interact_block_arr) { i.ShowBitmap(); }
	}
};
std::vector<CMovingBitmap> InteractBlock::interact_block_arr = {};

// Environment
class EnvironmentBlock {
private:
	std::string _image_name;
	CMovingBitmap _image_object;
	int _x, _y;
public:
	static std::vector<CMovingBitmap> environment_block_arr;
	EnvironmentBlock(std::string image_name, int x, int y) :_image_name(image_name), _x(x), _y(y) {
		_image_object = ImageFactory::createInteractBlock(_image_name, _x, _y);
		environment_block_arr.push_back(_image_object);
	}
	~EnvironmentBlock() {}
	static void showImage() {
		for (auto i : environment_block_arr) { i.ShowBitmap(); }
	}
};
std::vector<CMovingBitmap> EnvironmentBlock::environment_block_arr = {};
/*-----------------------------------------------------------------------------------------------------*/


/* ----FUNCTION---- */
/*-----------------------------------------------------------------------------------------------------*/
// build ground
void CGameStateRun::build_block_ground(int type, int amt, int x, int y) {
	std::vector<CMovingBitmap> block_arr;
	CMovingBitmap brick;
	for (int i = 0; i < amt; i++) {
		brick = ImageFactory::createBrick(type, x, y);
		block_arr.push_back(brick);
		x += 60;
	}
	current_ground_arr_flag += 1;
	if (current_ground_arr_flag % 2 == 0) {
		upper_ground_brick_arr.push_back(block_arr);
	}
	else {
		rem_ground_brick_arr.push_back(block_arr);
	}
}

// load ground 
void CGameStateRun::loadImage_ground(int amount, int x_up, int y_up, int x_down, int y_down) {
	build_block_ground(3, amount, x_up, y_up); // ground brick up
	build_block_ground(5, amount, x_down, y_down); // ground brick down
}

// load block multiple (vertical)
void CGameStateRun::loadImage_multiple_ver(int type, int amount, int x, int y) {
	std::vector<CMovingBitmap> block_arr;
	CMovingBitmap brick;
	for (int i = 0; i < amount; i++) { // ground brick up
		brick = ImageFactory::createBrick(type, x, y);
		block_arr.push_back(brick);
		y -= 60;
	}
	ver_block_arr.push_back(block_arr);
}

// load block multiple (horizontal)
void CGameStateRun::loadImage_multiple_hor(int type, int amount, int x, int y) {
	std::vector<CMovingBitmap> block_arr;
	CMovingBitmap brick;
	for (int i = 0; i < amount; i++) { // ground brick up
		brick = ImageFactory::createBrick(type, x, y);
		block_arr.push_back(brick);
		x += 60;
	}
	hor_block_arr.push_back(block_arr);
}

// load enemy
void CGameStateRun::loadImage_enemy(std::string name, int x, int y) {
	CMovingBitmap enemy = ImageFactory::createEnemy(name, x, y);
	enemy_arr.push_back(enemy);
}

// load environment
void CGameStateRun::loadImage_environment(std::string name, int x, int y) {
	CMovingBitmap environment_obj = ImageFactory::createEnvironment(name, x, y);
	environment_arr.push_back(environment_obj);
}

// --Show--
void CGameStateRun::show_ground() {
	for (auto i : upper_ground_brick_arr) { for (auto j : i) j.ShowBitmap(); }
	for (auto i : rem_ground_brick_arr) { for (auto j : i) j.ShowBitmap(); }
}

void CGameStateRun::show_hor() {
	for (auto i : hor_block_arr) { for (auto j : i) { j.ShowBitmap(); } }
}

void CGameStateRun::show_ver() {
	for (auto i : ver_block_arr) { for (auto j : i) { j.ShowBitmap(); } }
}

void CGameStateRun::show_enemy() {
	for (auto i : enemy_arr) { i.ShowBitmap(); }
}

void CGameStateRun::show_environment() {
	for (auto i : environment_arr) { i.ShowBitmap(); }
}

void CGameStateRun::show_animation() {
	for (auto i : animation_arr) { i.ShowBitmap(); }
}

// check value is in range [min, max] or not
bool CGameStateRun::inRange(double num, double min, double max) {
	return (min <= num && num <= max);
}

// able to jump
void CGameStateRun::ableToJump(int &jumpSpd, int &jumpBns, double &ground) {
	jumpBns++; // frame

	if (keyUp == true) {
		if (player.GetFrameIndexOfBitmap() == 0 || player.GetFrameIndexOfBitmap() == 1) {
			player.SetFrameIndexOfBitmap(4);
		}
		if (player.GetFrameIndexOfBitmap() == 2 || player.GetFrameIndexOfBitmap() == 3) {
			player.SetFrameIndexOfBitmap(5);
		}
	}

	if (player.GetTop() < ground) {// player in the air
		if ((player.GetFrameIndexOfBitmap() == 0 || player.GetFrameIndexOfBitmap() == 1) && (player_on_air == true)) {
			player.SetFrameIndexOfBitmap(4);
		}
		if ((player.GetFrameIndexOfBitmap() == 2 || player.GetFrameIndexOfBitmap() == 3) && (player_on_air == true)) {
			player.SetFrameIndexOfBitmap(5);
		}
		jumpSpd += 1; // v += a
		player_on_air = true;
	}

	else if (keyUp == true && player.GetTop() == ground) {// jump while on the ground
		jumpBns = 0; // for big jump 
		jumpSpd = -19;// v0			
	}
	
	if (player.GetTop() >= ground) {// player touch on the ground
		player_on_air = false;
		if (player.GetFrameIndexOfBitmap() == 4) {
			player.SetFrameIndexOfBitmap(0);
		}
		if (player.GetFrameIndexOfBitmap() == 5) {
			player.SetFrameIndexOfBitmap(2);
		}
	}
	
	if (jumpBns == 5 && keyUp) {// jump hold duration (if hold long will higher)
		jumpSpd -= 5; // v-=5(a)
	}
	if ((player_on_air == false) && (keyUp == true)) { audio->Play(1, false); } // player jump audio

}
// brick collision
void CGameStateRun::check_collision_brick(std::vector<CMovingBitmap> &arr, CMovingBitmap &player) {
	for (auto &i : arr) {
		if (CMovingBitmap::IsOverlap(player, i)) {
			int obj_left = i.GetLeft();
			int obj_right = i.GetLeft() + i.GetWidth();
			int obj_top = i.GetTop();
			int obj_bottom = i.GetTop() + i.GetHeight();
			int obj_mid_x = i.GetLeft() + (i.GetWidth() / 2);
			int obj_mid_y = i.GetTop() + (i.GetHeight() / 2);
			//head touch
			if (inRange(player.GetTop() - 1, obj_mid_y, obj_bottom) && player.GetLeft() + 10 <= obj_right && player.GetLeft() + player.GetWidth() - 10 >= obj_left) {
				jumpSpeed = 0;
				player.SetTopLeft(player.GetLeft(), obj_bottom);
				jumpSpeed += 1;

			}
			//foot touch
			else if (inRange(player.GetTop() + player.GetHeight() + 1, obj_top, obj_mid_y) && player.GetLeft() + 2 < obj_right && player.GetLeft() + player.GetWidth() - 2 > obj_left) {
				jumpSpeed = 0;
				jumpBonusFrame = 0;
				player.SetTopLeft(player.GetLeft(), obj_top - player.GetHeight());
				double ground = obj_top - player.GetHeight();
				CGameStateRun::ableToJump(jumpSpeed, jumpBonusFrame, ground); // can jump on block
			}
			//left touch
			else if (inRange(player.GetLeft() - 1, obj_mid_x, obj_right) && player.GetTop() <= obj_bottom && player.GetTop() + player.GetHeight() - 5 >= obj_top) {
				moveSpeed = 0;
				player.SetTopLeft(obj_right, player.GetTop());
				frame += 2;

			}
			//right touch
			else if (inRange(player.GetLeft() + player.GetWidth() + 1, obj_left, obj_mid_x) && player.GetTop() <= obj_bottom && player.GetTop() + player.GetHeight() - 5 >= obj_top) {
				moveSpeed = 0;
				player.SetTopLeft(obj_left - player.GetWidth(), player.GetTop());
				frame += 2;

			}
		}
	}
}

// check interact brick collision
void CGameStateRun::check_collision_interact_brick(std::vector<CMovingBitmap> &arr, CMovingBitmap &player) {
	for (auto &i : arr) {
		if (CMovingBitmap::IsOverlap(player, i)) {
			int obj_left = i.GetLeft();
			int obj_right = i.GetLeft() + i.GetWidth();
			int obj_top = i.GetTop();
			int obj_bottom = i.GetTop() + i.GetHeight();
			int obj_mid_x = i.GetLeft() + (i.GetWidth() / 2);
			int obj_mid_y = i.GetTop() + (i.GetHeight() / 2);

			bool is_collide_block = true;
			// head touch
			if (inRange(player.GetTop() - 1, obj_mid_y, obj_bottom) && player.GetLeft() + 10 <= obj_right && player.GetLeft() + player.GetWidth() - 10 >= obj_left) {
				// checkpoint flag
				if (i.GetImageFileName() == "resources/image/object/environment/checkpoint_reached.bmp" || i.GetImageFileName() == "resources/image/object/environment/blank.bmp") {
					i.SetFrameIndexOfBitmap(1);
					shift_amount = 0; // change checkpoint by reset shift_amount 
					current_checkpoint_x = 120;
					current_checkpoint_y = groundY_up - player.GetHeight();
					return;
				}
				// item block 
				else if ((i.GetImageFileName() == "resources/image/object/block1/item_brick.bmp" || i.GetImageFileName() == "resources/image/object/block1/brown_brick2.bmp") && (i.GetSpawn() == "coin")) {
					if (i.GetFrameIndexOfBitmap() == 0) {
						i.SetFrameIndexOfBitmap(1); // change image
						audio->Play(3, false); // coin audio
						
						// enable coin animation
						coin_animation.SetFrameIndexOfBitmap(0);
						coin_animation.SetTopLeft(i.GetLeft(), i.GetTop() - 168);
						animation_arr.push_back(coin_animation); 
						animation_flag = true;
						coin_animation_flag = true;
					}
				}
				jumpSpeed = 0;
				player.SetTopLeft(player.GetLeft(), obj_bottom);
				jumpSpeed += 1;
			}
			//foot touch
			else if (inRange(player.GetTop() + player.GetHeight() + 1, obj_top, obj_mid_y) && player.GetLeft() + 2 < obj_right && player.GetLeft() + player.GetWidth() - 2 > obj_left) {
				// checkpoint flag
				if (i.GetImageFileName() == "resources/image/object/environment/checkpoint_reached.bmp" || i.GetImageFileName() == "resources/image/object/environment/blank.bmp") {
					i.SetFrameIndexOfBitmap(1);
					shift_amount = 0;
					current_checkpoint_x = 120;
					current_checkpoint_y = groundY_up - player.GetHeight();
					return;
				}
				// interact pipe
				else if (keyDown == true && i.GetImageFileName() == "resources/image/object/block2/pipeline_big.bmp") {
					player.SetFrameIndexOfBitmap(6);
					moveSpeed = 0;
					// enable animation
					interact_pipe_animation.SetFrameIndexOfBitmap(0);
					interact_pipe_animation.SetTopLeft(i.GetLeft()+30, i.GetTop() - 68);
					animation_arr.push_back(interact_pipe_animation);
					animation_flag = true;
					interact_pipe_animation_flag = true;
					is_dangerous_pipe = i.GetDanger();
					return;
				}
				jumpSpeed = 0;
				jumpBonusFrame = 0;
				player.SetTopLeft(player.GetLeft(), obj_top - player.GetHeight());
				double ground = obj_top - player.GetHeight();
				CGameStateRun::ableToJump(jumpSpeed, jumpBonusFrame, ground); // can jump on block
			}
			//left touch
			else if (inRange(player.GetLeft() - 1, obj_mid_x, obj_right) && player.GetTop() <= obj_bottom && player.GetTop() + player.GetHeight() - 5 >= obj_top) {
				// checkpoint flag
				if (i.GetImageFileName() == "resources/image/object/environment/checkpoint_reached.bmp" || i.GetImageFileName() == "resources/image/object/environment/blank.bmp") {
					i.SetFrameIndexOfBitmap(1);
					shift_amount = 0;
					current_checkpoint_x = 120;
					current_checkpoint_y = groundY_up - player.GetHeight();
					return;
				}
				moveSpeed = 0;
				player.SetTopLeft(obj_right, player.GetTop());
				frame += 2;
				
			}
			//right touch
			else if (inRange(player.GetLeft() + player.GetWidth() + 1, obj_left, obj_mid_x) && player.GetTop() <= obj_bottom && player.GetTop() + player.GetHeight() - 5 >= obj_top) {
				// checkpoint flag
				if (i.GetImageFileName() == "resources/image/object/environment/checkpoint_reached.bmp" || i.GetImageFileName() == "resources/image/object/environment/blank.bmp") {
					i.SetFrameIndexOfBitmap(1);
					shift_amount = 0;
					current_checkpoint_x = 120;
					current_checkpoint_y = groundY_up - player.GetHeight();
					return;
				}
				moveSpeed = 0;
				player.SetTopLeft(obj_left - player.GetWidth(), player.GetTop());
				frame += 2;
			}
		}
	}
}

// check ground collision
void CGameStateRun::check_ground_collision(std::vector<CMovingBitmap> &arr, CMovingBitmap &player) {
	int obj_left = arr[0].GetLeft();
	int obj_right = arr[arr.size() - 1].GetLeft() + arr[arr.size() - 1].GetWidth();
	int obj_top = arr[0].GetTop();
	int obj_bottom = arr[0].GetTop() + arr[0].GetHeight();

	bool Left = inRange(player.GetLeft() + player.GetWidth(), obj_left + 4, obj_right - 4);
	bool Right = inRange(player.GetLeft(), obj_left + 4, obj_right - 4);
	bool isCollideUpperBrick = inRange(player.GetTop() + player.GetHeight(), obj_top, obj_top + 300);
	if ((Left == true || Right == true) && (isCollideUpperBrick == true)) {
		jumpSpeed = 0;
		jumpBonusFrame = 0;
		player.SetTopLeft(player.GetLeft(), obj_top - player.GetHeight());
		double ground = obj_top - player.GetHeight();
		CGameStateRun::ableToJump(jumpSpeed, jumpBonusFrame, ground); // can jump on block
	}
}

// high from ground
int CGameStateRun::high_from_ground(int blockCount) {
	return  groundY_up - (60 * blockCount);
}

// far from start
int CGameStateRun::far_from_start(int blockCount) {
	return 60 * blockCount;
}


/* ---- CGameStateRun ---- */
/*-----------------------------------------------------------------------------------------------------*/
CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{

}

// move Horizontal
void CGameStateRun::moveHor() {
	if (keyRight == true) {//move right
		player.SetFrameIndexOfBitmap(0);

		// change image while moving
		if ((animate_frame % 6 == 0) && (player.GetFrameIndexOfBitmap() == 0) && (jumpSpeed == 0)) { // frame moldulus of odd number
			player.SetFrameIndexOfBitmap(1);

		}
		else if ((animate_frame % 6 == 0) && (player.GetFrameIndexOfBitmap() == 1) && (jumpSpeed == 0)) {  // frame moldulus of even number 
			player.SetFrameIndexOfBitmap(0);
		}

		// move forward
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
	if (keyLeft == true) {//move left
		player.SetFrameIndexOfBitmap(2);

		// change image while moving
		if ((animate_frame % 6 == 0) && (player.GetFrameIndexOfBitmap() == 2) && (jumpSpeed == 0)) { // frame moldulus of odd number
			player.SetFrameIndexOfBitmap(3);

		}
		else if ((animate_frame % 4 == 0) && (player.GetFrameIndexOfBitmap() == 3) && (jumpSpeed == 0)) {  // frame moldulus of even number 
			player.SetFrameIndexOfBitmap(2);
		}

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
}

// jump
void CGameStateRun::moveVer()
{
	double fall_gnd = 1500;
	ableToJump(jumpSpeed, jumpBonusFrame, fall_gnd);
}

void CGameStateRun::OnMove()  // 移動遊戲元素 move (always loop)
{
	frame += 1;//用來判斷幀數
	animate_frame += 1;
	moveHor();
	moveVer();

	// gravity and moving
	if (player.GetTop() + jumpSpeed >= 1500) {// fall down (dead)
		player.SetTopLeft(player.GetLeft() + moveSpeed, 1500);
		jumpSpeed = 0; moveSpeed = 0;
		game_over = true;
		player_on_air = false;
		dead_audio_flag += 1;
		if (dead_audio_flag == 10) { dead_audio_flag = 5; }
	}
	else if (moveSpeed != 0 || jumpSpeed != 0) {// move
		player.SetTopLeft(player.GetLeft() + moveSpeed, player.GetTop() + jumpSpeed);
	}

	if (player.GetTop() <= -500) { // player fly to high (dead)
		jumpSpeed = 0;  moveSpeed = 0;
		game_over = true;
		player_on_air = false;
		dead_audio_flag += 1;
		if (dead_audio_flag == 10) { dead_audio_flag = 5; }
	}

	if (frame + 1 < 0) {//到int的上限後 歸零 prevent int overflow
		frame = 0;
	}
	if (animate_frame + 1 < 0) {
		animate_frame = 0;
	}

	
	for (auto i : upper_ground_brick_arr) { CGameStateRun::check_ground_collision(i, player); } // collision ground
	for (auto i : ver_block_arr) { CGameStateRun::check_collision_brick(i, player); } // collision check vertical
	for (auto i : hor_block_arr) { CGameStateRun::check_collision_brick(i, player); } // collision check horizontal
	CGameStateRun::check_collision_interact_brick(InteractBlock::interact_block_arr, player); // collision interact block

	// player restriction
	if (player.GetLeft() <= 0) {
		player.SetTopLeft(0, player.GetTop());
	}
	if (player.GetLeft() + player.GetWidth() > 512) { // right
		int player_posX = 512 - player.GetWidth();
		player.SetTopLeft(player_posX, player.GetTop());
		// shift image left 
		for (auto &i : upper_ground_brick_arr) {
			for (auto &j : i) {
				int block_pos = j.GetLeft() - moveSpeed;
				j.SetTopLeft(block_pos, j.GetTop());
			}
		}
		for (auto &i : rem_ground_brick_arr) {
			for (auto &j : i) {
				int block_pos = j.GetLeft() - moveSpeed;
				j.SetTopLeft(block_pos, j.GetTop());
			}
		}
		for (auto &i : ver_block_arr) {
			for (auto &j : i) {
				int block_pos = j.GetLeft() - moveSpeed;
				j.SetTopLeft(block_pos, j.GetTop());
			}
		}
		for (auto &i : hor_block_arr) {
			for (auto &j : i) {
				int block_pos = j.GetLeft() - moveSpeed;
				j.SetTopLeft(block_pos, j.GetTop());
			}
		}
		for (auto &i : InteractBlock::interact_block_arr) {
			int block_pos = i.GetLeft() - moveSpeed;
			i.SetTopLeft(block_pos, i.GetTop());
		}
		for (auto &i : environment_arr) {
			int obj_pos = i.GetLeft() - moveSpeed;
			i.SetTopLeft(obj_pos, i.GetTop());
		}
		// shift animation left
		if(animation_flag == true){
			// coin 
			int coin_pos = coin_animation.GetLeft() - moveSpeed;
			coin_animation.SetTopLeft(coin_pos, coin_animation.GetTop());
			// interact pipe
			int ani_pos = interact_pipe_animation.GetLeft() - moveSpeed;
			interact_pipe_animation.SetTopLeft(ani_pos, interact_pipe_animation.GetTop());
		}

		shift_amount += moveSpeed; // for calculate returning checkpoint
	}
	// start coin animation
	if (coin_animation_flag == true && animation_flag == true) {
		coin_animation.SetAnimation(20, false);
		if (coin_animation.GetFrameIndexOfBitmap() == 7) {
			coin_animation.SetAnimation(20, true);
		}
	}
	// start pipe_interact animation
	if (interact_pipe_animation_flag == true && animation_flag == true) {
		if (is_dangerous_pipe == true) {
			interact_pipe_animation.SetAnimation(40, false);
			if (interact_pipe_animation.GetFrameIndexOfBitmap() == 19) {
				interact_pipe_animation.SetAnimation(40, true);
				player_fly_animation = true;
			}

			if (player_fly_animation == true) {
				player.SetFrameIndexOfBitmap(4);
				jumpSpeed -= 2;
			}
		}
		else {
			interact_pipe_animation.SetAnimation(40, false);
			if (interact_pipe_animation.GetFrameIndexOfBitmap() == 16) {
				interact_pipe_animation.SetAnimation(40, true);
			}
		}
	}
	// game over
	if (game_over == true) {
		audio->Stop(0);
		audio->Stop(1);
		if (dead_audio_flag == 1) {
			audio->Play(2);
			game_over_count -= 1;
			Sleep(2000);
			game_over_image.SetFrameIndexOfBitmap(0); // game over image
			
			// reset interact block
			for (auto &i : InteractBlock::interact_block_arr) {
				if(i.GetImageFileName() != "resources/image/object/environment/checkpoint_reached.bmp" && i.GetImageFileName() != "resources/image/object/environment/blank.bmp"){
					i.SetFrameIndexOfBitmap(0);
				}
			}
			// shift image right (go back to checkpoint)
			for (auto &i : upper_ground_brick_arr) {
				for (auto &j : i) {
					int block_pos = j.GetLeft() + shift_amount;
					j.SetTopLeft(block_pos, j.GetTop());
				}
			}
			for (auto &i : rem_ground_brick_arr) {
				for (auto &j : i) {
					int block_pos = j.GetLeft() + shift_amount;
					j.SetTopLeft(block_pos, j.GetTop());
				}
			}
			for (auto &i : ver_block_arr) {
				for (auto &j : i) {
					int block_pos = j.GetLeft() + shift_amount;
					j.SetTopLeft(block_pos, j.GetTop());
				}
			}
			for (auto &i : hor_block_arr) {
				for (auto &j : i) {
					int block_pos = j.GetLeft() + shift_amount;
					j.SetTopLeft(block_pos, j.GetTop());
				}
			}
			for (auto &i : InteractBlock::interact_block_arr) {
				int block_pos = i.GetLeft() + shift_amount;
				i.SetTopLeft(block_pos, i.GetTop());
			}
			for (auto &i : environment_arr) {
				int obj_pos = i.GetLeft() + shift_amount;
				i.SetTopLeft(obj_pos, i.GetTop());
			}
			// reset animation
			if (animation_flag == true) {
				// coin
				int coin_pos = coin_animation.GetLeft() + shift_amount;
				coin_animation.SetTopLeft(coin_pos, coin_animation.GetTop());
				// interact pipe
				int ani_pos = interact_pipe_animation.GetLeft() + shift_amount;
				interact_pipe_animation.SetTopLeft(ani_pos, interact_pipe_animation.GetTop());
			}
			animation_flag = false;
			interact_pipe_animation_flag = false;
			player_fly_animation = false;
		}
		else {
			Sleep(2000);
			game_over_image.SetFrameIndexOfBitmap(1);
			audio->Play(0, true); // play field music
			player.SetFrameIndexOfBitmap(0);
			player.SetTopLeft(current_checkpoint_x, current_checkpoint_y); // player start at current checkpoint
			// reset value
			shift_amount = 0;
			dead_audio_flag = 0;
			game_over = false;
		}
	}
}

/* ---- Map ---- */
/*-----------------------------------------------------------------------------------------------------*/
void CGameStateRun::setMap1(CMovingBitmap &player) {
	int currentGroundBlock = 0; // track how many ground block were build
	// phase 1
	loadImage_ground(17, groundX_up, groundY_up, groundX_down, groundY_down);

	loadImage_environment("mountain", far_from_start(1), groundY_up - mountain_height); // mountain height = 132
	loadImage_environment("cloud_eye", far_from_start(7), high_from_ground(9));

	InteractBlock item_block1("item_block", far_from_start(9), high_from_ground(4), false, "coin");
	loadImage_multiple_hor(1, 5, far_from_start(9 + 3), high_from_ground(4));
	InteractBlock item_block2("item_block", far_from_start(9 + 5), high_from_ground(4 + 3), false, "coin");

	// phase 2
	currentGroundBlock += 17;
	loadImage_ground(15, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down); // ground

	loadImage_environment("grass", far_from_start(currentGroundBlock + 2), groundY_up - grass_height);
	InteractBlock pipeline1("pipeline_mid", far_from_start(currentGroundBlock + 3), groundY_up - 180, false, "");

	loadImage_environment("grass", far_from_start(currentGroundBlock + 9), groundY_up - grass_height);
	InteractBlock pipeline2("pipeline_big", far_from_start(currentGroundBlock + 10), groundY_up - 240, true, "");

	loadImage_environment("cloud_eye", far_from_start(currentGroundBlock + 6), high_from_ground(10));

	// phase 3 
	currentGroundBlock += 15;
	loadImage_ground(9, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down);
	loadImage_environment("mountain", far_from_start(currentGroundBlock + 2), groundY_up - mountain_height);

	// phase 4 
	currentGroundBlock += 12;
	loadImage_ground(11, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down);
	loadImage_environment("grass", far_from_start(currentGroundBlock + 3), groundY_up - grass_height);

	loadImage_multiple_hor(1, 3, far_from_start(currentGroundBlock + 3), high_from_ground(4));
	loadImage_multiple_hor(1, 5, far_from_start(currentGroundBlock + 6), high_from_ground(7));

	// phase5
	currentGroundBlock += 14;

	loadImage_ground(11, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down);
	loadImage_multiple_hor(1, 3, far_from_start(currentGroundBlock), high_from_ground(9));
	loadImage_multiple_hor(1, 1, far_from_start(currentGroundBlock + 2), high_from_ground(4));

	loadImage_environment("cloud_eye", far_from_start(currentGroundBlock + 7), high_from_ground(12));

	loadImage_multiple_hor(1, 2, far_from_start(currentGroundBlock + 9), high_from_ground(4));
	InteractBlock cp_flag("checkpoint_flag", far_from_start(currentGroundBlock + 9), high_from_ground(4) - checkpoint_flag_height, false, ""); // checkpoint

	// phase6
	currentGroundBlock += 11;
	loadImage_ground(17, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down);

	InteractBlock item_block3("item_block", far_from_start(currentGroundBlock + 3), high_from_ground(4), false, "coin");
	InteractBlock item_block4("item_block", far_from_start(currentGroundBlock + 6), high_from_ground(4), false, "coin");
	InteractBlock item_block5("item_block", far_from_start(currentGroundBlock + 9), high_from_ground(4), false, "coin");
	InteractBlock item_block6("item_block", far_from_start(currentGroundBlock + 6), high_from_ground(8), false, "coin");


	loadImage_environment("grass", far_from_start(currentGroundBlock + 10), groundY_up - grass_height);

	loadImage_multiple_ver(4, 1, far_from_start(currentGroundBlock + 14), high_from_ground(1));
	loadImage_multiple_ver(4, 2, far_from_start(currentGroundBlock + 15), high_from_ground(1));
	loadImage_multiple_ver(4, 3, far_from_start(currentGroundBlock + 16), high_from_ground(1));

	// phase7
	currentGroundBlock += 20;
	loadImage_ground(6, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down);
	loadImage_multiple_ver(4, 3, far_from_start(currentGroundBlock), high_from_ground(1));
	loadImage_multiple_ver(4, 2, far_from_start(currentGroundBlock + 1), high_from_ground(1));

	// phase 8
	currentGroundBlock += 7;
	loadImage_ground(13, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down);

	loadImage_multiple_hor(1, 2, far_from_start(currentGroundBlock + 3), high_from_ground(4));
	loadImage_multiple_hor(1, 1, far_from_start(currentGroundBlock + 8), high_from_ground(4));
	// cloud eye
	InteractBlock cloud_eye1("cloud_eye", far_from_start(currentGroundBlock + 7), high_from_ground(9), true, "");
	InteractBlock pipeline3("pipeline_short", far_from_start(currentGroundBlock + 11), groundY_up - 120, false, "");

	// phase 9
	currentGroundBlock += 13;
	loadImage_ground(9, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down);

	loadImage_multiple_hor(4, 9, far_from_start(currentGroundBlock), high_from_ground(1));
	loadImage_multiple_hor(4, 8, far_from_start(currentGroundBlock + 1), high_from_ground(2));
	loadImage_multiple_hor(4, 7, far_from_start(currentGroundBlock + 2), high_from_ground(3));
	loadImage_multiple_hor(4, 6, far_from_start(currentGroundBlock + 3), high_from_ground(4));

	loadImage_multiple_hor(4, 5, far_from_start(currentGroundBlock + 4), high_from_ground(5));
	loadImage_multiple_hor(4, 4, far_from_start(currentGroundBlock + 5), high_from_ground(6));
	loadImage_multiple_hor(4, 3, far_from_start(currentGroundBlock + 6), high_from_ground(7));
	loadImage_multiple_hor(4, 2, far_from_start(currentGroundBlock + 7), high_from_ground(8));

	// phase 10
	currentGroundBlock += 9;
	loadImage_ground(7, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down);
	loadImage_multiple_hor(4, 1, far_from_start(currentGroundBlock + 4), high_from_ground(1));
	InteractBlock ep_flag("endpoint_flag", far_from_start(currentGroundBlock + 4) + 20, high_from_ground(10) - 20, false, "");

	// phase 11
	currentGroundBlock += 7;
	loadImage_ground(10, far_from_start(currentGroundBlock), groundY_up, far_from_start(currentGroundBlock), groundY_down);
	loadImage_environment("grass", far_from_start(currentGroundBlock - 2), groundY_up - grass_height);
	loadImage_environment("endpoint_building", far_from_start(currentGroundBlock + 2), groundY_up - endpoint_building_height);
}

void CGameStateRun::setAudio() {
	audio->Load(0, "resources/audio/map_song/field.wav"); // field song
	audio->Load(1, "resources/audio/player_audio/jump.wav"); // jump audio
	audio->Load(2, "resources/audio/player_audio/death.wav"); // death audio
	audio->Load(3, "resources/audio/interact_audio/coin.wav"); // coin audio
	// audio->Load(4, "resources/audio/interact_audio/pipe_interact.wav");
}

void CGameStateRun::setAnimation() {
	// coin
	coin_animation.LoadBitmapByString({ "resources/animation/object/coin/coin1.bmp", "resources/animation/object/coin/coin2.bmp", "resources/animation/object/coin/coin3.bmp", "resources/animation/object/coin/coin4.bmp", "resources/animation/object/coin/coin5.bmp", "resources/animation/object/coin/coin6.bmp", "resources/animation/object/coin/coin7.bmp", "resources/animation/object/coin/coin8.bmp" }, RGB(163, 73, 164));
	// interact pipe
	interact_pipe_animation.LoadBitmapByString({"resources/animation/interact_pipe/animate_1.bmp", "resources/animation/interact_pipe/animate_2.bmp", "resources/animation/interact_pipe/animate_3.bmp",
												"resources/animation/interact_pipe/animate_4.bmp", "resources/animation/interact_pipe/animate_5.bmp", "resources/animation/interact_pipe/animate_6.bmp",
												"resources/animation/interact_pipe/animate_7.bmp", "resources/animation/interact_pipe/animate_8.bmp", "resources/animation/interact_pipe/animate_9.bmp",
												"resources/animation/interact_pipe/animate_10.bmp", "resources/animation/interact_pipe/animate_11.bmp", "resources/animation/interact_pipe/animate_12.bmp",
												"resources/animation/interact_pipe/animate_13.bmp", "resources/animation/interact_pipe/animate_14.bmp", "resources/animation/interact_pipe/animate_15.bmp",
												"resources/animation/interact_pipe/animate_16.bmp", "resources/animation/interact_pipe/animate_17.bmp", "resources/animation/interact_pipe/animate_18.bmp",
												"resources/animation/interact_pipe/animate_19.bmp", "resources/animation/interact_pipe/animate_20.bmp" }, RGB(255, 242, 0));
}
/*-----------------------------------------------------------------------------------------------------*/
// init
void CGameStateRun::OnInit() // 遊戲的初值及圖形設定 set initial value and image
{
	setAudio();
	setAnimation();

	audio->Play(0, true);
	// player
	player.LoadBitmapByString(player_image, RGB(255, 242, 0));
	player.SetTopLeft(120, groundY_up - player.GetHeight());

	// set initial checkpoint
	current_checkpoint_x = 120;
	current_checkpoint_y = groundY_up - player.GetHeight();

	// game over
	game_over_image.LoadBitmapByString({ "resources/image/logo/game_over1.bmp", "resources/image/logo/game_over2.bmp" }, RGB(163, 73, 164));
	game_over_image.SetFrameIndexOfBitmap(1);
	game_over_image.SetTopLeft(0, 0);

	setMap1(player);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == VK_LEFT) {
		keyLeft = true;
		keyRight = false;
	}
	if (nChar == VK_RIGHT) {
		keyRight = true;
		keyLeft = false;
	}
	if (nChar == VK_DOWN) {
		keyDown = true;
	}
	if (nChar == VK_UP) {
		keyUp = true;
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT) {
		keyRight = false;
	}
	if (nChar == VK_LEFT) {
		keyLeft = false;
	}
	if (nChar == VK_UP) {
		keyUp = false;
	}
	if (nChar == VK_DOWN) {
		keyDown = false;
	}
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作 left mouse button (down)
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作 left mouse button (up)
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作 
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{

	show_ground();
	show_ver();
	show_hor();
	show_enemy();
	show_environment();
	if (animation_flag == true) {
		coin_animation.ShowBitmap();
		interact_pipe_animation.ShowBitmap();
	}

	InteractBlock::showImage();
	game_over_image.ShowBitmap();
	if (game_over == true) {
		CDC *pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 120, "Courier New", RGB(255, 255, 255), 20);
		CTextDraw::Print(pDC, 500, 400, std::to_string(game_over_count));
		CDDraw::ReleaseBackCDC();
	}

	player.ShowBitmap();
}