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
/////////////////////////////////////////////////////////////////////////////

bool isMoveLeft = false;
bool isMoveRight = false;
bool isMoveUp = false;
bool isMoveDown = false;
bool isAnimated = false;
int i = 0;
std::vector<CMovingBitmap>  block_arr;

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

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
	
}

void CGameStateRun::OnMove()							// 移動遊戲元素 move (always loop)
{
	// player movement
	if (isMoveLeft) {
		player.SetTopLeft(player.GetLeft() - 3, player.GetTop());
	}
	if (isMoveRight) {
		player.SetTopLeft(player.GetLeft() + 3, player.GetTop());
	}
	
	// player restriction (resolution(x,y) 800 X 600)
	if (player.GetLeft()+player.GetWidth()<= 32) {
		player.SetTopLeft(0, player.GetTop());
	}

	// brick collision
	// if (player.GetLeft() + player.GetWidth() >= enemy.GetLeft()) {
	//	player.SetTopLeft(enemy.GetLeft()-27, player.GetTop());
	// }
}

void CGameStateRun::OnInit() // 遊戲的初值及圖形設定 set initial value and image
{	
	
	player.LoadBitmapByString({ "resources/image/player/player_1.bmp", "resources/image/player/player_2.bmp" }, RGB(255, 201, 14)); // player color background (255, 201, 14)
	player.SetFrameIndexOfBitmap(0);
 	player.SetTopLeft(100, 536-player.GetHeight()-32);

	CMovingBitmap ground_brick2;
	ground_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick3.bmp"}, RGB(64, 128, 128));
	ground_brick.SetFrameIndexOfBitmap(0);
	ground_brick.SetTopLeft(90,536-32);
	
	// build groundbrick
	int block_posX = 0;
	int block_posY = 536 - 32;
	for (int i = 0; i < 2; i++) {
		ground_brick2 = BrickFactory::createBrick(3, block_posX, block_posY);
		block_arr.push_back(ground_brick2);
		block_posX += 32;
	}
	// enemy
	// enemy.LoadBitmapByString({ "resources/image/enemy/normal.bmp" }, RGB(64, 128, 128));
	// enemy.SetFrameIndexOfBitmap(0);
	// enemy.SetTopLeft(200, 120);
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
	player.ShowBitmap();
	ground_brick.ShowBitmap();
	for (auto i : block_arr) { // use 'auto' for iterate to avoid "signed/unsigned mismatch"
		i.ShowBitmap();
	}
}
