/************************************************************************
* ships.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* ships.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ships.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

ship_t *ships = NULL;
int ships_cnt = SHIP_COUNT;
int sinit = 0;

void ships_init()
{
	ships = realloc(ships,sizeof(ship_t)*SHIP_COUNT);
	ships_cnt = SHIP_COUNT;

	int i;
	for (i=0; i<SHIP_COUNT; i++) {
		ships[i].pos.x = 300;
		ships[i].pos.y = 400;
		ships[i].pos.w = 0;
		ships[i].pos.h = 0;
		ships[i].drives[0] = 1;
		ships[i].drives[1] = 7;
		ships[i].drives[2] = 90;
		ships[i].drives[3] = 0;
		ships[i].speed[0] = 1;
		ships[i].speed[1] = 1;
		ships[i].onscreen = 0;
		ships[i].structural_integrity = 100;
		if (!sinit)
			ships[i].sheilds = NULL;
		ships[i].sheild_state[0] = 0;
		ships[i].sheild_state[1] = 100;

		ships[i].phaser_color.r = 200;
		ships[i].phaser_color.g = 0;
		ships[i].phaser_color.b = 0;

		ships[i].weapons[0] = 1;
		ships[i].weapons[1] = 1;
		ships[i].weapons[2] = 1;
		ships[i].weapons[3] = 1;
		ships[i].weapons[4] = 6;
		ships[i].weapons[5] = 1;
	}
	ships[VOYAGER].pos.x = (screen_size[0]/3)-100;
	ships[VOYAGER].pos.y = screen_size[1]/2;
	ships[VOYAGER].onscreen = 1;
	ships[VOYAGER].phaser[0][0] = 168;
	ships[VOYAGER].phaser[0][1] = 17;
	ships[VOYAGER].phaser[1][0] = 168;
	ships[VOYAGER].phaser[1][1] = 28;
	ships[VOYAGER].phaser[2][0] = 50;
	ships[VOYAGER].phaser[2][1] = 35;
	ships[VOYAGER].phaser[3][0] = 75;
	ships[VOYAGER].phaser[3][1] = 15;
	ships[VOYAGER].phaser_color.r = 200;
	ships[VOYAGER].phaser_color.g = 0;
	ships[VOYAGER].phaser_color.b = 0;

	ships[KAZON].phaser[0][0] = 77;
	ships[KAZON].phaser[0][1] = 17;
	ships[KAZON].phaser[1][0] = 89;
	ships[KAZON].phaser[1][1] = 45;
	ships[KAZON].phaser[2][0] = 8;
	ships[KAZON].phaser[2][1] = 52;
	ships[KAZON].phaser[3][0] = 18;
	ships[KAZON].phaser[3][1] = 17;
	ships[KAZON].phaser_color.r = 32;
	ships[KAZON].phaser_color.g = 120;
	ships[KAZON].phaser_color.b = 230;

	ships[HIROGEN].phaser[0][0] = 103;
	ships[HIROGEN].phaser[0][1] = 5;
	ships[HIROGEN].phaser[1][0] = 114;
	ships[HIROGEN].phaser[1][1] = 32;
	ships[HIROGEN].phaser[2][0] = 18;
	ships[HIROGEN].phaser[2][1] = 22;
	ships[HIROGEN].phaser[3][0] = 16;
	ships[HIROGEN].phaser[3][1] = 14;
	ships[HIROGEN].phaser_color.r = 200;
	ships[HIROGEN].phaser_color.g = 0;
	ships[HIROGEN].phaser_color.b = 0;

	ships[S8472].phaser[0][0] = 143;
	ships[S8472].phaser[0][1] = 35;
	ships[S8472].phaser[1][0] = 143;
	ships[S8472].phaser[1][1] = 35;
	ships[S8472].phaser[2][0] = 45;
	ships[S8472].phaser[2][1] = 35;
	ships[S8472].phaser[3][0] = 45;
	ships[S8472].phaser[3][1] = 35;
	ships[S8472].phaser_color.r = 236;
	ships[S8472].phaser_color.g = 236;
	ships[S8472].phaser_color.b = 30;

	ships[BORG_CUBE].phaser[0][0] = 175;
	ships[BORG_CUBE].phaser[0][1] = 55;
	ships[BORG_CUBE].phaser[1][0] = 190;
	ships[BORG_CUBE].phaser[1][1] = 135;
	ships[BORG_CUBE].phaser[2][0] = 50;
	ships[BORG_CUBE].phaser[2][1] = 145;
	ships[BORG_CUBE].phaser[3][0] = 60;
	ships[BORG_CUBE].phaser[3][1] = 50;
	ships[BORG_CUBE].phaser_color.r = 44;
	ships[BORG_CUBE].phaser_color.g = 210;
	ships[BORG_CUBE].phaser_color.b = 0;

	ships[BORG_SPHERE].phaser[0][0] = 74;
	ships[BORG_SPHERE].phaser[0][1] = 27;
	ships[BORG_SPHERE].phaser[1][0] = 74;
	ships[BORG_SPHERE].phaser[1][1] = 71;
	ships[BORG_SPHERE].phaser[2][0] = 26;
	ships[BORG_SPHERE].phaser[2][1] = 70;
	ships[BORG_SPHERE].phaser[3][0] = 26;
	ships[BORG_SPHERE].phaser[3][1] = 33;
	ships[BORG_SPHERE].phaser_color.r = 44;
	ships[BORG_SPHERE].phaser_color.g = 210;
	ships[BORG_SPHERE].phaser_color.b = 0;

	if (sinit)
		return;

	ships[VOYAGER].surface = IMG_Load("images/voyager.png");
	ships[VOYAGER].sheilds = IMG_Load("images/voyager_sheilds.png");

	ships[KAZON].surface = IMG_Load("images/kazon.png");
	ships[KAZON].sheilds = IMG_Load("images/kazon_sheilds.png");

	ships[HIROGEN].surface = IMG_Load("images/hirogen.png");
	ships[HIROGEN].sheilds = IMG_Load("images/hirogen_sheilds.png");

	ships[S8472].surface = IMG_Load("images/8472.png");
	ships[S8472].sheilds = IMG_Load("images/8472_sheilds.png");

	ships[BORG_CUBE].surface = IMG_Load("images/borg_cube.png");
	ships[BORG_CUBE].sheilds = IMG_Load("images/borg_cube_sheilds.png");

	ships[BORG_SPHERE].surface = IMG_Load("images/borg_sphere.png");
	ships[BORG_SPHERE].sheilds = IMG_Load("images/borg_sphere_sheilds.png");
	sinit = 1;
}

void ships_exit()
{
	int i;
	for (i=0; i<SHIP_COUNT; i++) {
		SDL_FreeSurface(ships[i].surface);
	}
}

void ships_main()
{
	int vx = 0;
	int vy = 0;
	if (game_state < 2) {
		if (key[0] && ships[VOYAGER].pos.y > 0)
			vy -= 6;
		if (key[1] && ships[VOYAGER].pos.y < (screen_size[1]-100))
			vy += 6;
		if (key[2] && ships[VOYAGER].pos.x > 50)
			vx -= 6;
		if (key[3] && ships[VOYAGER].pos.x < (screen_size[0]-200))
			vx += 6;

		ship_move(VOYAGER,vx,vy);

		if (ships[VOYAGER].speed[0] != ships[VOYAGER].speed[1]) {
			if (!ships[VOYAGER].speed[0] || !ships[VOYAGER].speed[1]) {
				ships[VOYAGER].speed[0] = 1;
			}else if (ships[VOYAGER].speed[1] > ships[VOYAGER].speed[0]) {
				ships[VOYAGER].speed[0] += (ships[VOYAGER].speed[1]/(ships[VOYAGER].speed[0] == 1 ? 5 : ships[VOYAGER].speed[0]));
				if (ships[VOYAGER].speed[0] > ships[VOYAGER].speed[1])
					ships[VOYAGER].speed[0] = ships[VOYAGER].speed[1];
			}else{
				ships[VOYAGER].speed[0] -= (ships[VOYAGER].speed[0]/ships[VOYAGER].speed[1]);
				if (ships[VOYAGER].speed[0] < ships[VOYAGER].speed[1])
					ships[VOYAGER].speed[0] = ships[VOYAGER].speed[1];
			}
		}
	}

	int ticks = SDL_GetTicks();
	int i;
	SDL_Rect pos;
	for (i=0; i<ships_cnt; i++) {
		if (ships[i].onscreen) {
			pos = ships[i].pos;
			SDL_BlitSurface(ships[i].surface,NULL,screen,&pos);
			if (ships[i].sheild_state[0] > ticks && ships[i].sheild_state[1] && ships[i].sheilds) {
				pos = ships[i].pos;
				SDL_BlitSurface(ships[i].sheilds,NULL,screen,&pos);
			}
		}
	}
}

void ship_move(int id, int xinc, int yinc)
{
	if (id >= ships_cnt)
		return;

	ships[id].pos.x += xinc;
	ships[id].pos.y += yinc;
}

int ship_clone(int type)
{
	int r = ships_cnt;
	ships_cnt++;
	ship_t *ns = realloc(ships,sizeof(ship_t)*ships_cnt);
	if (!ns)
		return -1;

	ships = ns;
	ships[r].pos.x = 300;
	ships[r].pos.y = 400;
	ships[r].pos.w = 0;
	ships[r].pos.h = 0;
	ships[r].drives[0] = 1;
	ships[r].drives[1] = 7;
	ships[r].drives[2] = 90;
	ships[r].drives[3] = 0;
	ships[r].speed[0] = 1;
	ships[r].speed[1] = 1;
	ships[r].onscreen = 0;
	ships[r].surface = ships[type].surface;
	ships[r].structural_integrity = 100;
	ships[r].sheilds = ships[type].sheilds;
	ships[r].sheild_state[0] = 0;
	ships[r].sheild_state[1] = 100;

	ships[r].phaser[0][0] = ships[type].phaser[0][0];
	ships[r].phaser[0][1] = ships[type].phaser[0][1];
	ships[r].phaser[1][0] = ships[type].phaser[1][0];
	ships[r].phaser[1][1] = ships[type].phaser[1][1];
	ships[r].phaser[2][0] = ships[type].phaser[2][0];
	ships[r].phaser[2][1] = ships[type].phaser[2][1];
	ships[r].phaser[3][0] = ships[type].phaser[3][0];
	ships[r].phaser[3][1] = ships[type].phaser[3][1];

	ships[r].phaser_color.r = ships[type].phaser_color.r;
	ships[r].phaser_color.g = ships[type].phaser_color.g;
	ships[r].phaser_color.b = ships[type].phaser_color.b;

	ships[r].weapons[0] = 1;
	ships[r].weapons[1] = 1;
	ships[r].weapons[2] = 1;
	ships[r].weapons[3] = 1;
	ships[r].weapons[4] = 6;
	
	ships[r].y_mot = rand_range(-1,1);

	return r;
}
