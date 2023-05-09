#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include "colliders.h"
#include <string>
#include <typeinfo>

using namespace game_framework;
/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
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

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	player.move();
	// player.dead_check(); check player is dead or not

	for (auto &i : enemys_arr) {
		i.emove();
	}
	Touching();

	shiftMapImage();
}

// CMovingBitmap game_over;
void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	vector<string> player_image = { "resources/image/player/player_1.bmp" , "resources/image/player/player_2.bmp" ,"resources/image/player/player_1_flip.bmp" , "resources/image/player/player_2_flip.bmp", "resources/image/player/player_jump.bmp", "resources/image/player/player_jump_flip.bmp", "resources/image/player/player_blank_image.bmp" };
	vector<string> coin_image = { "resources/animation/object/coin/coin1.bmp", "resources/animation/object/coin/coin2.bmp",
								"resources/animation/object/coin/coin3.bmp", "resources/animation/object/coin/coin4.bmp", 
								"resources/animation/object/coin/coin5.bmp", "resources/animation/object/coin/coin6.bmp", 
								"resources/animation/object/coin/coin7.bmp", "resources/animation/object/coin/coin8.bmp", };
	vector<string> interact_pipe_image = { "resources/animation/interact_pipe/animate_1.bmp","resources/animation/interact_pipe/animate_2.bmp", "resources/animation/interact_pipe/animate_3.bmp","resources/animation/interact_pipe/animate_4.bmp", 
	"resources/animation/interact_pipe/animate_5.bmp","resources/animation/interact_pipe/animate_6.bmp", "resources/animation/interact_pipe/animate_7.bmp","resources/animation/interact_pipe/animate_8.bmp", 
	"resources/animation/interact_pipe/animate_9.bmp","resources/animation/interact_pipe/animate_10.bmp", "resources/animation/interact_pipe/animate_11.bmp","resources/animation/interact_pipe/animate_12.bmp", 
	"resources/animation/interact_pipe/animate_13.bmp","resources/animation/interact_pipe/animate_14.bmp", "resources/animation/interact_pipe/animate_15.bmp","resources/animation/interact_pipe/animate_16.bmp",
	"resources/animation/interact_pipe/animate_17.bmp","resources/animation/interact_pipe/animate_18.bmp", "resources/animation/interact_pipe/animate_19.bmp","resources/animation/interact_pipe/animate_20.bmp" };
	vector<string> game_over_image = { "resources/image/logo/game_over1.bmp", "resources/image/logo/game_over2.bmp" };
	
	player = Player(200, groundY_up-68, player_image); // player initial posiiton
	player.load_voice();
	player.map_audio->Play(0, true); // load map1 song
	
	coin_animation.LoadBitmapByString(coin_image, RGB(163, 73, 164));
	pipe_animation.LoadBitmapByString(interact_pipe_image, RGB(255, 242, 0));

	/*
	game_over.LoadBitmapByString(game_over_image, RGB(163, 73, 164));
	game_over.SetFrameIndexOfBitmap(1);
	*/
	MapSetting();// set map
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == VK_LEFT) {
		player.keyLeft = true;
		player.keyRight = false;
	}
	if (nChar == VK_RIGHT) {
		player.keyRight = true;
		player.keyLeft = false;
	}
	if (nChar == VK_UP) {
		player.keyUp = true;
	}
	if (nChar == VK_DOWN) {
		player.keyDown = true;
	}
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT) {
		player.keyRight = false;
	}
	if (nChar == VK_LEFT) {
		player.keyLeft = false;
	}
	if (nChar == VK_UP) {
		player.keyUp = false;
	}
	if (nChar == VK_DOWN) {
		player.keyDown = false;
	}
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
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
	for (auto i : environment_arr) { i.coll.ShowBitmap(); }
	for (auto i : bricks_arr) { i.coll.ShowBitmap();}

	for (auto i : enemys_arr) {
		if(!i.is_dead)
			i.coll.ShowBitmap();
	}
	if (animation_flag == true) {
		if (coin_animation_flag == true) { coin_animation.ShowBitmap(); }
		if (pipe_animation_flag == true) { pipe_animation.ShowBitmap(); }
	}
	if (!player.isDead) {
		player.coll.ShowBitmap();
	}
	/* display game over screen
	if (player.isDead == true) {
		Sleep(1500);
		game_over.SetFrameIndexOfBitmap(0);
		game_over.ShowBitmap();

		CDC *pDC = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC, 120, "Courier New", RGB(255, 255, 255), 20);
		CTextDraw::Print(pDC, 500, 400, std::to_string(player.dead_count));
		CDDraw::ReleaseBackCDC();
	}
	*/
}
