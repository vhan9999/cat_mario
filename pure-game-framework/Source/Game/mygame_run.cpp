#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <vector>
using namespace game_framework;



/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// This class is the game execution object of the game, and the main game programs are here
// select multiple line = alt + shift + .
/////////////////////////////////////////////////////////////////////////////

bool isMoveLeft = false;
bool isMoveRight = false;
bool isMoveUp = false;
bool isMoveDown = false;
bool isAnimated = false;
bool isCollideEnemy = false;
int i = 0; // index loop vector

// ground_brick array
std::vector<CMovingBitmap> ground_brick_arr1; // ground_brick 1 
std::vector<CMovingBitmap> ground_brick_arr2; // ground_brick 2
std::vector<CMovingBitmap> ground_brick_arr3; // ground_brick 3

/* ----CLASS---- */

// Brick Factory
class BrickFactory {
public:
	static CMovingBitmap createBrick(int type, int x, int y) {
		CMovingBitmap brick;
		switch (type) {
		case 3: // brick3
			brick.LoadBitmapByString({"resources/image/object/block1/brown_brick3.bmp"}, RGB(64, 128, 128));
		case 5: // brick5
			brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick5.bmp" }, RGB(64, 128, 128));
		default:
			break;
		}
		brick.SetFrameIndexOfBitmap(0);
		brick.SetTopLeft(x, y);
		return brick;
	}
};
	
/* ----FUNCTION---- */
void build_brick_horizontal(std::vector<CMovingBitmap> &ground_brick_arr, int type, int amount, int x, int y) {
	CMovingBitmap brick;
	for (int i = 0; i < amount; i++) { // ground brick up
		brick = BrickFactory::createBrick(type, x, y);
		ground_brick_arr.push_back(brick);
		x += 32-2; // -2 pixel remaining pixel
	}
}

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
	if (isCollideEnemy == false) {
		// player movement
		if (isMoveLeft) {
			player.SetTopLeft(player.GetLeft() - 2, player.GetTop());
		}
		if (isMoveRight) {
			player.SetTopLeft(player.GetLeft() + 2, player.GetTop());
		}

		// player restriction (resolution(x,y) 800 X 600)
		if (player.GetLeft() + player.GetWidth() <= 32) {
			player.SetTopLeft(0, player.GetTop());
		}
	}

	// enemy collision
	if ((player.GetLeft() + player.GetWidth())-5 >= enemy.GetLeft()) {
		isCollideEnemy = true;
	}
}

void CGameStateRun::OnInit() // 遊戲的初值及圖形設定 set initial value and image
{	
	
	// player
	player.LoadBitmapByString({ "resources/image/player/player_1.bmp", "resources/image/player/player_2.bmp" }, RGB(255, 201, 14)); // player color background (255, 201, 14)
	player.SetFrameIndexOfBitmap(0);
 	player.SetTopLeft(100, 536-player.GetHeight()-32);

	// build groundbrick
	
	// ground brick up
	int groundX_up = 0;
	int groundY_up = 536 - 32;
	build_brick_horizontal(ground_brick_arr1, 3, 23, groundX_up, groundY_up);

	// ground brick mid
	int groundX_mid = 0;
	int groundY_mid = 536-2; // -2 ramaining pixel
	build_brick_horizontal(ground_brick_arr2, 5, 23, groundX_mid, groundY_mid);

	// ground brick mid
	int groundX_down = 0;
	int groundY_down = 566-2;
	build_brick_horizontal(ground_brick_arr3, 5, 23, groundX_down, groundY_down);

	// enemy
	enemy.LoadBitmapByString({ "resources/image/enemy/normal.bmp" }, RGB(64, 128, 128));
	enemy.SetFrameIndexOfBitmap(0);
	enemy.SetTopLeft(200, 536 - enemy.GetHeight() - 32);

	// brick
	int brick_X = 0;
	int brick_Y = 504 - 32+2;
	brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick3.bmp" }, RGB(64, 128, 128));
	brick.SetFrameIndexOfBitmap(0);
	brick.SetTopLeft(brick_X, brick_Y);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT) {
		isMoveLeft = true;
		// animate moving  
		player.SetFrameIndexOfBitmap(0);
		player.SetAnimation(100, false);
		player.SetFrameIndexOfBitmap(1);
	}
	if (nChar == VK_RIGHT) {
		isMoveRight = true;
		// animate moving  
		player.SetFrameIndexOfBitmap(0);
		player.SetAnimation(100, false);
		player.SetFrameIndexOfBitmap(1);
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT) {
		isMoveLeft = false;
		// stop animate moving
		player.SetAnimation(100, true);
	}
	if (nChar == VK_RIGHT) {
		isMoveRight = false;
		// stop animate moving
		player.SetAnimation(100, true);
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
	// use 'auto' for iterate to avoid "signed/unsigned mismatch"
	for (auto i : ground_brick_arr1) { i.ShowBitmap();} // ground brick up
	for (auto i : ground_brick_arr2) { i.ShowBitmap();} //ground brick mid
	for (auto i : ground_brick_arr3) { i.ShowBitmap(); } //ground brick down

	player.ShowBitmap();
	enemy.ShowBitmap();

	brick.ShowBitmap();
}
