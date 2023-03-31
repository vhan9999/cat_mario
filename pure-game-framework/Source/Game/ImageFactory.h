#pragma once
#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "stdio.h"
#include <vector>
using namespace game_framework;


class ImageFactory {
public:
	static CMovingBitmap createBrick(int type, int x, int y) {
		CMovingBitmap new_brick;
		switch (type) {
		case 1: // brick1
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick.bmp" }, RGB(163, 73, 164));
		case 2: // brick2
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick2.bmp" }, RGB(163, 73, 164));
		case 3: // brick3
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick3.bmp" }, RGB(163, 73, 164));
		case 4: //brick4
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick4.bmp" }, RGB(163, 73, 164));
		case 5: // brick5
			new_brick.LoadBitmapByString({ "resources/image/object/block1/brown_brick5.bmp" }, RGB(163, 73, 164));
		default:
			break;
		}
		new_brick.SetFrameIndexOfBitmap(0);
		new_brick.SetTopLeft(x, y);
		return new_brick;
	}

	static CMovingBitmap createEnemy(std::string name, int x, int y) {
		CMovingBitmap enemy;
		if (name == "normal") {
			enemy.LoadBitmapByString({ "resources/image/enemy/normal.bmp" }, RGB(163, 73, 164));
		}
		else if (name == "star_smile") {
			enemy.LoadBitmapByString({ "resources/image/enemy/star_smile.bmp" }, RGB(163, 73, 164));
		}
		enemy.SetFrameIndexOfBitmap(0);
		enemy.SetTopLeft(x, y);
		return enemy;
	}
};