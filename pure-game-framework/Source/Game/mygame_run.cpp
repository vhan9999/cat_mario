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



/* ----VARIABLE---- */
/*-----------------------------------------------------------------------------------------------------*/

// ground brick coordinate
int groundX_up = 0;
int groundY_up = 716;
int groundX_mid = 0;
int groundY_mid = 776;
int groundX_down = 0;
int groundY_down = 836;

// ground_brick array
std::vector<CMovingBitmap> ground_brick_arr1; // ground_brick 1 
std::vector<CMovingBitmap> ground_brick_arr2; // ground_brick 2
std::vector<CMovingBitmap> ground_brick_arr3; // ground_brick 3

// vertical brick
std::vector<CMovingBitmap> ver_brick_arr;
/*-----------------------------------------------------------------------------------------------------*/


/* ----CLASS---- */
/*-----------------------------------------------------------------------------------------------------*/
// Brick Factory
class BrickFactory {
public:
	static CMovingBitmap createBrick(int type, int x, int y) {
		CMovingBitmap brick;
		switch (type) {
		case 3: // brick3
			brick.LoadBitmapByString({"resources/image/object/block1/brown_brick3.bmp"}, RGB(163, 73, 164));
		case 4: //brick4
			brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick4.bmp" }, RGB(163, 73, 164));
		case 5: // brick5
			brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick5.bmp" }, RGB(163, 73, 164));
		default:
			break;
		}
		brick.SetFrameIndexOfBitmap(0);
		brick.SetTopLeft(x, y);
		return brick;
	}
};
/*-----------------------------------------------------------------------------------------------------*/




/* ----FUNCTION---- */
/*-----------------------------------------------------------------------------------------------------*/
void build_brick_horizontal(std::vector<CMovingBitmap> &brick_arr, int type, int amount, int x, int y) { // build brick horizontally
	CMovingBitmap brick;
	for (int i = 0; i < amount; i++) { // ground brick up
		brick = BrickFactory::createBrick(type, x, y);
		brick_arr.push_back(brick);
		x += 60; 
	}
}

void build_brick_vertical(std::vector<CMovingBitmap> &brick_arr, int type, int amount, int x, int y) {
	CMovingBitmap brick;
	for (int i = 0; i < amount; i++) {
		brick = BrickFactory::createBrick(type, x, y);
		brick_arr.push_back(brick);
		y -= 60;
	}
}

// load and show ground brick image
void loadBitMap_ground(int amount) {
	// ground brick up
	build_brick_horizontal(ground_brick_arr1, 3, amount, groundX_up, groundY_up);

	// ground brick mid
	build_brick_horizontal(ground_brick_arr2, 5, amount, groundX_mid, groundY_mid);

	// ground brick down
	build_brick_horizontal(ground_brick_arr3, 5, amount, groundX_down, groundY_down);
}
void showBitMap_ground() {
	// use 'auto' for iterate to avoid "signed/unsigned mismatch"
	for (auto i : ground_brick_arr1) { i.ShowBitmap(); } // ground brick up
	for (auto i : ground_brick_arr2) { i.ShowBitmap(); } //ground brick mid
	for (auto i : ground_brick_arr3) { i.ShowBitmap(); } //ground brick down
}

// load and show vertical brick
void loadBitMap_vertical(int amount, int x, int y) {
	build_brick_vertical(ver_brick_arr, 4, amount, x, y);
}
void showBitMap_vertical() {
	for (auto i : ver_brick_arr) { i.ShowBitmap(); }
}

// check value is in range [min, max] or not
bool inRange(double num, double min, double max) {
	return (min <= num && num <= max);
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

// collide single object
void CGameStateRun::singleBlockCollision(CMovingBitmap &block, CMovingBitmap &player) {
	if ((player.GetTop() + player.GetHeight() >= block.GetTop()) && (player.GetTop() <= block.GetTop() + block.GetHeight())) { // compare player height and block height
		if (inRange(player.GetLeft() + player.GetWidth(), block.GetLeft(), block.GetLeft() + 7)) { // left
			player.SetTopLeft(block.GetLeft() - player.GetWidth(), player.GetTop());
		}
		else if (inRange(player.GetLeft(), block.GetLeft() + block.GetWidth() - 7, block.GetLeft() + block.GetWidth())) { // right
			player.SetTopLeft(block.GetLeft() + block.GetWidth(), player.GetTop());
		}
	}
	if (inRange(player.GetLeft() + player.GetWidth(), block.GetLeft() + 10, (block.GetLeft() + block.GetWidth()) / 2) || inRange(player.GetLeft(), (block.GetLeft() + block.GetWidth()) / 2, block.GetLeft() + block.GetWidth() - 10)) { // upper
		if (player.GetTop() + player.GetHeight() >= block.GetTop()) {
			player.SetTopLeft(player.GetLeft(), block.GetTop() - player.GetHeight());
		}
	}
}

// collide enemy
void CGameStateRun::singleEnemyCollision(CMovingBitmap &enemy, CMovingBitmap &player, int &frame, int &jumpBonusFrame) {
	if ((player.GetTop() + player.GetHeight() >= enemy.GetTop()) && (player.GetTop() <= enemy.GetTop() + enemy.GetHeight())) { // compare player height and enemy height
		if (inRange(player.GetLeft() + player.GetWidth(), enemy.GetLeft(), enemy.GetLeft() + 7)) { // left
			frame = 0; moveSpeed = 0; jumpBonusFrame = 0; jumpSpeed = 0;
		}
		if (inRange(player.GetLeft(), enemy.GetLeft() + enemy.GetWidth() - 7, enemy.GetLeft() + enemy.GetWidth())) { // right
			frame = 0; moveSpeed = 0; jumpBonusFrame = 0; jumpSpeed = 0;
		}
	}

}

// move Horizontal
void CGameStateRun::moveHor() {
	if (keyRight == true) {//move right
		if (frame %10 == 0) {//every 10 frame
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
	jumpBonusFrame++;
	if (player.GetTop() < groundY_up - player.GetHeight()) {// 重力
		jumpSpeed += 1;
	}
	else if (keyUp && player.GetTop() == groundY_up - player.GetHeight()) {// touch ground jump
		jumpBonusFrame = 0;
		jumpSpeed = -19;
	}
	if (jumpBonusFrame == 5 && keyUp) {// jump hold duration (if hold long will higher)
		isBigJump = true;
		jumpSpeed -= 5;
	}
}

void CGameStateRun::OnInit() // 遊戲的初值及圖形設定 set initial value and image
{		
	// player
	player.LoadBitmapByString({ "resources/image/player/player_1.bmp"}, RGB(255, 242, 0)); 
	player.SetFrameIndexOfBitmap(0);
 	player.SetTopLeft(300, groundY_up-player.GetHeight());

	// ground brick
	loadBitMap_ground(15);

	// enemy
	enemy.LoadBitmapByString({ "resources/image/enemy/normal.bmp" }, RGB(163, 73, 164));
	enemy.SetFrameIndexOfBitmap(0);
	enemy.SetTopLeft(700, groundY_up-enemy.GetHeight());

	// brick
	brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick3.bmp" }, RGB(163, 73, 164));
	brick.SetFrameIndexOfBitmap(0);
	brick.SetTopLeft(90, groundY_up - brick.GetHeight());

	// vertical brick
	loadBitMap_vertical(4, 420, 656);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
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
	showBitMap_ground();
	player.ShowBitmap();
	enemy.ShowBitmap();

	brick.ShowBitmap();
	showBitMap_vertical();
}
