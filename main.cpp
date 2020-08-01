#include "main.h"
using namespace GLOBAL;

int main() {
	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	srand(time(NULL));

	display = al_create_display(WORLD_WIDTH, WORLD_HEIGHT);
	al_set_window_title(display, "Battle Arena");
	timer = al_create_timer(SPF);
	queue = al_create_event_queue();
	mask_colour = al_map_rgb(153, 217, 234);
	white = al_map_rgb(255, 255, 255);
	shadow = al_load_bitmap("shadow.png");
	al_convert_mask_to_alpha(shadow, mask_colour);
	background = al_load_bitmap("background.png");
	font = al_load_font("fixedsys.ttf", 20, 0);
	blood.sprite = al_load_bitmap("blood-splatter.png");
	//al_convert_mask_to_alpha(blood.sprite, white);
	al_convert_mask_to_alpha(blood.sprite, mask_colour);

	/* Bitmap loading & masking */
	team_1_knight_attack_states = al_load_bitmap("team-1-knight-attack-states.png");
	team_1_knight_idle_states = al_load_bitmap("team-1-knight-idle-states.png");
	team_1_knight_movement_states = al_load_bitmap("team-1-knight-movement-states.png");
	al_convert_mask_to_alpha(team_1_knight_attack_states, mask_colour);
	al_convert_mask_to_alpha(team_1_knight_idle_states, mask_colour);
	al_convert_mask_to_alpha(team_1_knight_movement_states, mask_colour);
	explosion_sheet = al_load_bitmap("explosion.png");
	al_convert_mask_to_alpha(explosion_sheet, mask_colour);

	team_2_knight_attack_states = al_load_bitmap("team-2-knight-attack-states.png");
	team_2_knight_idle_states = al_load_bitmap("team-2-knight-idle-states.png");
	team_2_knight_movement_states = al_load_bitmap("team-2-knight-movement-states.png");
	al_convert_mask_to_alpha(team_2_knight_attack_states, mask_colour);
	al_convert_mask_to_alpha(team_2_knight_idle_states, mask_colour);
	al_convert_mask_to_alpha(team_2_knight_movement_states, mask_colour);


	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	actors.team1_troop.team = 1;
	actors.team2_troop.team = 2;

	rng.proj.x = 0;
	rng.proj.y = WORLD_HEIGHT / 2;

	/* Bitmap Arrays */
	array_add = 0;
	for (int i = 0; i < 2; i++) {
		actors.team1_troop.knight_idle[i] = al_create_sub_bitmap(team_1_knight_idle_states, array_add, 0, 21, 26);
		array_add += 22;
	}
	array_add = 0;
	for (int i = 0; i < 3; i++) {
		actors.team1_troop.knight_moving[i] = al_create_sub_bitmap(team_1_knight_movement_states, array_add, 0, 25, 26);
		array_add += 26;
	}
	actors.team1_troop.knight_attack[0] = al_create_sub_bitmap(team_1_knight_attack_states, 0, 0, 19, 26);
	actors.team1_troop.knight_attack[1] = al_create_sub_bitmap(team_1_knight_attack_states, 20, 0, 24, 26);
	actors.team1_troop.knight_attack[2] = al_create_sub_bitmap(team_1_knight_attack_states, 45, 0, 19, 26);

	array_add = 0;
	for (int i = 0; i < 2; i++) {
		actors.team2_troop.knight_idle[i] = al_create_sub_bitmap(team_2_knight_idle_states, array_add, 0, 21, 26);
		array_add += 22;
	}
	array_add = 0;
	for (int i = 0; i < 3; i++) {
		actors.team2_troop.knight_moving[i] = al_create_sub_bitmap(team_2_knight_movement_states, array_add, 0, 25, 26);
		array_add += 26;
	}
	actors.team2_troop.knight_attack[0] = al_create_sub_bitmap(team_2_knight_attack_states, 0, 0, 19, 26);
	actors.team2_troop.knight_attack[1] = al_create_sub_bitmap(team_2_knight_attack_states, 20, 0, 24, 26);
	actors.team2_troop.knight_attack[2] = al_create_sub_bitmap(team_2_knight_attack_states, 45, 0, 19, 26);

	array_add = 0;
	for (int i = 0; i < 9; i++) {
		explosion[i] = al_create_sub_bitmap(explosion_sheet, array_add, 0, 75, 75);
		array_add += 78;
	}

	int x = 0;
	int y = 0;
	al_start_timer(timer);
	while (running == true) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			running = false;
		}

		
		if (event.type == ALLEGRO_EVENT_TIMER) {
			rng.fire_frames++;
			elapsed_time += SPF;
			actors.team1_troop.spawn_timer++;
			actors.team2_troop.spawn_timer++;
			
			for (unsigned int i = 0; i < actors.team1_troop.num_of_troops; i++) {
				if (team_1_vector.size() < actors.team1_troop.num_of_troops && actors.team1_troop.spawn_timer == spawn_delay) {
					actors.team1_troop.spawn_timer = 0;
					actors.team1_troop.create_units(0);
					team_1_vector.push_back(actors.team1_troop);
				}
			}
			for (unsigned int i = 0; i < actors.team2_troop.num_of_troops; i++) {
				if (team_2_vector.size() < actors.team2_troop.num_of_troops && actors.team2_troop.spawn_timer == spawn_delay) {
					actors.team2_troop.spawn_timer = 0;
					actors.team2_troop.create_units(WORLD_WIDTH);
					team_2_vector.push_back(actors.team2_troop);
				}
			}

			for (int i = 0; i < team_1_vector.size(); i++) {
				for (int x = 0; x < blood_splatters.size(); x++) {
					if (team_1_vector[i].x + team_1_vector[i].width / 2 > blood_splatters[x].x &&
						team_1_vector[i].x + team_1_vector[i].width / 2 < blood_splatters[x].x + blood_splatters[x].width &&
						team_1_vector[i].y + team_1_vector[i].height > blood_splatters[x].y &&
						team_1_vector[i].y + team_1_vector[i].height < blood_splatters[x].y + blood_splatters[x].height) {
						team_1_vector[i].draw_shadow = false;
					}
				}

				if (team_1_vector[i].health < 0 ) {
					team_1_vector[i].alive = false;
				}
				else {
					team_1_vector[i].alive = true;
				}
				team_1_vector[i].target = rand() % team_2_vector.size();

				team_1_vector[i].target_dx = team_2_vector[team_1_vector[i].target].x - team_1_vector[i].x;
				team_1_vector[i].target_dy = team_2_vector[team_1_vector[i].target].y - team_1_vector[i].y;
				team_1_vector[i].target_dz = sqrt(pow(team_1_vector[i].target_dx, 2) + pow(team_1_vector[i].target_dy, 2));
				team_1_vector[i].target_radians = atan2(team_1_vector[i].target_dy, team_1_vector[i].target_dx);
			}
			for (int i = 0; i < team_2_vector.size(); i++) {
				for (int x = 0; x < blood_splatters.size(); x++) {
					if (team_2_vector[i].x + team_2_vector[i].width / 2 > blood_splatters[x].x &&
						team_2_vector[i].x + team_2_vector[i].width / 2 < blood_splatters[x].x + blood_splatters[x].width &&
						team_2_vector[i].y + team_2_vector[i].height > blood_splatters[x].y &&
						team_2_vector[i].y + team_2_vector[i].height < blood_splatters[x].y + blood_splatters[x].height) {
						team_2_vector[i].draw_shadow = false;
					}
				}
				if (team_2_vector[i].health < 0) {
					team_2_vector[i].alive = false;
				}
				else {
					team_2_vector[i].alive = true;
				}
				team_2_vector[i].target = rand() % team_1_vector.size();

				team_2_vector[i].target_dx = team_1_vector[team_2_vector[i].target].x - team_2_vector[i].x ;
				team_2_vector[i].target_dy = team_1_vector[team_2_vector[i].target].y - team_2_vector[i].y;
				team_2_vector[i].target_dz = sqrt(pow(team_2_vector[i].target_dx, 2) + pow(team_2_vector[i].target_dy, 2));
				team_2_vector[i].target_radians = atan2(team_2_vector[i].target_dy, team_2_vector[i].target_dx);
			}

			
			al_clear_to_color(white);
			al_draw_bitmap(background, 0, 0, 0);



			for (unsigned int x = 0; x < team_1_vector.size(); x++) {
				if (team_1_vector[x].health > 0 && team_1_vector[x].draw_shadow == true) {
					al_draw_bitmap(shadow, team_1_vector[x].x + team_1_vector[x].width / 3.5, team_1_vector[x].y + team_1_vector[x].height / 2 + team_1_vector[x].height / 4, 0);
				}
				else {
					if (rand() % 2 == 1 && blood_splatters.size() < blood.max) {
						blood.x = team_1_vector[x].x;
						blood.y = team_1_vector[x].y;
						blood_splatters.push_back(blood);
					}
				}
				for (unsigned int y = 0; y < team_2_vector.size(); y++) {
					if (sqrt(pow(team_1_vector[x].x - team_2_vector[y].x, 2) + pow(team_1_vector[x].y - team_2_vector[y].y, 2)) < 20) {
						team_1_vector[x].attack_timer++;
						if (team_1_vector[x].attack_timer == attack_delay) {
							team_1_vector[x].attack_timer = 0;
							team_2_vector[y].health -= team_1_vector[x].damage;
							team_2_vector[y].shunt(atan2(team_2_vector[y].y - team_1_vector[x].y, team_2_vector[y].x - team_1_vector[x].x), team_1_vector[x].knockback_factor);
							team_2_vector[y].stunned = true;
						}
					}
					else {
						team_2_vector[y].stunned = false;
					}
				}
			}
			for (int i = 0; i < blood_splatters.size(); i++) {
				blood_splatters[i].draw();
			}
			for (unsigned int x = 0; x < team_2_vector.size(); x++) {
				if (team_2_vector[x].health > 0 && team_2_vector[x].draw_shadow == true) {
					al_draw_bitmap(shadow, team_2_vector[x].x, team_2_vector[x].y + team_2_vector[x].height / 2 + team_2_vector[x].height / 4, 0);
				}
				else {
					if (rand() % 2 == 1 && blood_splatters.size() < blood.max) {
						blood.x = team_2_vector[x].x;
						blood.y = team_2_vector[x].y;
						blood_splatters.push_back(blood);
					}
				}
				for (unsigned int y = 0; y < team_1_vector.size(); y++) {
					if (sqrt(pow(team_2_vector[x].x - team_1_vector[y].x, 2) + pow(team_2_vector[x].y - team_1_vector[y].y, 2)) < 20) {
						team_2_vector[x].attack_timer++;
						if (team_2_vector[x].attack_timer == attack_delay) {
							team_2_vector[x].attack_timer = 0;
							team_1_vector[y].health -= team_2_vector[x].damage;
							team_1_vector[y].shunt(atan2(team_1_vector[y].y - team_2_vector[x].y, team_1_vector[y].x - team_2_vector[x].x), team_2_vector[x].knockback_factor);
							team_1_vector[y].stunned = true;
						}
						else {
							team_1_vector[y].stunned = false;
						}
					}
				}
			}


			for (unsigned int i = 0; i < team_1_vector.size(); i++) {
				if (team_1_vector[i].stunned == false) {
					team_1_vector[i].follow_target();
				}
				team_1_vector[i].border_collision();
				team_1_vector[i].attack_target();
				team_1_vector[i].animation.elapsed_time += SPF;
				team_1_vector[i].animate();
				team_1_vector[i].width = al_get_bitmap_width(team_1_vector[i].sprite);
				team_1_vector[i].height = al_get_bitmap_height(team_1_vector[i].sprite);
				if (team_1_vector[i].alive == true) {
					al_draw_bitmap(team_1_vector[i].sprite, team_1_vector[i].x, team_1_vector[i].y, 0);
					//al_draw_line(team_1_vector[i].x, team_1_vector[i].y, team_1_vector[i].x + WORLD_WIDTH * cos(team_1_vector[i].target_radians), team_1_vector[i].y + WORLD_WIDTH * sin(team_1_vector[i].target_radians), al_map_rgb(255, 0, 0), 1);
				}
				else {
					if (team_1_vector.size() > 1) {
						team_1_vector.erase(team_1_vector.begin() + i);
					}
				}
			}

			for (unsigned int i = 0; i < team_2_vector.size(); i++) {
				if (team_2_vector[i].stunned == false) {
					team_2_vector[i].follow_target();
				}
				team_2_vector[i].border_collision();
				team_2_vector[i].attack_target();
				team_2_vector[i].animation.elapsed_time += SPF;
				team_2_vector[i].animate();
				team_2_vector[i].width = al_get_bitmap_width(team_2_vector[i].sprite);
				team_2_vector[i].height = al_get_bitmap_height(team_2_vector[i].sprite);

				if (team_2_vector[i].alive == true) {
					al_draw_bitmap(team_2_vector[i].sprite, team_2_vector[i].x, team_2_vector[i].y, 0);

					//al_draw_line(team_2_vector[i].x, team_2_vector[i].y, team_2_vector[i].x + WORLD_WIDTH * cos(team_2_vector[i].target_radians), team_2_vector[i].y + WORLD_WIDTH * sin(team_2_vector[i].target_radians), al_map_rgb(0, 0, 255), 1);
				}
				else {
					if (team_2_vector.size() > 1) {
						team_2_vector.erase(team_2_vector.begin() + i);
					}
				}
			}

			if (rng.fire_frames == rng.fire_timer) {
				rng.fire_frames = 0;
				rng.proj.fired = true;
				rng.proj.x = rng.x, rng.proj.y = rng.y;
				rng.proj.tgt.target = rand() % team_2_vector.size();
				rng.proj.tgt.x = team_2_vector[rng.proj.tgt.target].x - team_2_vector[rng.proj.tgt.target].vx;
				team_2_vector[rng.proj.tgt.target].health = 0;
				rng.proj.tgt.y = team_2_vector[rng.proj.tgt.target].y;
				rng.proj.dx = rng.proj.tgt.x - rng.proj.x;
				rng.proj.speed = rng.proj.dx / rng.proj.flight_time;
				rng.proj.fire_angle = atan2(pow(GRAVITY * rng.proj.flight_time, 2), rng.proj.dx * 2);
				rng.proj.vx = rng.proj.speed;
				rng.proj.vy = (GRAVITY * rng.proj.flight_time / 2) / rng.proj.flight_time * sin(rng.proj.fire_angle);
			}
			if (rng.proj.fired == true) {
				x = rng.proj.x - 75 / 2;
				y = rng.proj.y - 75 / 2;
			}
			if (rng.proj.x > WORLD_WIDTH or rng.proj.x < 0 or rng.proj.y < 0 or rng.proj.y > WORLD_HEIGHT) {
				rng.proj.fired = false;
			}

			al_draw_circle(rng.x, rng.y, 2, al_map_rgb(255, 0, 0), 10);
			al_draw_circle(rng.proj.x, rng.proj.y, 2, al_map_rgb(0, 0, 0), 10);
			rng.proj.tgt.dz = sqrt(pow(rng.proj.y - rng.proj.tgt.y, 2) + pow(rng.proj.x - rng.proj.tgt.x, 2));


			for (int i = 0; i < team_2_vector.size(); i++) {
				if (sqrt(pow(team_2_vector[i].y - rng.proj.y, 2) + pow(team_2_vector[i].x - rng.proj.x, 2)) < rng.proj.tgt.explode_radius) {
					rng.proj.fired = false;
					rng.proj.x = -10;
					rng.proj.y = -10;
					rng.proj.land = true;
					rng.proj.can_explode = true;
					
					rng.proj.vx = 0, rng.proj.vy = 0;
				}
			}
			if (rng.proj.land == true) {
				rng.proj.land = false;
				elapsed_time = 0;
			}
			if (rng.proj.fired == true) {
				rng.fire_frames = 0;
				rng.proj.x += rng.proj.vx;
				rng.proj.y -= rng.proj.vy;
				rng.proj.vy += GRAVITY * sin(3.1459);
			}
			if (rng.proj.can_explode == true) {
				int FRAME_NUM = 8 * fmod(elapsed_time, 0.75) / 0.75;
				cout << FRAME_NUM << endl;
				al_draw_bitmap(explosion[FRAME_NUM], x, y, 0);
				if (FRAME_NUM == 7) {
					rng.proj.can_explode = false;
				}
			}
			//cout << rng.proj.x << "   " << rng.proj.y << "   " << rng.proj.vx << "   " << rng.proj.vy << "   " << rng.proj.fire_angle << "    " << rng.fire_frames << endl;

			red_counter = "Red Team: ";
			blue_counter = "Blue Team: ";
			al_draw_text(font, al_map_rgb(255, 0, 0), 0, 0, 0, red_counter.append(to_string(team_1_vector.size())).c_str());
			al_draw_text(font, al_map_rgb(0, 0, 255), WORLD_WIDTH - 150, 0, 0, blue_counter.append(to_string(team_2_vector.size())).c_str());
			al_flip_display();

		}//timer end
	}
	// while loop end

	al_uninstall_keyboard();
	al_destroy_bitmap(team_1_knight_attack_states);
	al_destroy_bitmap(team_1_knight_idle_states);
	al_destroy_bitmap(team_1_knight_movement_states);
	al_destroy_display(display);
	al_destroy_timer(timer);
}
