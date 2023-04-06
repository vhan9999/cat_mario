#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "stdio.h"
#include "mygame.h"
#include <vector>


using namespace game_framework;
class Collider : public CGameStateRun{
public:
	Collider() = default;
	~Collider() = default;
	virtual void Event() = 0;
	virtual void move() {}
	CMovingBitmap _enemy;
};
Collider* ColliderFactory(int x, int y, std::string type, std::string file, int  speed_x, int  speed_y) {
	if (type == "normalEnemy") {
		return new normalEnemy(x, y, file, speed_x, speed_y);
	}
}

class normalEnemy : public Collider {
public:
	normalEnemy(int x, int y, std::string file, int speed_x, int speed_y) {
		_enemy.LoadBitmapByString({ file }, RGB(163, 73, 164));
		_enemy.SetFrameIndexOfBitmap(0);
		_enemy.SetTopLeft(x, y);
		_speed_x = speed_x;
		_speed_y = speed_y;
	}
	void Event() {
		if (CMovingBitmap::IsOverlap(player, this->_enemy)) {
			if (player.GetTop() + player.GetHeight() >= this->_enemy.GetTop() + 8)
				//enemy dead
				delete this;
			else
				//player dead
				isDead = true;
				
		}
	}
	void move() {
		this->_enemy.SetTopLeft(this->_enemy.GetLeft() + _speed_x, this->_enemy.GetTop() + _speed_y);
	}

private:
	int _speed_x;
	int _speed_y;

};