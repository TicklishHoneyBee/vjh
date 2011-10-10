/************************************************************************
* weapon.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* weapon.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* weapon.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

SDL_Surface *torpedo_surface = NULL;

weapon_t shots_active[50] = {
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0}
};

int weapon_get_bank(int ship, int x, int y)
{
	if (ship < -1)
		return -1;
	int c_x = ships[ship].pos.x+(ships[ship].surface->w/2);
	int c_y = ships[ship].pos.y+(ships[ship].surface->h/2);

	if (x > c_x) {
		if (!ships[ship].weapons[0] && !ships[ship].weapons[1])
			return -1;
		if (y > c_y) {
			if (!ships[ship].weapons[1])
				return -1;
			return 1;
		}else{
			if (!ships[ship].weapons[0])
				return -1;
			return 0;
		}
	}else{
		if (y > c_y) {
			if (!ships[ship].weapons[2])
				return -1;
			return 2;
		}else{
			if (!ships[ship].weapons[3])
				return -1;
			return 3;
		}
	}
}

static inline void s_draw_pixel(int x, int y, SDL_Color *c)
{
	SDL_Rect dest = {x,y,1,1};
	Uint32 color = SDL_MapRGB(screen->format,c->r,c->g,c->b);
	SDL_FillRect(screen,&dest,color);
}


void phaser(int ship, int x, int y)
{
	int bank = weapon_get_bank(ship,x,y);
	if (bank < 0)
		return;

	int id = -1;
	int i;
	int c = 0;
	for (i=0; i<50; i++) {
		if (shots_active[i].type == WEAPON_INACTIVE) {
			if (id < 0)
				id = i;
		}else if (shots_active[i].type == WEAPON_PHASER) {
			if (shots_active[i].ship == ship && shots_active[i].bank == bank)
				c++;
		}
	}

	if (c >= ships[ship].weapons[bank] || id < 0)
		return;

	sound_play(SND_PHASER);

	shots_active[id].type = WEAPON_PHASER;
	shots_active[id].ship = ship;
	shots_active[id].bank = bank;
	shots_active[id].tx = x;
	shots_active[id].ty = y;
	shots_active[id].ticks = 0;
	shots_active[id].pos = 0;
}

void draw_phaser(int id)
{
	int ox = ships[shots_active[id].ship].pos.x+ships[shots_active[id].ship].phaser[shots_active[id].bank][0];
	int oy = ships[shots_active[id].ship].pos.y+ships[shots_active[id].ship].phaser[shots_active[id].bank][1];
	int tx = shots_active[id].tx;
	int ty = shots_active[id].ty;

	int x = tx;
	int y = ty;

	if (shots_active[id].pos > 99) {
		if (!shots_active[id].ticks)
			shots_active[id].ticks = SDL_GetTicks();
	}else{
		float d_x = ox-tx;
		float d_y = oy-ty;

		float ll;
		float inc = 12;
		float ttx = d_x;
		float tty = d_y;
		if (ttx < 0)
			ttx *= -1;
		if (tty < 0)
			tty *= -1;

		ll = sqrtf((ttx*ttx)+(tty*tty));

		inc = 100.f/(ll/100.0f);

		float p = (float)(shots_active[id].pos)/100.0f;

		shots_active[id].pos += inc;

		x = ox - p * d_x;
		y = oy - p * d_y;
	}

	tx = x;
	ty = y;
	movement(id,tx,ty);

	if (tx < ox) {
		tx = ox;
		ox = x;
		ty = oy;
		oy = y;
	}

	float xdiff = (tx - ox);
	float ydiff = (ty - oy);

	if (xdiff == 0.0f && ydiff == 0.0f) {
		return;
	}

	float fx;
	float fy;
	float slope;
	float max;
	float min;

	if (fabs(xdiff) > fabs(ydiff)) {
		if (tx < ox) {
			min = tx;
			max = ox;
		}else{
			min = ox;
			max = tx;
		}

		slope = ydiff / xdiff;
		for (fx = min; fx <= max; fx += 1.0f) {
			fy = ty + ((fx - tx) * slope);
			s_draw_pixel(fx, fy, &ships[shots_active[id].ship].phaser_color);
		}
	}else{
		if (ty < oy) {
			min = ty;
			max = oy;
		}else{
			min = oy;
			max = ty;
		}

		slope = xdiff / ydiff;
		for (fy = min; fy <= max; fy += 1.0f) {
			fx = tx + ((fy - ty) * slope);
			s_draw_pixel(fx, fy, &ships[shots_active[id].ship].phaser_color);
		}
	}
}

void torpedo(int ship, int x, int y)
{
	if (!torpedo_surface)
		torpedo_surface = IMG_Load("images/torpedo.png");

	if (!ships[ship].weapons[4])
		return;

	int bank = weapon_get_bank(ship,x,y);
	if (bank < 0)
		return;

	int id = -1;
	int i;
	int c = 0;
	for (i=0; i<50; i++) {
		if (shots_active[i].type == WEAPON_INACTIVE) {
			if (id < 0)
				id = i;
		}else if (shots_active[i].type == WEAPON_TORPEDO) {
			if (shots_active[i].ship == ship && shots_active[i].bank == bank)
				c++;
		}
	}

	if (c > ships[ship].weapons[5])
		return;


	ships[ship].weapons[4]--;
	sound_play(SND_TORPEDO);

	shots_active[id].type = WEAPON_TORPEDO;
	shots_active[id].ship = ship;
	shots_active[id].bank = bank;
	shots_active[id].tx = x;
	shots_active[id].ty = y;
	shots_active[id].ox = ships[shots_active[id].ship].pos.x+ships[shots_active[id].ship].phaser[shots_active[id].bank][0];
	shots_active[id].oy = ships[shots_active[id].ship].pos.y+ships[shots_active[id].ship].phaser[shots_active[id].bank][1];
	shots_active[id].ticks = 0;
	shots_active[id].pos = 0;
}

void draw_torpedo(int id)
{
	float ox = shots_active[id].ox-10;
	float oy = shots_active[id].oy-10;
	float tx = shots_active[id].tx-10;
	float ty = shots_active[id].ty-10;

	int x;
	int y;

	if (shots_active[id].pos > 99) {
		if (!shots_active[id].ticks)
			shots_active[id].ticks = SDL_GetTicks();
		x = tx;
		y = ty;
	}else{
		float d_x = ox-tx;
		float d_y = oy-ty;

		float ll;
		float inc = 2;
		float ttx = d_x;
		float tty = d_y;
		if (ttx < 0)
			ttx *= -1;
		if (tty < 0)
			tty *= -1;

		ll = sqrtf((ttx*ttx)+(tty*tty));

		inc = 100.f/(ll/20.0f);

		float p = (float)(shots_active[id].pos)/100.0f;

		shots_active[id].pos += inc;

		x = ox - p * d_x;
		y = oy - p * d_y;
	}

	SDL_Rect dest = {x,y,0,0};
	SDL_BlitSurface(torpedo_surface,NULL,screen,&dest);

	movement(id,x,y);
}

void weapon_main()
{
	int ticks = SDL_GetTicks()-500;

	int i;
	for (i=0; i<50; i++) {
		if (!game_state || (shots_active[i].type == WEAPON_PHASER && !ships[shots_active[i].ship].onscreen)) {
			shots_active[i].type = WEAPON_INACTIVE;
			continue;
		}
		if (shots_active[i].type != WEAPON_INACTIVE) {
			if (shots_active[i].type == WEAPON_PHASER) {
				if (!shots_active[i].ticks || shots_active[i].ticks > ticks) {
					draw_phaser(i);
				}else{
					shots_active[i].type = WEAPON_INACTIVE;
				}
			}else if (shots_active[i].type == WEAPON_TORPEDO) {
				if (!shots_active[i].ticks || shots_active[i].ticks > ticks) {
					draw_torpedo(i);
				}else{
					shots_active[i].type = WEAPON_INACTIVE;
				}
			}
		}
	}
}
