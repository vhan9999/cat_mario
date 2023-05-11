#include "stdafx.h"
#include "colliders.h"

using namespace game_framework;
void Enemy::emove() {
	coll.SetTopLeft(coll.GetLeft() + speed_x, coll.GetTop() + speed_y);
	if (steel) {
		speed_y = speed_y + 1;
	}
	if (coll.GetTop() > 1500 || coll.GetTop() < -300) {
		speed_y = 0;
		speed_x = 0;
		steel = false;
	}
}