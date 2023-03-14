#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "stdio.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// This class is the game execution object of the game, and the main game programs are here
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnMove()							// 移動遊戲元素 move
{
	frame++;
	moveHor();
	moveVer();
	if (player.GetTop() + jumpSpeed > 800) {
		player.SetTopLeft(player.GetLeft() + moveSpeed, 800);
	}
	else if (moveSpeed != 0 || jumpSpeed != 0) {
		player.SetTopLeft(player.GetLeft() + moveSpeed, player.GetTop() + jumpSpeed);
	}
	if (frame + 1 < 0) {
		frame = 0;
	}
}

void CGameStateRun::moveHor() {
	
	if (keyRight == true) {
		if (frame %10 == 0) {
			moveSpeed += 3;
			if (moveSpeed < 0)
				moveSpeed++;
		}
		if (moveSpeed >= 6)
			moveSpeed = 6;
	}
	if (keyLeft == true) {

		if (frame % 10 == 0) {
			moveSpeed -= 3;
			if (moveSpeed > 0)
				moveSpeed--;
		}
		if (moveSpeed <= -6)
			moveSpeed = -6;
	}
	if ((!keyRight && !keyLeft && moveSpeed != 0)) {
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

void CGameStateRun::moveVer() {
	jumpBonusFrame++;
	if (player.GetTop() < 800) {
		jumpSpeed += 1;
	}
	else if (keyUp && player.GetTop() == 800) {
		jumpBonusFrame = 0;
		jumpSpeed = -24;
	}
	else if (player.GetTop() > 800) {
		jumpSpeed = 0;
		player.SetTopLeft(player.GetLeft(), 800);
	}
	if (jumpBonusFrame == 4 && keyUp) {
		isBigJump = true;
		jumpSpeed -= 10;
	}
	/*else if (isBigJump && jumpBonusFrame <30) 
		player.SetTopLeft(player.GetLeft(), player.GetTop() + jumpSpeed / 3);
	else if (isBigJump && jumpBonusFrame == 30) {
		player.SetTopLeft(player.GetLeft(), player.GetTop() + jumpSpeed / 3);
		isBigJump = false;
	}*/
	
}

void CGameStateRun::OnInit() // 遊戲的初值及圖形設定 set initial value and image
{	
	player.LoadBitmapByString({ "resources/image/player/player_1.bmp" });
	player.SetTopLeft(50, 800);
	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
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
	player.ShowBitmap();
}
