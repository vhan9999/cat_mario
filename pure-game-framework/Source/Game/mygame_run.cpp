#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

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
	if (isMoveLeft) {
		player.SetTopLeft(player.GetLeft() - 3, player.GetTop());
	}
	if (isMoveRight) {
		player.SetTopLeft(player.GetLeft() + 3, player.GetTop());
	}

}

void CGameStateRun::OnInit() // 遊戲的初值及圖形設定 set initial value and image
{	
 	player.LoadBitmapByString({ "resources/image/player/player_1.bmp", "resources/image/player/player_2.bmp" });
	player.SetFrameIndexOfBitmap(0);
 	player.SetTopLeft(120, 300);
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
}
