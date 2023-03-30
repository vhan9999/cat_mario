/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// Constants
	/////////////////////////////////////////////////////////////////////////////

	enum AUDIO_ID {				// 定義各種音效的編號
		AUDIO_DING,				// 0
		AUDIO_LAKE,				// 1
		AUDIO_NTUT				// 2
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
	protected:
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap logo;								// csie的logo
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作 
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作

		void ableToJump(int &jumpSpd, int &jumpBns, double &ground);
		bool inRange(double num, double min, double max);
		int high_from_ground(int blockCount);
		int far_from_start(int blockCount);

		// collision
		void check_collision_ver(std::vector<CMovingBitmap> &arr, CMovingBitmap &player);
		void check_collision_hor(std::vector<CMovingBitmap> &arr, CMovingBitmap &player);
		void check_ground_collision(std::vector<CMovingBitmap> &arr, CMovingBitmap &player);
		void check_enemy_collision(CMovingBitmap &enemy, CMovingBitmap &player);

		// Load and show image 
		void build_block_ground(int type, int amt, int x, int y);
		void loadImage_ground(int amount, int x_up, int y_up, int x_mid, int y_mid, int x_down, int y_down);
		void loadImage_multiple_ver(int type, int amount, int x, int y);
		void loadImage_multiple_hor(int type, int amount, int x, int y);
		void loadImage_enemy(std::string name, int x, int y);
		void show_ground();
		void show_hor();
		void show_ver();
		void show_enemy();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();	// 顯示這個狀態的遊戲畫面
	private:
		CMovingBitmap background;
		CMovingBitmap player;
		int moveSpeed = 0;
		int jumpSpeed = 0;
		bool keyUp = false;
		bool isBigJump = false;
		bool keyRight = false;
		bool keyLeft = false;
		int frame = 0;
		int jumpBonusFrame;
		void moveHor();
		void moveVer();
		CMovingBitmap ground_brick;
		CMovingBitmap brick;
		CMovingBitmap brick2;
		CMovingBitmap sky_brick;
		int groundX_up = 0;
		int groundY_up = 716;
		int groundX_mid = 0;
		int groundY_mid = 776;
		int groundX_down = 0;
		int groundY_down = 836;
		int current_ground_arr_flag = -1; // to track number of element ground block were built

		std::vector<std::vector<CMovingBitmap>> upper_ground_brick_arr; // ground block arr
		std::vector<std::vector<CMovingBitmap>> rem_ground_brick_arr; // ground block arr
		std::vector<std::vector<CMovingBitmap>> ver_block_arr; // vertical block arr
		std::vector<std::vector<CMovingBitmap>> hor_block_arr; // horizontal block arr
		std::vector<CMovingBitmap> enemy_arr; // enemy array
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int counter;	// 倒數之計數器
	};

}
