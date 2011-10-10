/************************************************************************
* game.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* game.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* game.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

int game_state;
int level;
int lives;
int active_ship;
int level_ships;
int boss_mode;
int boss_ships[20];
time_t s_time;
time_t w_time;

int rand_range(int low, int high)
{
	if (low >= high) return low;
	int c;
	if (low == 0) {
		c = (rand() % high);
	}else{
		c = (rand() % (high-low+1)+low);
	}
	return c;
}

void game_start()
{
	active_ship = 0;
	game_state = 1;
	level = 1;
	level_ships = 0;
	boss_mode = 0;
	lives = 3;
	s_time = time(NULL);
	w_time = 0;
	srand((unsigned int)s_time);
}

void game_exit()
{
	ships_init();
	game_state = 0;
	key[0] = 0;
	key[1] = 0;
	key[2] = 0;
	key[3] = 0;
}

void game_main()
{
	if (game_state != 1)
		return;

	time_t now = time(NULL);

	if (!ships[VOYAGER].onscreen) {
		if (--lives < 1) {
			game_exit();
			return;
		}
		s_time = now;
		ships[VOYAGER].structural_integrity = 100;
		ships[VOYAGER].sheild_state[0] = 0;
		ships[VOYAGER].sheild_state[1] = 100;
		ships[VOYAGER].drives[0] = 1;
		ships[VOYAGER].drives[1] = 8;
		ships[VOYAGER].drives[2] = 90;
		ships[VOYAGER].drives[3] = 0;
		ships[VOYAGER].weapons[0] = 1;
		ships[VOYAGER].weapons[1] = 1;
		ships[VOYAGER].weapons[2] = 1;
		ships[VOYAGER].weapons[3] = 1;
		ships[VOYAGER].weapons[4] = 6;
		ships[VOYAGER].weapons[5] = 1;
		ships[VOYAGER].pos.x = (screen_size[0]/3)-100;
		ships[VOYAGER].pos.y = screen_size[1]/2;
		ships[VOYAGER].onscreen = 1;
		if (active_ship) {
			ships[active_ship].onscreen = 0;
			active_ship = 0;
		}
		return;
	}
	
	if (level >= SHIP_COUNT) {
	}

	int s;
	int i;
	int k;
	int y;

	if (boss_mode) {
		s = level*3;
		if (!boss_ships[0]) {
			int o = (screen_size[1]/(s*1.5));
			y = o;
			for (i=0; i<s; i++) {
				boss_ships[i] = ship_clone(level);
				if (boss_ships[i] < 0) {
					s = i;
					for (i=0; i<s; i++) {
						ships[boss_ships[i]].onscreen = 0;
						boss_ships[i] = 0;
					}
					return;
				}
				ships[boss_ships[i]].onscreen = 1;
				ships[boss_ships[i]].pos.x = screen_size[0];
				ships[boss_ships[i]].pos.y = y;
				y += o;
			}
		}else if (ships[boss_ships[0]].pos.x > screen_size[0]-300) {
			for (i=0; i<s; i++) {
				if (!(i%2) || ships[boss_ships[i]].pos.x > screen_size[0]-150)
					ships[boss_ships[i]].pos.x -= 5;
			}
		}else{
			k = 0;
			for (i=0; i<s; i++) {
				if (ships[boss_ships[i]].onscreen)
					k = 1;
			}
			if (!k) {
				boss_ships[0] = 0;
				boss_mode = 0;
				level++;
				y = rand_range(100,screen_size[1]-100);
				pickup_add(screen_size[0]-200,y,PU_LIFE,1);
				pickup_add(screen_size[0]-150,y,PU_PARTS,5);
				pickup_add(screen_size[0]-100,y,PU_SHEILDS,level);
				pickup_add(screen_size[0]-50,y,PU_PHASER,level);
				pickup_add(screen_size[0],y,PU_TORPEDO,level);
				s_time = now+4;
			}
			if (w_time >= now)
				return;
			k = rand_range(1,s);
			if (!ships[boss_ships[k-1]].onscreen)
				return;
			phaser(
				boss_ships[k-1],
				rand_range(ships[VOYAGER].pos.x-100,ships[VOYAGER].pos.x+300),
				rand_range(ships[VOYAGER].pos.y-500,ships[VOYAGER].pos.y+100)
			);
			w_time = now;
		}
		return;
	}

	if (!active_ship && s_time < now) {
		level_ships++;
		active_ship = ship_clone(level);
		if (active_ship < 0) {
			active_ship = 0;
			return;
		}
		ships[active_ship].onscreen = 1;
		ships[active_ship].pos.x = screen_size[0];
		ships[active_ship].pos.y = rand_range(10,(screen_size[1]-50)-ships[active_ship].surface->h);
		if (ships[active_ship].pos.y > (screen_size[1]*0.75)) {
			ships[active_ship].y_mot = -1;
		}else if (ships[active_ship].pos.y < (screen_size[1]/4)) {
			ships[active_ship].y_mot = 1;
		}
	}else if (active_ship && !ships[active_ship].onscreen) {
		s_time = now+((10/(ships[VOYAGER].speed[0]+1))+1);
		active_ship = 0;
		if (level_ships > 20) {
			boss_mode = 1;
			s_time = now+5;
			level_ships = 0;
			if (level == SHIP_COUNT)
				game_exit();
		}
	}else if (active_ship) {
		if (w_time < now) {
			if (level < 2) {
				w_time = now+2;
			}else if (level < 4) {
				w_time = now+1;
			}else{
				w_time = now;
			}
			phaser(
				active_ship,
				rand_range(ships[VOYAGER].pos.x-150,ships[VOYAGER].pos.x+350),
				rand_range(ships[VOYAGER].pos.y-100,ships[VOYAGER].pos.y+150)
			);
		}
		s = 3*level;
		if (level > 1 && ships[active_ship].pos.x > ships[VOYAGER].pos.x-100 && ships[active_ship].pos.x < ships[VOYAGER].pos.x+300)
			s /= 1.5;
		ship_move(active_ship,-s,ships[active_ship].y_mot);
		if (ships[active_ship].pos.x < -ships[active_ship].surface->w)
			ships[active_ship].onscreen = 0;
		if (ships[active_ship].pos.y > (screen_size[1]-(ships[active_ship].surface->h+30)))
			ships[active_ship].y_mot = -1;
		if (ships[active_ship].pos.y < 0)
			ships[active_ship].y_mot = 1;
	}
}
