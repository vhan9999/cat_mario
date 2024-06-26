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
int CGameState::lifes = 2;

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
	event_list.clear();
	enemys_arr.clear();
	bricks_arr.clear();
	environment_arr.clear();
	MapSetting();
	if (player.isEnd == false) {
		if (player.reach_checkpoint == true) {
			for (auto &i : bricks_arr) {
				int brick_pos = i.coll.GetLeft() - player.shift_amount;
				i.coll.SetTopLeft(brick_pos, i.coll.GetTop());
			}
			for (auto &i : environment_arr) {
				int env_pos = i.coll.GetLeft() - player.shift_amount;
				i.coll.SetTopLeft(env_pos, i.coll.GetTop());
			}
			player.distance_count = player.shift_amount;
		}
		else {
			player.distance_count = 0;
			player.reach_checkpoint = false;
		}
	}

	if (player.isEnd == true) {
		lifes = 2;
	}
	player.resetValue();

	animation_flag = false;
	coin_animation_flag = false;
	pipe_animation_flag = false;
	pipe_hor_animation_flag = false;

	if (current_map == 3) {
		if (player.reach_checkpoint == false) {
			player.coll.SetTopLeft(125, 0);
			player.map_audio->Stop(0); // load map1 song
			player.dungeon_audio->Play(6, true);
		}
		else {
			player.coll.SetTopLeft(512, groundY_up-player.coll.GetHeight());
			player.map_audio->Stop(0); // load map1 song
			player.dungeon_audio->Play(6, true);
		}
	}
	else if (current_map == 4) {
		player.coll.SetTopLeft(150, groundY_up - 120 - 68);
		player.dungeon_audio->Stop(6);
		player.map_audio->Play(0, true); // load map1 song
	}
	else {
		player.coll.SetTopLeft(120, groundY_up - 68);
		player.dungeon_audio->Stop(6);
		player.map_audio->Play(0, true); // load map1 song
	}
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	player.move();
	if (current_map != 2) { 
		shiftMapImage();
	}
	
	for (auto &i : enemys_arr) {
		i.emove();
	}
	for (auto &i : bricks_arr) {
		i.move();
	}
	Touching();
	EventCtrl();
	player.check_finish();

}

// CMovingBitmap game_over;
void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	vector<string> player_image = { "resources/image/player/player_1.bmp" , "resources/image/player/player_2.bmp" ,"resources/image/player/player_1_flip.bmp" , "resources/image/player/player_2_flip.bmp", "resources/image/player/player_jump.bmp", "resources/image/player/player_jump_flip.bmp", "resources/image/player/player_blank_image.bmp","resources/image/enemy/muscle_man.bmp","resources/image/enemy/muscle_man_flip.bmp" };
	vector<string> coin_image = { "resources/animation/object/coin/coin1.bmp", "resources/animation/object/coin/coin2.bmp",
								"resources/animation/object/coin/coin3.bmp", "resources/animation/object/coin/coin4.bmp", 
								"resources/animation/object/coin/coin5.bmp", "resources/animation/object/coin/coin6.bmp", 
								"resources/animation/object/coin/coin7.bmp", "resources/animation/object/coin/coin8.bmp", };
	vector<string> interact_pipe_image = { "resources/animation/interact_pipe/animate_1.bmp","resources/animation/interact_pipe/animate_2.bmp", "resources/animation/interact_pipe/animate_3.bmp","resources/animation/interact_pipe/animate_4.bmp", 
	"resources/animation/interact_pipe/animate_5.bmp","resources/animation/interact_pipe/animate_6.bmp", "resources/animation/interact_pipe/animate_7.bmp","resources/animation/interact_pipe/animate_8.bmp", 
	"resources/animation/interact_pipe/animate_9.bmp","resources/animation/interact_pipe/animate_10.bmp", "resources/animation/interact_pipe/animate_11.bmp","resources/animation/interact_pipe/animate_12.bmp", 
	"resources/animation/interact_pipe/animate_13.bmp","resources/animation/interact_pipe/animate_14.bmp", "resources/animation/interact_pipe/animate_15.bmp","resources/animation/interact_pipe/animate_16.bmp",
	"resources/animation/interact_pipe/animate_17.bmp","resources/animation/interact_pipe/animate_18.bmp", "resources/animation/interact_pipe/animate_19.bmp","resources/animation/interact_pipe/animate_20.bmp" };
	vector<string> interact_hor_pipe_image = { "resources/animation/interact_hor_pipe/01.bmp",  "resources/animation/interact_hor_pipe/02.bmp", "resources/animation/interact_hor_pipe/03.bmp",
											"resources/animation/interact_hor_pipe/04.bmp", "resources/animation/interact_hor_pipe/05.bmp", "resources/animation/interact_hor_pipe/06.bmp" };
	vector<string> game_over_image = { "resources/image/logo/game_over1.bmp", "resources/image/logo/game_over2.bmp" };
	
	player = Player(100, groundY_up - 68, player_image); // player initial posiiton
	player.load_voice();
	// set curent checkpoint
	player.current_checkpoint_x = 120;
	player.current_checkpoint_y = groundY_up - 68;

	// animation
	coin_animation.LoadBitmapByString(coin_image, RGB(163, 73, 164));
	pipe_animation.LoadBitmapByString(interact_pipe_image, RGB(255, 242, 0));
	pipe_hor_animation.LoadBitmapByString(interact_hor_pipe_image, RGB(255, 242, 0));
	
	

	MapSetting();// set map
	
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (player.isFinish == false) {
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
		if (nChar == 0X43) {
			if (cheat_mode == true) {
				cheat_mode = false;
			}
			else {
				cheat_mode = true;
			}
		}
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
	for (auto i : environment_arr) {
		i.coll.ShowBitmap();
	}
	for (auto i : enemys_arr) {
		if (!i.is_dead)
			i.coll.ShowBitmap();
	}
	for (auto i : bricks_arr) {
		i.coll.ShowBitmap();
	}
	
	if (animation_flag == true) {
		if (coin_animation_flag == true) { coin_animation.ShowBitmap(); }
		if (pipe_animation_flag == true) { pipe_animation.ShowBitmap(); }
	}


	if (cheat_mode == true) {
		CDC *pDC2 = CDDraw::GetBackCDC();
		CTextDraw::ChangeFontLog(pDC2, 30, "Courier New", RGB(255, 0, 0), 20);
		CTextDraw::Print(pDC2, 0, 60, "cheat mode on");
		CDDraw::ReleaseBackCDC();
	}

	/*
	CDC *pDC3 = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC3, 30, "Courier New", RGB(255, 0, 0), 20);
	CTextDraw::Print(pDC3, 0, 230, "distance : " + std::to_string(player.distance_count));
	CDDraw::ReleaseBackCDC();

	CDC *pDC0 = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC0, 30, "Courier New", RGB(255, 0, 0), 20);
	CTextDraw::Print(pDC0, 0, 190, "game end : " + std::to_string(gameEnd));
	CDDraw::ReleaseBackCDC();

	CDC *pDC1 = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC1, 30, "Courier New", RGB(255, 0, 0), 20);
	CTextDraw::Print(pDC1, 0, 140, "current map : " + std::to_string(current_map));
	CDDraw::ReleaseBackCDC();

	CDC *pDC2 = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC2, 30, "Courier New", RGB(255, 0, 0), 20);
	CTextDraw::Print(pDC2, 0, 60, "map3_frame: " + std::to_string(player.map3frame));
	CDDraw::ReleaseBackCDC();
	
	CDC *pDC3 = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC3, 30, "Courier New", RGB(255, 0, 0), 20);
	CTextDraw::Print(pDC3, 0, 230, "distance : " + std::to_string(player.distance_count));
	CDDraw::ReleaseBackCDC();
	*/
	
	if(!player.isDead)
		player.coll.ShowBitmap();

}
