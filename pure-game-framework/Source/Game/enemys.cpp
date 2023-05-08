#include "stdafx.h"
#include "colliders.h"

using namespace game_framework;
void Enemy::emove() {
	coll.SetTopLeft(coll.GetLeft() + speed_x, coll.GetTop() + speed_y);
	if (steel) {
		speed_y = speed_y + 1;
	}
}
