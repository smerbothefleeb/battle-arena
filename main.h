#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <random>
#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <Windows.h>
using namespace std;


/* Global Variables */
namespace GLOBAL {
	const int WORLD_WIDTH = 1080;
	const double GRAVITY = 9.80665;
	const int WORLD_HEIGHT = 720;
	const double FPS = 60.0;
	const double SPF = 1.0 / FPS;
	const int attack_delay = 10;
	const int projectile_timer = 20;
}

inline double elapsed_time;
inline int frames = 0;

/* Allegro Variables */
inline ALLEGRO_DISPLAY* display;
inline ALLEGRO_BITMAP* team_1_knight_idle_states;
inline ALLEGRO_BITMAP* team_1_knight_movement_states;
inline ALLEGRO_BITMAP* team_1_knight_attack_states;
inline ALLEGRO_BITMAP* background;
inline ALLEGRO_BITMAP* shadow;
inline ALLEGRO_FONT* font;

inline ALLEGRO_BITMAP* team_2_knight_idle_states;
inline ALLEGRO_BITMAP* team_2_knight_movement_states;
inline ALLEGRO_BITMAP* team_2_knight_attack_states;
inline ALLEGRO_BITMAP* explosion_sheet;
inline ALLEGRO_BITMAP* explosion[9];

inline ALLEGRO_COLOR mask_colour;
inline ALLEGRO_COLOR white;
inline ALLEGRO_TIMER* timer;
inline ALLEGRO_EVENT_QUEUE* queue;


/* Other Variables */
inline bool running = true;
inline int array_add = 0;
inline float spawn_delay = 3;
inline string red_counter;
inline string blue_counter;

inline struct Actors {
	struct Troop {
		bool stunned = false;
		int stunned_timer = 10;
		int stunned_frames = 0;
		float vx, vy;
		struct Animation;
		ALLEGRO_BITMAP* knight_moving[3];
		ALLEGRO_BITMAP* knight_attack[3];
		ALLEGRO_BITMAP* knight_idle[3];
		float move_speed = 1;
		int team;
		int spawn_timer = 0;
		int health = 200;
		bool alive = true;
		unsigned int num_of_troops = 100;
		bool combat = false;
		bool moving = true;
		bool draw_shadow = true;
		ALLEGRO_BITMAP* sprite = NULL;
		float x = 0, y = 0;
		int width = 0, height = 0;
		float range = 30;
		int damage = 5;
		int attack_timer = 0;
		int knockback_factor = 5;
		int knockback_timer = 0;
		int knockback_delay = 3;
		int target;
		float target_radians;
		float target_dz;
		float target_dy;
		float target_dx;
		void create_units(int X);
		void animate();
		bool follow_target();
		void attack_target();
		void border_collision();
		void shunt(float shunt_angle, int knockback);

		struct Animation {
			int FRAME_NUM = 0;
			int max_frames = 3;
			double elapsed_time = 0;
			int nframes = 3;
			double duration = 0.3;
		}animation;

		vector<float> enemy_dx = { NULL };
		vector<float> enemy_dy = { NULL };
		vector<float> enemy_radians = { NULL };
		vector<float> enemy_dz = { NULL };
	}team2_troop, team1_troop;
}actors;

inline struct Ranged_Troop {
	int x = 10, y = 360;
	ALLEGRO_BITMAP* bitmap;
	int fire_timer = 300;
	int fire_frames = 0;

	struct Projectile {
		bool land = false;
		bool can_explode = false;
		float x, y, vx = 0, vy = 0, v;
		bool fired = false;
		float dz, dx, dy;
		int flight_time = 260;
		float fire_angle;
		float speed;
		int colour_r = 255, colour_g = 0, colour_b = 0;

		struct Target {
			int target;
			float x, y, dz, explode_radius = 10;
		}tgt;
	}proj;
}rng;

inline vector<Ranged_Troop::Projectile> projectile;
inline vector<Actors::Troop> team_1_vector;
inline vector<Actors::Troop> team_2_vector;

inline struct Blood {
	ALLEGRO_BITMAP* sprite;
	int x = 0, y = 0, max = 50;
	int width = 60, height = 60;

	void draw();
}blood;
inline vector<Blood> blood_splatters;