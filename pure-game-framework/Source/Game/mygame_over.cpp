#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <string>

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g): CGameState(g)
{
}

void CGameStateOver::OnMove()
{
	dead_frame++;
	if(dead_frame >= 100)
		GotoGameState(GAME_STATE_RUN);
}

void CGameStateOver::OnBeginState()
{
	game_life.LoadBitmapByString({ "resources/image/logo/game_over1.bmp" });
	game_life.SetFrameIndexOfBitmap(0);
	game_life.SetTopLeft(0, 0);
	dead_frame = 0;
	lifes--;
}

void CGameStateOver::OnInit()
{
	
}
void CGameStateOver::OnShow()
{
	game_life.ShowBitmap();
	// game over text
	CDC *pDC = CDDraw::GetBackCDC();
	CTextDraw::ChangeFontLog(pDC, 120, "Courier New", RGB(255, 255, 255), 20);
	CTextDraw::Print(pDC, 500, 400, std::to_string(lifes));
	CDDraw::ReleaseBackCDC();

}
