#include "stdafx.h"
#include "colliders.h"
using namespace game_framework;


void Brick::move() {
	coll.SetTopLeft(coll.GetLeft() + speed_x, coll.GetTop() + speed_y);
	if (falling) {
		speed_y = speed_y + 1;
	}
	if (coll.GetTop() > 1500 || coll.GetTop() < -300) {
		speed_y = 0;
		speed_x = 0;
		falling = false;
	}
}