/************************************************************************
* collision.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* collision.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* collision.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

typedef struct motion_s {
	int x;
	int y;
	int id;
	int ticks;
	struct motion_s *next;
} motion_t;

typedef struct explosion_s {
	int x;
	int y;
	int wid1;
	int wid2;
	int sid1;
	int sid2;
	int ticks;
	int pu;
	struct explosion_s *next;
} explosion_t;

typedef struct pickup_s {
	int type;
	int value;
	int hit;
	int ticks;
	int x;
	int y;
	struct pickup_s *next;
} pickup_t;

motion_t *motions = NULL;
explosion_t *explosions = NULL;
pickup_t *pickups;

static void s_collision_disable_weapon(int id, int x, int y)
{
	int w = weapon_get_bank(id,x,y);
	if (w > -1)
		ships[id].weapons[w] = 0;
}

void pickup_add(int x, int y, int type, int value)
{
	pickup_t *put;
	pickup_t *pu = malloc(sizeof(pickup_t));
	if (!pu)
		return;
	pu->x = x;
	pu->y = y;
	pu->type = type;
	pu->hit = 0;
	if (type == PU_TRANSWARP) {
		pu->hit = 1;
		pu->ticks = SDL_GetTicks()+700;
	}
	pu->value = value;
	pu->next = NULL;
	put = pickups;
	if (put) {
		while (put->next) {
			put = put->next;
		}
		put->next = pu;
	}else{
		pickups = pu;
	}
}

void movement(int id, int x, int y)
{
	motion_t *m = malloc(sizeof(motion_t));
	if (!m)
		return;
	motion_t *p = motions;
	if (p) {
		while (p->next) {
			p = p->next;
		}
		p->next = m;
	}else{
		motions = m;
	}
	m->x = x;
	m->y = y;
	m->id = id;
	m->ticks = SDL_GetTicks();
	if (shots_active[id].type == WEAPON_PHASER) {
		m->ticks += 500;
	}else if (shots_active[id].type == WEAPON_TORPEDO) {
		m->ticks += 100;
	}
	m->next = NULL;
}

void collision_detect()
{
	motion_t *p = NULL;
	motion_t *m = motions;
	motion_t *t;
	int ticks = SDL_GetTicks();
	int i;
	int h;
	int slx;
	int sly;
	Uint32 px;
	Uint32 pxt;
	Uint8 pxa;
	SDL_PixelFormat *fmt;

	while (m) {
		h = 0;
		if (m->ticks < ticks || !game_state) {
			t = m;
			m = m->next;
			if (p) {
				p->next = t->next;
			}else{
				motions = t->next;
			}
			free(t);
			continue;
		}else{
			for (i=0; i<ships_cnt; i++) {
				if (!ships[i].onscreen || i == shots_active[m->id].ship || (boss_mode && i != VOYAGER && shots_active[m->id].ship != VOYAGER))
					continue;
				slx = (ships[i].pos.x+ships[i].surface->w);
				sly = (ships[i].pos.y+ships[i].surface->h);
				if (!(m->x > ships[i].pos.x && m->x < slx && m->y > ships[i].pos.y && m->y < sly))
					continue;

				fmt = ships[i].sheilds->format;
				slx = (m->x-ships[i].pos.x);
				sly = (m->y-ships[i].pos.y);

				SDL_LockSurface(ships[i].sheilds);
				px = (((Uint32*)ships[i].sheilds->pixels))[(sly*ships[i].sheilds->w)+slx];
				SDL_UnlockSurface(ships[i].sheilds);

				pxt = px & fmt->Amask;  /* Isolate alpha component */
				pxt = pxt >> fmt->Ashift; /* Shift it down to 8-bit */
				pxt = pxt << fmt->Aloss;  /* Expand to a full 8-bit number */
				pxa = (Uint8)pxt;

				if (pxa > 100)
					continue;
				collision(i,-1,m->id,-1,m->x,m->y);
				h = 1;
			}
			if (!h) {
				t = motions;
				while (t) {
					if (t->id != m->id) {
						if (t->x == m->x) {
							if (t->y == m->y || (t->y+3 > m->y && t->y-3 < m->y)) {
								collision(-1,-1,t->id,m->id,m->x,m->y);
							}
						}else if (t->y == m->y) {
							if (t->x+3 > m->x && t->x-3 < m->x) {
								collision(-1,-1,t->id,m->id,m->x,m->y);
							}
						}
					}
					t = t->next;
				}
			}
		}
		p = m;
		m = m->next;
	}

	int k;
	int slx2;
	int sly2;
	for (i=0; i<ships_cnt; i++){
		if (!ships[i].onscreen || !game_state)
			continue;
		slx = (ships[i].pos.x+ships[i].surface->w);
		sly = (ships[i].pos.y+ships[i].surface->h);
		for (k=i+1; k<ships_cnt; k++) {
			if (!ships[k].onscreen || !game_state || (boss_mode && i != VOYAGER && k != VOYAGER))
				continue;
			slx2 = (ships[k].pos.x+ships[k].surface->w);
			sly2 = (ships[k].pos.y+ships[k].surface->h);
			if (
				(
					(
						ships[i].surface->w < ships[k].surface->w
						&& (
							(
								ships[i].pos.x > ships[k].pos.x
								&& ships[i].pos.x < slx2
							) || (
								slx > ships[k].pos.x
								&& slx < slx2
							)
						)
					) || (
							(
								ships[k].pos.x > ships[i].pos.x
								&& ships[k].pos.x < slx
							) || (
								slx2 > ships[i].pos.x
								&& slx2 < slx
							)
					)
				) && (
					(
						ships[i].surface->h < ships[k].surface->h
						&& (
							(
								ships[i].pos.y > ships[k].pos.y
								&& ships[i].pos.y < sly2
							) || (
								sly > ships[k].pos.y
								&& sly < sly2
							)
						)
					) || (
							(
								ships[k].pos.y > ships[i].pos.y
								&& ships[k].pos.y < sly
							) || (
								sly2 > ships[i].pos.y
								&& sly2 < sly
							)
					)
				)
			) {
				int bx = ships[i].pos.x+(ships[i].surface->w/2);
				int by = ships[i].pos.y+(ships[i].surface->h/2);
				int ex = ships[k].pos.x+(ships[k].surface->w/2);
				int ey = ships[k].pos.y+(ships[k].surface->h/2);
				int fx = 0;
				int fy = 0;
				float d_x = bx-ex;
				float d_y = by-ey;

				float ttx = d_x;
				float tty = d_y;
				if (ttx < 0)
					ttx *= -1;
				if (tty < 0)
					tty *= -1;

				fx = bx - 0.5f * d_x;
				fy = by - 0.5f * d_y;
				collision(i,k,-1,-1,fx,fy);
			}
		}
	}

	explosion_t *e = explosions;
	explosion_t *te = NULL;
	explosion_t *pe = NULL;
	pickup_t *pu;
	pickup_t *put;
	SDL_Rect dest = {0,0,0,0};
	while (e) {
		if (e->ticks < ticks || !game_state) {
			te = e;
			e = e->next;
			if (pe) {
				pe->next = te->next;
			}else{
				explosions = te->next;
			}
			if (te->pu) {
				pu = malloc(sizeof(pickup_t));
				if (pu) {
					pu->x = te->x;
					pu->y = te->y;
					pu->type = te->pu;
					pu->hit = 0;
					if (pu->type == PU_PHASER) {
						pu->value = rand_range(1,2);
					}else{
						pu->value = rand_range(1,5);
					}
					pu->next = NULL;
					put = pickups;
					if (put) {
						while (put->next) {
							put = put->next;
						}
						put->next = pu;
					}else{
						pickups = pu;
					}
				}
			}
			free(te);
			continue;
		}else{
			dest.x = e->x-12;
			dest.y = e->y-12;
			SDL_BlitSurface(explosion,NULL,screen,&dest);
			if (e->sid1 > -1 && e->sid2 == -1 && ships[e->sid1].onscreen) {
				hud_print(e->x,(e->y+((e->ticks-ticks)/10))-60,NULL,ORANGE,"%d%%",ships[e->sid1].sheild_state[1]);
			}
		}
		pe = e;
		e = e->next;
	}

	pu = pickups;
	pickup_t *pup = NULL;
	put = NULL;
	while (pu) {
		if (!game_state || pu->x < 1 || (pu->hit && pu->ticks < ticks)) {
			put = pu;
			pu = pu->next;
			if (pup) {
				pup->next = put->next;
			}else{
				pickups = put->next;
			}
			free(put);
			continue;
		}else if (pu->hit){
			int c = RED;
			if (ticks%5 < 3)
				c = ORANGE;
			if (pu->type == PU_PARTS) {
				if (pu->value == -1) {
					hud_print(pu->x,(pu->y+((pu->ticks-ticks)/10))-60,NULL,c,"TransWarp Coil");
				}else{
					hud_print(pu->x,(pu->y+((pu->ticks-ticks)/10))-60,NULL,c,"Integrity +%d%%",pu->value*5);
				}
			}else if (pu->type == PU_PHASER) {
				hud_print(pu->x,(pu->y+((pu->ticks-ticks)/10))-60,NULL,c,"Phasers +%d",pu->value);
			}else if (pu->type == PU_SHEILDS) {
				hud_print(pu->x,(pu->y+((pu->ticks-ticks)/10))-60,NULL,c,"Sheilds +%d%%",pu->value*10);
			}else if (pu->type == PU_TORPEDO) {
				hud_print(pu->x,(pu->y+((pu->ticks-ticks)/10))-60,NULL,c,"Torpedos +%d",pu->value);
			}else if (pu->type == PU_LIFE) {
				hud_print(pu->x,(pu->y+((pu->ticks-ticks)/10))-60,NULL,c,"Life +1");
			}else if (pu->type == PU_TRANSWARP) {
				hud_print(pu->x,(pu->y+((pu->ticks-ticks)/10))-60,NULL,c,"TransWarp");
			}
		}else if (
			(pu->x+20) > ships[VOYAGER].pos.x
			&& pu->x < (ships[VOYAGER].pos.x+ships[VOYAGER].surface->w)
			&& (pu->y+20) > ships[VOYAGER].pos.y
			&& pu->y < (ships[VOYAGER].pos.y+ships[VOYAGER].surface->h)
		) {
			pu->hit = 1;
			pu->ticks = ticks+700;
			if (pu->type == PU_PARTS) {
				if (!ships[VOYAGER].drives[0]) {
					ships[VOYAGER].drives[0] = 1;
				}else if (!ships[VOYAGER].drives[1]) {
					ships[VOYAGER].drives[1] = 8;
				}else if (!ships[VOYAGER].drives[2]) {
					ships[VOYAGER].drives[2] = 90;
				}else if (ships[VOYAGER].structural_integrity != 100) {
					ships[VOYAGER].structural_integrity += (pu->value*5);
				}else if (level == BORG_CUBE) {
					ships[VOYAGER].drives[3]++;
					pu->value = -1;
				}
			}else if (pu->type == PU_PHASER) {
				k = pu->value;
				for (i=0; i<4; i++) {
					if (!ships[VOYAGER].weapons[i]) {
						ships[VOYAGER].weapons[i]++;
						k--;
					}
					if (!k)
						break;
				}
				for (h=0; k && h<2; h++) {
					for (i=0; i<4; i++) {
						if (ships[VOYAGER].weapons[i] < 2) {
							ships[VOYAGER].weapons[i]++;
							k--;
						}
						if (!k)
							break;
					}
				}
			}else if (pu->type == PU_SHEILDS) {
				ships[VOYAGER].sheild_state[1] += (pu->value*10);
				if (ships[VOYAGER].sheild_state[1] > 100)
					ships[VOYAGER].sheild_state[1] = 100;
			}else if (pu->type == PU_TORPEDO) {
				ships[VOYAGER].weapons[4] += pu->value;
				if (pu->value > 4 && ships[VOYAGER].weapons[5] < 3)
					ships[VOYAGER].weapons[5]++;
			}else if (pu->type == PU_LIFE) {
				lives += 1;
			}
		}else{
			dest.x = pu->x-20;
			dest.y = pu->y-20;
			SDL_BlitSurface(pickup,NULL,screen,&dest);
			pu->x -= 4;
		}
		pup = pu;
		pu = pu->next;
	}
}

void collision(int sid1, int sid2, int wid1, int wid2, int x, int y)
{
	if (wid1 > -1 && wid2 > -1 && shots_active[wid1].type == WEAPON_PHASER && shots_active[wid2].type == WEAPON_PHASER)
		return;

	explosion_t *p = explosions;
	while (p) {
		if ((p->wid1 == wid1 && p->wid2 == wid2 && (p->x == x || p->y == y)) || (sid1 > -1 && sid2 > -1 && p->sid1 == sid1 && p->sid2 == sid2) || (wid1 > -1 && sid1 > -1 && p->wid1 == wid1 && p->sid1 == sid1)) {
			return;
		}
		p = p->next;
	}

	explosion_t *e = malloc(sizeof(explosion_t));
	if (!e)
		return;

	e->x = x;
	e->y = y;
	e->wid1 = wid1;
	e->wid2 = wid2;
	e->sid1 = sid1;
	e->sid2 = sid2;
	e->next = NULL;
	e->pu = PU_NONE;
	e->ticks = SDL_GetTicks()+1000;
	sound_play(SND_EXPLOSION);
	p = explosions;
	if (p) {
		while (p->next) {
			p = p->next;
		}
		p->next = e;
	}else{
		explosions = e;
	}
	int i;
	int k;

	if (wid1 > -1 && wid2 > -1) {
		if (shots_active[wid1].type == WEAPON_TORPEDO) {
			shots_active[wid1].tx = x;
			shots_active[wid1].ty = y;
			shots_active[wid1].pos = 100;
			shots_active[wid1].ticks = 1;
		}
		if (shots_active[wid2].type == WEAPON_TORPEDO) {
			shots_active[wid2].tx = x;
			shots_active[wid2].ty = y;
			shots_active[wid2].pos = 100;
			shots_active[wid2].ticks = 1;
		}
	}else if (wid1 > -1 && sid1 > -1) {
		shots_active[wid1].tx = x;
		shots_active[wid1].ty = y;
		shots_active[wid1].pos = 100;
		ships[sid1].sheild_state[0] = e->ticks+200;
		if (shots_active[wid1].ship) {
			if (shots_active[wid1].type == WEAPON_TORPEDO) {
				e->ticks -= 200;
				shots_active[wid1].ticks = 1;
				ships[sid1].sheild_state[1] -= 30;
				if (ships[sid1].sheild_state[1] < 10) {
					if (ships[sid1].sheild_state[1] < 1) {
						ships[sid1].sheild_state[1] = 0;
						ships[sid1].structural_integrity -= 80;
					}else{
						ships[sid1].structural_integrity -= 10;
					}
				}
			}else{
				ships[sid1].sheild_state[1] -= 20;
				if (ships[sid1].sheild_state[1] < 10) {
					if (ships[sid1].sheild_state[1] < 1) {
						ships[sid1].sheild_state[1] = 0;
						ships[sid1].structural_integrity -= 30;
					}else{
						ships[sid1].structural_integrity -= 1;
					}
				}
			}
		}else{
			if (shots_active[wid1].type == WEAPON_TORPEDO) {
				e->ticks -= 200;
				shots_active[wid1].ticks = 1;
				ships[sid1].sheild_state[1] -= 40;
				if (ships[sid1].sheild_state[1] < 10) {
					if (ships[sid1].sheild_state[1] < 1) {
						ships[sid1].sheild_state[1] = 0;
						ships[sid1].structural_integrity -= 80;
					}else{
						ships[sid1].structural_integrity -= 20;
					}
				}
			}else{
				ships[sid1].sheild_state[1] -= 30;
				if (ships[sid1].sheild_state[1] < 10) {
					if (ships[sid1].sheild_state[1] < 1) {
						ships[sid1].sheild_state[1] = 0;
						ships[sid1].structural_integrity -= 40;
					}else{
						ships[sid1].structural_integrity -= 5;
					}
				}
			}
		}
		if (ships[sid1].structural_integrity < 90) {
			s_collision_disable_weapon(sid1,x,y);
			ships[sid1].drives[3] = 0;
			if (ships[sid1].structural_integrity < 40) {
				ships[sid1].drives[0] = 0;
			}
			if (ships[sid1].structural_integrity < 60) {
				ships[sid1].drives[1] = 0;
			}
			if (ships[sid1].structural_integrity < 80) {
				ships[sid1].drives[2] = 0;
			}
			for (i=3; i>-1; i--) {
				if (ships[sid1].drives[i] < ships[sid1].speed[1])
					ships[sid1].speed[1] = ships[sid1].drives[i];
				if (ships[sid1].speed[1] > 0)
					break;
			}
			if (ships[sid1].structural_integrity < 1) {
				ships[sid1].onscreen = 0;
				e = malloc(sizeof(explosion_t));
				if (!e)
					return;

				e->x = ships[sid1].pos.x+(ships[sid1].surface->w/2);
				e->y = ships[sid1].pos.y+(ships[sid1].surface->h/2);
				e->wid1 = wid1;
				e->wid2 = wid2;
				e->sid1 = sid1;
				e->sid2 = sid2;
				e->next = NULL;
				if (sid1 == VOYAGER) {
					e->pu = PU_NONE;
				}else{
					k = 2;
					for (i=0; i<4; i++) {
						if (ships[VOYAGER].weapons[i] < 2)
							k = 1;
					}
					e->pu = rand_range(k,4);
				}
				e->ticks = SDL_GetTicks()+400;
				sound_play(SND_EXPLOSION);
				p = explosions;
				if (p) {
					while (p->next) {
						p = p->next;
					}
					p->next = e;
				}else{
					explosions = e;
				}
			}
		}
	}else if (sid1 > -1 && sid2 > -1) {
		ships[sid1].sheild_state[1] = 0;
		ships[sid1].structural_integrity -= 90;
		ships[sid1].drives[1] = 0;
		ships[sid1].drives[2] = 0;
		ships[sid1].drives[3] = 0;
		for (i=3; i>-1; i--) {
			ships[sid1].speed[1] = ships[sid1].drives[i];
			if (ships[sid1].speed[1] > 0)
				break;
		}

		ships[sid2].sheild_state[1] = 0;
		ships[sid2].structural_integrity -= 90;
		ships[sid2].drives[1] = 0;
		ships[sid2].drives[2] = 0;
		ships[sid2].drives[3] = 0;
		for (i=3; i>-1; i--) {
			ships[sid2].speed[1] = ships[sid2].drives[i];
			if (ships[sid2].speed[1] > 0)
				break;
		}

		if (ships[sid1].structural_integrity < 1) {
			if (ships[sid2].structural_integrity < 1)
				ships[sid2].onscreen = 0;

			ships[sid1].onscreen = 0;
			e = malloc(sizeof(explosion_t));
			if (!e)
				return;

			e->x = ships[sid1].pos.x+(ships[sid1].surface->w/2);
			e->y = ships[sid1].pos.y+(ships[sid1].surface->h/2);
			e->wid1 = wid1;
			e->wid2 = wid2;
			e->sid1 = sid1;
			e->sid2 = sid2;
			e->next = NULL;
			if (sid1 == VOYAGER) {
				e->pu = PU_NONE;
			}else{
				e->pu = rand_range(1,4);
			}
			e->ticks = SDL_GetTicks()+400;
			sound_play(SND_EXPLOSION);
			p = explosions;
			if (p) {
				while (p->next) {
					p = p->next;
				}
				p->next = e;
			}else{
				explosions = e;
			}
		}
		if (ships[sid2].structural_integrity < 1) {
			ships[sid2].onscreen = 0;
			e = malloc(sizeof(explosion_t));
			if (!e)
				return;

			e->x = ships[sid2].pos.x+(ships[sid2].surface->w/2);
			e->y = ships[sid2].pos.y+(ships[sid2].surface->h/2);
			e->wid1 = wid1;
			e->wid2 = wid2;
			e->sid1 = sid1;
			e->sid2 = sid2;
			e->next = NULL;
			if (sid2 == VOYAGER) {
				e->pu = PU_NONE;
			}else{
				e->pu = rand_range(1,4);
			}
			e->ticks = SDL_GetTicks()+400;
			sound_play(SND_EXPLOSION);
			p = explosions;
			if (p) {
				while (p->next) {
					p = p->next;
				}
				p->next = e;
			}else{
				explosions = e;
			}
		}
	}
}
