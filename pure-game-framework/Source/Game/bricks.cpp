#include "stdafx.h"
#include "colliders.h"
using namespace game_framework;


void Brick::move() {
	coll.SetTopLeft(coll.GetLeft() + speed_x, coll.GetTop() + speed_y);
	if (falling) {
		speed_y = speed_y + 1;
	}
	
}