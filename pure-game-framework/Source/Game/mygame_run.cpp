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

// vertical block
std::vector<CMovingBitmap> ver_block_arr;
// horizontal block
std::vector<CMovingBitmap> hor_block_arr;
/*-----------------------------------------------------------------------------------------------------*/


/* ----CLASS---- */
/*-----------------------------------------------------------------------------------------------------*/
// Brick Factory
class BrickFactory {
public:
	static CMovingBitmap createBrick(int type, int x, int y) {
		CMovingBitmap brick;
		switch (type) {
		case 1: // brick1
			brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick.bmp" }, RGB(163, 73, 164));
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
void build_block_vertical(std::vector<CMovingBitmap> &block_arr, int type, int amount, int x, int y) { // build brick vertically
	CMovingBitmap brick;
	for (int i = 0; i < amount; i++) {
		brick = BrickFactory::createBrick(type, x, y);
		block_arr.push_back(brick);
		y -= 60;
	}
}
void build_block_horizontal(std::vector<CMovingBitmap> &brick_arr, int type, int amount, int x, int y) { // build brick horizontally
	CMovingBitmap brick;
	for (int i = 0; i < amount; i++) { // ground brick up
		brick = BrickFactory::createBrick(type, x, y);
		brick_arr.push_back(brick);
		x += 60;
	}
}
// load and show 
void showBitMap_single() {
	for (auto i : ver_block_arr) { i.ShowBitmap(); }
}
void loadImage_ground(int amount) { // ground brick image
	build_block_horizontal(ground_brick_arr1, 3, amount, groundX_up, groundY_up); // ground brick up
	build_block_horizontal(ground_brick_arr2, 5, amount, groundX_mid, groundY_mid); // ground brick mid
	build_block_horizontal(ground_brick_arr3, 5, amount, groundX_down, groundY_down); // ground brick down
}
void showBitMap_ground() {
	// use 'auto' for iterate to avoid "signed/unsigned mismatch"
	for (auto i : ground_brick_arr1) { i.ShowBitmap(); } // ground brick up
	for (auto i : ground_brick_arr2) { i.ShowBitmap(); } //ground brick mid
	for (auto i : ground_brick_arr3) { i.ShowBitmap(); } //ground brick down
}
// vertical block
void loadImage_vertical(int type, int amount, int x, int y) { 
	build_block_vertical(ver_block_arr, type, amount, x, y);
}
void showBitMap_vertical() {
	for (auto i : ver_block_arr) { i.ShowBitmap(); }
}
// horizontal block
void loadImage_horizontal(int type, int amount, int x, int y) {
	build_block_horizontal(hor_block_arr, type, amount, x, y);
}
void showBitMap_horizontal() {
	for (auto i : hor_block_arr) { i.ShowBitmap(); }
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
void CGameStateRun::blockCollision(CMovingBitmap &block, CMovingBitmap &player) {
	// detect left/right side collision of block
	bool collUp = inRange(player.GetTop(), block.GetTop(), block.GetTop()+block.GetHeight());
	bool collUpMid = inRange((player.GetTop() + player.GetHeight()) / 4, block.GetTop(), block.GetTop() + block.GetHeight());
	bool collMid = inRange((player.GetTop()+player.GetHeight())/2, block.GetTop(), block.GetTop() + block.GetHeight());
	bool collDown = inRange(player.GetTop() + player.GetHeight(), block.GetTop(), block.GetTop() + block.GetHeight());
	// left
	if (inRange(player.GetLeft()+player.GetWidth(),block.GetLeft(), block.GetLeft()+4) && (collUp == true || collMid == true || collDown == true || collUpMid== true)) { 
		player.SetTopLeft(block.GetLeft()-player.GetWidth(), player.GetTop());
		moveSpeed = 0;
	}
	// right
	if (inRange(player.GetLeft(), block.GetLeft() + block.GetWidth() - 4, block.GetLeft() + block.GetWidth()) && (collUp == true || collMid == true || collDown == true || collUpMid==true)) {
		player.SetTopLeft(block.GetLeft()+block.GetWidth(), player.GetTop());
		moveSpeed = 0;
	}
	// detect upper/lower side collision of block
	bool atLeft = inRange(player.GetLeft() + player.GetWidth(), block.GetLeft()+15, (block.GetLeft()+block.GetWidth())+15);
	bool atRight = inRange(player.GetLeft(), block.GetLeft()+15, (block.GetLeft() + block.GetWidth())-15);
	bool collideBottomBrick = inRange(player.GetTop(), (block.GetTop() + (block.GetHeight()/2)), block.GetTop() + block.GetHeight());
	// down
	if ((atLeft == true || atRight==true ) && collideBottomBrick==true) {
		jumpSpeed = 0;
		player.SetTopLeft(player.GetLeft(), block.GetTop() + block.GetHeight());
		jumpSpeed += 1;	
	}
	bool atDownLeft = inRange(player.GetLeft() + player.GetWidth(), block.GetLeft(), block.GetLeft() + 14.5);
	bool atDownRight = inRange(player.GetLeft(), (block.GetLeft() + block.GetWidth()) - 14.5, block.GetLeft() + block.GetWidth());
	if (atDownLeft == true && collideBottomBrick == true) { player.SetTopLeft(block.GetLeft() - player.GetWidth(), player.GetTop());}
	if (atDownRight == true && collideBottomBrick == true) { player.SetTopLeft(block.GetLeft() + block.GetWidth(), player.GetTop()); }
}

// collide enemy
void CGameStateRun::singleEnemyCollision(CMovingBitmap &enemy, CMovingBitmap &player, int &frame, int &jumpBonusFrame) {
	if ((player.GetTop() + player.GetHeight() >= enemy.GetTop()) && (player.GetTop() <= enemy.GetTop() + enemy.GetHeight())) { // compare player height and enemy height
		if (inRange(player.GetLeft() + player.GetWidth(), enemy.GetLeft(), enemy.GetLeft() + 6)) { // left
			frame = 0; moveSpeed = 0; jumpBonusFrame = 0; jumpSpeed = 0;
		}
		if (inRange(player.GetLeft(), enemy.GetLeft() + enemy.GetWidth() - 6, enemy.GetLeft() + enemy.GetWidth())) { // right
			frame = 0; moveSpeed = 0; jumpBonusFrame = 0; jumpSpeed = 0;
		}
	}
}

void CGameStateRun::OnMove()  // 移動遊戲元素 move (always loop)
{
	frame++;//用來判斷幀數
	moveHor();
	moveVer();
	if (player.GetTop() + jumpSpeed > groundY_up - player.GetHeight()) {//touch ground
		player.SetTopLeft(player.GetLeft() + moveSpeed, groundY_up - player.GetHeight());
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
	// block collision
	CGameStateRun::blockCollision(brick, player);
	// sky block collision
	CGameStateRun::blockCollision(sky_brick, player);
	// enemy collision
	CGameStateRun::singleEnemyCollision(enemy, player, frame, jumpBonusFrame);
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
	jumpBonusFrame++; // frame
	if (player.GetTop() < groundY_up - player.GetHeight()) {// player in the air
		jumpSpeed += 1; // v += a 
	}
	else if (keyUp && player.GetTop() == groundY_up - player.GetHeight()) {// touch ground jump
		jumpBonusFrame = 0; // for big jump 
		jumpSpeed = -19;//v0
	}
	if (jumpBonusFrame == 5 && keyUp) {// jump hold duration (if hold long will higher)
		jumpSpeed -= 5; // v-=5(a)
	}
}

void CGameStateRun::OnInit() // 遊戲的初值及圖形設定 set initial value and image
{		
	// player
	player.LoadBitmapByString({ "resources/image/player/player_1.bmp"}, RGB(255, 242, 0)); 
	player.SetFrameIndexOfBitmap(0);
 	player.SetTopLeft(600+60-13, groundY_up-player.GetHeight());

	// ground brick
	loadImage_ground(17);

	// enemy
	enemy.LoadBitmapByString({ "resources/image/enemy/normal.bmp" }, RGB(163, 73, 164));
	enemy.SetFrameIndexOfBitmap(0);
	enemy.SetTopLeft(840, groundY_up - 54);

	// brick
	brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick3.bmp" }, RGB(163, 73, 164));
	brick.SetFrameIndexOfBitmap(0);
	brick.SetTopLeft(60, groundY_up - brick.GetHeight());

	// vertical brick
	loadImage_vertical(1, 4, 420, groundY_up-60);
	// Horizontal sky brick
	loadImage_horizontal(1, 3, 780, groundY_up-240);
	// sky brick
	sky_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick.bmp" }, RGB(163, 73, 164));
	sky_brick.SetFrameIndexOfBitmap(0);
	sky_brick.SetTopLeft(600, groundY_up - sky_brick.GetHeight()*4);
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
	showBitMap_vertical();
	showBitMap_horizontal();

	player.ShowBitmap();
	enemy.ShowBitmap();
	brick.ShowBitmap();
	sky_brick.ShowBitmap();
}
