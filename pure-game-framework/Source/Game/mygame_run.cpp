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
// Brick Factory
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

	static CMovingBitmap createEnemy(std::string name, int x, int y) {
		CMovingBitmap enemy;
		if (name == "normal") {
			enemy.LoadBitmapByString({ "resources/image/enemy/normal.bmp" }, RGB(163, 73, 164));
		}
		enemy.SetFrameIndexOfBitmap(0);
		enemy.SetTopLeft(x, y);
		return enemy;
	}
};
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
	if (current_ground_arr_flag % 3 == 0) {
		upper_ground_brick_arr.push_back(block_arr);
	}
	else {
		rem_ground_brick_arr.push_back(block_arr);
	}
}

// load ground 
void CGameStateRun::loadImage_ground(int amount, int x_up, int y_up, int x_mid, int y_mid, int x_down, int y_down) {
	build_block_ground(3, amount, x_up, y_up); // ground brick up
	build_block_ground(5, amount, x_mid, y_mid); // ground brick mid
	build_block_ground(5, amount, x_down, y_down); // ground brick down
}

void CGameStateRun::show_ground() {
	for (auto i : upper_ground_brick_arr) { for (auto j : i) j.ShowBitmap(); }
	for (auto i : rem_ground_brick_arr) { for (auto j : i) j.ShowBitmap(); }
}

// build multiple vertical block
void CGameStateRun::loadImage_multiple_ver(int type, int amount, int x, int y) { // build block multiple (vertical)
	std::vector<CMovingBitmap> block_arr;
	CMovingBitmap brick;
	for (int i = 0; i < amount; i++) { // ground brick up
		brick = ImageFactory::createBrick(type, x, y);
		block_arr.push_back(brick);
		y -= 60;
	}
	ver_block_arr.push_back(block_arr);
}

// load multiple vertical horizontal
void CGameStateRun::loadImage_multiple_hor(int type, int amount, int x, int y) { // build block multiple (horizontal)
	std::vector<CMovingBitmap> block_arr;
	CMovingBitmap brick;
	for (int i = 0; i < amount; i++) { // ground brick up
		brick = ImageFactory::createBrick(type, x, y);
		block_arr.push_back(brick);
		x += 60;
	}
	hor_block_arr.push_back(block_arr);
}

void CGameStateRun::show_hor() {
	for (auto i : hor_block_arr) { for (auto j : i) { j.ShowBitmap(); } }
}

void CGameStateRun::show_ver() {
	for (auto i : ver_block_arr) { for (auto j : i) { j.ShowBitmap(); } }
}

// check value is in range [min, max] or not
bool CGameStateRun::inRange(double num, double min, double max) {
	return (min <= num && num <= max);
}

// able to jump
void CGameStateRun::ableToJump(int &jumpSpd, int &jumpBns, double &ground) {
	jumpBns++; // frame
	if (player.GetTop() < ground) {// player in the air
		jumpSpd += 1; // v += a 
	}
	else if (keyUp && player.GetTop() == ground) {// touch ground jump
		jumpBns = 0; // for big jump 
		jumpSpd = -19;//v0
	}
	if (jumpBns == 5 && keyUp) {// jump hold duration (if hold long will higher)
		jumpSpd -= 5; // v-=5(a)
	}
}

// collision vertical
void CGameStateRun::check_collision_ver(std::vector<CMovingBitmap> &arr, CMovingBitmap &player) {
	int obj_left = arr[0].GetLeft();
	int obj_right = arr[0].GetLeft() + arr[0].GetWidth();
	int obj_top = arr[arr.size() - 1].GetTop();
	int obj_bottom = arr[0].GetTop() + arr[0].GetHeight();
	int obj_height = arr[0].GetHeight();
	// detect left/right side collision of block
	bool collUp = inRange(player.GetTop(), obj_top, obj_bottom);
	bool collUpMid = inRange((player.GetTop() + player.GetHeight()) / 4, obj_top, obj_bottom);
	bool collMid = inRange((player.GetTop() + player.GetHeight()) / 2, obj_top, obj_bottom);
	bool collDown = inRange(player.GetTop() + player.GetHeight(), obj_top, obj_bottom);
	bool isCollideLeftSide = inRange(player.GetLeft() + player.GetWidth(), obj_left, obj_left + 4);
	bool isCollideRightSide = inRange(player.GetLeft(), obj_right - 4, obj_right);
	// left side of block
	if ((isCollideLeftSide == true) && (collUp == true || collUpMid == true || collMid == true || collDown == true)) {
		moveSpeed = 0;
		player.SetTopLeft(obj_left - player.GetWidth(), player.GetTop());
		frame += 2;
	}
	// right side of block
	if ((isCollideRightSide == true) && ((collUp == true || collUpMid == true || collMid == true || collDown == true))) {
		moveSpeed = 0;
		player.SetTopLeft(obj_right, player.GetTop());
		frame += 2;
	}
	// upper side of block
	bool Left = inRange(player.GetLeft() + player.GetWidth(), obj_left + 4, obj_right - 4);
	bool Right = inRange(player.GetLeft(), obj_left + 4, obj_right - 4);
	bool isCollideUpperBrick = inRange(player.GetTop() + player.GetHeight(), obj_top, obj_top + 29.99);
	if ((Left == true || Right == true) && (isCollideUpperBrick == true)) {
		jumpSpeed = 0;
		jumpBonusFrame = 0;
		player.SetTopLeft(player.GetLeft(), obj_top - player.GetHeight());
		double ground = obj_top - player.GetHeight();
		CGameStateRun::ableToJump(jumpSpeed, jumpBonusFrame, ground); // can jump on block
	}
	else {
		player.SetTopLeft(player.GetLeft(), player.GetTop());
	}
	// bottom side of block
	bool atLeft = inRange(player.GetLeft() + player.GetWidth(), obj_left + 15, obj_right + 15);
	bool atRight = inRange(player.GetLeft(), obj_left + 15, obj_right - 15);
	bool isCollideBottomBrick = inRange(player.GetTop(), (obj_top + (obj_height / 2)), obj_bottom);
	if ((atLeft == true || atRight == true) && isCollideBottomBrick == true) {
		jumpSpeed = 0;
		player.SetTopLeft(player.GetLeft(), obj_bottom);
		jumpSpeed += 1;
	}
	bool atLeftSide = inRange(player.GetLeft() + player.GetWidth(), obj_left, obj_left + 14.99);
	bool atRightSide = inRange(player.GetLeft(), obj_right - 14.9, obj_right);
	if (atLeftSide == true && isCollideBottomBrick == true) { player.SetTopLeft(obj_left - player.GetWidth(), player.GetTop()); }
	if (atRightSide == true && isCollideBottomBrick == true) { player.SetTopLeft(obj_right, player.GetTop()); }
}

// collision horizontal
void CGameStateRun::check_collision_hor(std::vector<CMovingBitmap> &arr, CMovingBitmap &player) {
	int obj_left = arr[0].GetLeft();
	int obj_right = arr[arr.size() - 1].GetLeft() + arr[arr.size() - 1].GetWidth();
	int obj_top = arr[0].GetTop();
	int obj_bottom = arr[0].GetTop() + arr[0].GetHeight();
	int obj_height = arr[0].GetHeight();

	// detect left/right side collision of block
	bool collUp = inRange(player.GetTop(), obj_top, obj_bottom);
	bool collUpMid = inRange((player.GetTop() + player.GetHeight()) / 4, obj_top, obj_bottom);
	bool collMid = inRange((player.GetTop() + player.GetHeight()) / 2, obj_top, obj_bottom);
	bool collDown = inRange(player.GetTop() + player.GetHeight(), obj_top, obj_bottom);
	bool isCollideLeftSide = inRange(player.GetLeft() + player.GetWidth(), obj_left, obj_left + 4);
	bool isCollideRightSide = inRange(player.GetLeft(), obj_right - 4, obj_right);
	// left side of block
	if ((isCollideLeftSide == true) && (collUp == true || collUpMid == true || collMid == true || collDown == true)) {
		moveSpeed = 0;
		player.SetTopLeft(obj_left - player.GetWidth(), player.GetTop());
		frame += 2;
	}
	// right side of block
	if ((isCollideRightSide == true) && ((collUp == true || collUpMid == true || collMid == true || collDown == true))) {
		moveSpeed = 0;
		player.SetTopLeft(obj_right, player.GetTop());
		frame += 2;
	}
	// upper side of block
	bool Left = inRange(player.GetLeft() + player.GetWidth(), obj_left + 4, obj_right - 4);
	bool Right = inRange(player.GetLeft(), obj_left + 4, obj_right - 4);
	bool isCollideUpperBrick = inRange(player.GetTop() + player.GetHeight(), obj_top, obj_top + 29.99);
	if ((Left == true || Right == true) && (isCollideUpperBrick == true)) {
		jumpSpeed = 0;
		jumpBonusFrame = 0;
		player.SetTopLeft(player.GetLeft(), obj_top - player.GetHeight());
		double ground = obj_top - player.GetHeight();
		CGameStateRun::ableToJump(jumpSpeed, jumpBonusFrame, ground); // can jump on block
	}
	else {
		player.SetTopLeft(player.GetLeft(), player.GetTop());
	}
	// bottom side of block
	bool atLeft = inRange(player.GetLeft() + player.GetWidth(), obj_left + 15, obj_right + 15);
	bool atRight = inRange(player.GetLeft(), obj_left + 15, obj_right - 15);
	bool isCollideBottomBrick = inRange(player.GetTop(), (obj_top + (obj_height / 2)), obj_top + obj_height);
	if ((atLeft == true || atRight == true) && isCollideBottomBrick == true) {
		jumpSpeed = 0;
		player.SetTopLeft(player.GetLeft(), obj_bottom);
		jumpSpeed += 1;
	}
	bool atLeftSide = inRange(player.GetLeft() + player.GetWidth(), obj_left, obj_left + 14.99);
	bool atRightSide = inRange(player.GetLeft(), obj_right - 14.9, obj_right);
	if (atLeftSide == true && isCollideBottomBrick == true) { player.SetTopLeft(obj_left - player.GetWidth(), player.GetTop()); }
	if (atRightSide == true && isCollideBottomBrick == true) { player.SetTopLeft(obj_right, player.GetTop()); }
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

/*-----------------------------------------------------------------------------------------------------*/
/* ---- CGameStateRun ---- */
CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{

}

void CGameStateRun::OnMove()  // 移動遊戲元素 move (always loop)
{
	frame++;//用來判斷幀數
	moveHor();
	moveVer();
	// gravity and moving
	if (player.GetTop() + jumpSpeed >= 1500) {// fall down
		player.SetTopLeft(player.GetLeft() + moveSpeed, 1500);
		jumpSpeed = 0;
	}
	else if (moveSpeed != 0 || jumpSpeed != 0) {//move
		player.SetTopLeft(player.GetLeft() + moveSpeed, player.GetTop() + jumpSpeed);
	}
	if (frame + 1 < 0) {//到int的上限後 歸零
		frame = 0;
	}
	// player restriction
	if (player.GetLeft() <= 0) {
		player.SetTopLeft(0, player.GetTop());
	}

	// enemy collision
	// CGameStateRun::singleEnemyCollision(enemy, player, frame, jumpBonusFrame);
	for (auto i : upper_ground_brick_arr) { CGameStateRun::check_ground_collision(i, player);} // collision ground
	for (auto i : ver_block_arr) { CGameStateRun::check_collision_ver(i, player); } // collision check vertical
	for (auto i : hor_block_arr) { CGameStateRun::check_collision_hor(i, player);} // collision check horizontal 
}

// move Horizontal
void CGameStateRun::moveHor() {
	if (keyRight == true) {//move right
		if (frame % 10 == 0) {//every 10 frame
			moveSpeed += 3;
			if (moveSpeed < 0)
				moveSpeed++;
		}
		if (moveSpeed >= 6)//speed max = 6
			moveSpeed = 6;
	}
	if (keyLeft == true) {//move left

		if (frame % 10 == 0) {
			moveSpeed -= 3;
			if (moveSpeed > 0)
				moveSpeed--;
		}
		if (moveSpeed <= -6)
			moveSpeed = -6;
	}
	if ((!keyRight && !keyLeft && moveSpeed != 0)) {// stop
		if (frame % 5 == 0) {
			if (moveSpeed > 1)
				moveSpeed -= 1;
			else if (moveSpeed < -1)
				moveSpeed += 1;
			else
				moveSpeed = 0;
		}
	}
}

// jump
void CGameStateRun::moveVer()
{
	double fall_ground = 1500; 
	ableToJump(jumpSpeed, jumpBonusFrame, fall_ground);
}

// init
void CGameStateRun::OnInit() // 遊戲的初值及圖形設定 set initial value and image
{
	// player
	player.LoadBitmapByString({ "resources/image/player/player_1.bmp" }, RGB(255, 242, 0));
	player.SetFrameIndexOfBitmap(0);
	player.SetTopLeft(600 + 60 - 13, groundY_up - player.GetHeight());
	player.SetTopLeft(120, 500);
	
	// enemy
	//loadImage_enemy("normal", 540, groundY_up-54);

	// ground brick
	loadImage_ground(8, groundX_up, groundY_up, groundX_mid, groundY_mid, groundX_down, groundY_down);
	loadImage_ground(8, 60*11, groundY_up, 60*11, groundY_mid, 60*11, groundY_down);

	// front brick
	loadImage_multiple_hor(2, 2, 60, high_from_ground(1));

	// vertical brick (stair)
	loadImage_multiple_ver(1, 4, 420, high_from_ground(1));
	loadImage_multiple_ver(1, 3, 360, high_from_ground(1));
	loadImage_multiple_ver(1, 2, 300, high_from_ground(1));
	loadImage_multiple_ver(1, 1, 240, high_from_ground(1));

	// lower sky brick 
	loadImage_multiple_hor(1, 1, 540, high_from_ground(3));
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == 0x25) {
		keyLeft = true;
		keyRight = false;
	}
	if (nChar == 0x27) {
		keyRight = true;
		keyLeft = false;
	}
	if (nChar == 0x26) {
		keyUp = true;
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 0x27) {
		keyRight = false;
	}
	if (nChar == 0x25) {
		keyLeft = false;
	}
	if (nChar == 0x26) {
		keyUp = false;
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

	player.ShowBitmap();
}
