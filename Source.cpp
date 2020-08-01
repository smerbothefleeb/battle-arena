#include "main.h"
using namespace GLOBAL;

void Actors::Troop::animate() {
	if (this->health > 0) {
		if (this->moving == true && this->combat == false) {
			this->sprite = this->knight_moving[this->animation.FRAME_NUM = (int)this->animation.nframes * fmod(this->animation.elapsed_time, this->animation.duration) / this->animation.duration];
		}
		if (this->moving == false && this->combat == false) {
			this->sprite = this->knight_idle[this->animation.FRAME_NUM = (int)this->animation.nframes * fmod(this->animation.elapsed_time, (this->animation.duration * 5)) / (this->animation.duration * 5) - 1];
		}
		if (this->moving == false && this->combat == true) {
			this->sprite = this->knight_attack[this->animation.FRAME_NUM = (int)this->animation.nframes * fmod(this->animation.elapsed_time, this->animation.duration) / this->animation.duration];
		}
	}
}

void Actors::Troop::create_units(int X) {
	this->x = X;
	this->y = rand() % WORLD_HEIGHT;
	this->sprite = NULL;
}

bool Actors::Troop::follow_target() {
	if (this->health > 0) {
		this->vx = rand() % 1 + this->move_speed * cos(this->target_radians);
		this->vy = rand() % 1 + this->move_speed * sin(this->target_radians);
		this->x += this->vx;
		this->y += this->vy;

		if (this->vx == 0 && this->vy == 0) {
			this->moving = false;
			return this->moving;
		}
		else {
			this->moving = true;
			return this->moving;
		}
	}
}

void Actors::Troop::border_collision() {
	if (this->health > 0) {
		if (this->x + this->width > WORLD_WIDTH) {
			this->x -= this->x + this->width - WORLD_WIDTH;
		}
		if (this->x < 0) {
			this->x = 0;
		}

		if (this->y < 0) {
			this->y = 0;
		}
		if (this->y + this->height > WORLD_HEIGHT) {
			this->y -= this->y + this->height - WORLD_HEIGHT;
		}
	}
}

void Actors::Troop::attack_target() {
	if (this->health > 0) {

		if (this->target_dz <= this->range) {
			this->moving = false;
			this->combat = true;
		}
		else {
			this->combat = false;
		}
	}
}

void Actors::Troop::shunt(float shunt_angle, int knockback) {
	if (this->health > 0) {
		this->knockback_timer++;
		for (int i = 0; i < knockback; i++) {
			if (this->knockback_timer == this->knockback_delay) {
				this->x += 20 * cos(shunt_angle);
				this->y += 20 * sin(shunt_angle);
				this->knockback_timer = 0;
			}
		}
	}
}

void Blood::draw() {
	al_draw_bitmap(this->sprite, this->x, this->y, 0);
}
