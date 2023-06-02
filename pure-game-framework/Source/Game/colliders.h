#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include <string>
#ifndef COLLIDERS_H
#define COLLIDERS_H


namespace game_framework {
	class Collider{
	public:
		Collider() = default;
		Collider(int posx, int posy, std::vector<std::string> image);
		virtual ~Collider();
		virtual void move();
		virtual void event();
		CMovingBitmap coll;
		int speed_x = 0;
		int speed_y = 0;
		std::string name = "";
	};

	class Player : public Collider {
	public:
		Player() = default;
		Player(int posx, int posy, std::vector<std::string> image) : Collider(posx, posy, image) {}
		~Player() = default;
		void move();
		void moveHor();
		void moveVer();
		void ableToJump(double &ground);
		void ani();
		void load_voice();
		void voice();
		void resetValue();
		void check_finish();

		int frame = 0;
		double animate_frame = 0;
		int jumpBonusFrame = 0;
		int dead_frame = 0;
		int jumpSpeed = 0;
		int moveSpeed = 0;
		int dead_count = 2;
		int finish_audio_flag = 0;
		int dead_audio_flag = 0;
		int shift_amount = 0; // track player distance far from checkpoint point
		int distance_count = 0; // track player distance far from initial point
		int finish_point;

		bool keyUp = false;
		bool keyDown = false;
		bool keyLeft = false;
		bool keyRight = false;
		bool player_fall = false;
		bool player_fly = false;

		bool player_on_air = false;
		bool isMove = false;
		bool isDead = false;
		bool isFinish = false;
		bool isEnd = false;
		bool reach_checkpoint = false;

		int current_checkpoint_x;
		int current_checkpoint_y;


		CAudio *map_audio = CAudio::Instance();
		CAudio *player_jump_audio = CAudio::Instance();
		CAudio *game_over_audio = CAudio::Instance();
		CAudio *coin_item_brick_audio = CAudio::Instance();
		CAudio *pipe_interact_audio = CAudio::Instance();
		CAudio *player_finish_audio = CAudio::Instance();
		CAudio *dungeon_audio = CAudio::Instance();
		// CAudio *player_break_brick = CAudio::Instance();
	};

	class Brick : public Collider {
	public:
		Brick(){}
		Brick(int posx, int posy, std::vector<std::string> image) : Collider(posx, posy, image) {
		}
		~Brick() = default;
		void move();
		bool head_touch_break = false; // break and animation
		bool foot_touch_break = false;
		bool invisible = false;
		bool foot_touch_fall = false;
		bool head_touch_change = false;//change picture 
		bool have_coin = false;
		bool is_danger = false;
		int is_fall = 0;
		bool falling = false;
		std::string item = "";

	};

	class Enemy : public Collider {
	public:
		Enemy() {}
		Enemy(int posx, int posy , std::vector<std::string> image) : Collider(posx, posy, image) {}
		~Enemy() = default;
		void emove();

		bool step_enemy_enemy_dead = false;//true -> enemy dead
		bool step_enemy_player_dead = false;
		bool step_enemy_jump = false;
		bool steel = true;
		bool is_dead = false;
		bool able_touch = true;
		bool turtle = false;
		bool cloud = false;
		bool big_mushroom = false;
	};

	class Environment : public Collider {
	public:
		Environment() = default;
		Environment(int posx, int posy, std::vector<std::string> image) : Collider(posx, posy, image) {}
		~Environment() = default;
	};
}

#endif