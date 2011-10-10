/************************************************************************
* hud.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* hud.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* hud.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

char asym[10];
char* sym_to_string(int s)
{
	if (controls[s].type == CTRL_MOUSE) {
		if (controls[s].sym == SDL_BUTTON_LEFT) {
		sprintf(asym,"mouse 1");
		}else if (controls[s].sym == SDL_BUTTON_RIGHT) {
		sprintf(asym,"mouse 2");
		}else{
		sprintf(asym,"???");
		}
		return asym;
	}else if (isgraph(controls[s].sym)) {
		sprintf(asym,"%c",controls[s].sym);
		return asym;
	}else if (controls[s].sym == SDLK_ESCAPE) {
		sprintf(asym,"esc");
		return asym;
	}else if (controls[s].sym == SDLK_SPACE) {
		sprintf(asym,"spc");
		return asym;
	}else if (controls[s].sym == SDLK_KP0) {
		sprintf(asym,"kp-0");
		return asym;
	}else if (controls[s].sym == SDLK_KP1) {
		sprintf(asym,"kp-1");
		return asym;
	}else if (controls[s].sym == SDLK_KP2) {
		sprintf(asym,"kp-2");
		return asym;
	}else if (controls[s].sym == SDLK_KP3) {
		sprintf(asym,"kp-3");
		return asym;
	}else if (controls[s].sym == SDLK_KP4) {
		sprintf(asym,"kp-4");
		return asym;
	}else if (controls[s].sym == SDLK_KP5) {
		sprintf(asym,"kp-5");
		return asym;
	}else if (controls[s].sym == SDLK_KP6) {
		sprintf(asym,"kp-6");
		return asym;
	}else if (controls[s].sym == SDLK_KP7) {
		sprintf(asym,"kp-7");
		return asym;
	}else if (controls[s].sym == SDLK_KP8) {
		sprintf(asym,"kp-8");
		return asym;
	}else if (controls[s].sym == SDLK_KP9) {
		sprintf(asym,"kp-9");
		return asym;
	}else if (controls[s].sym == SDLK_UP) {
		sprintf(asym,"up");
		return asym;
	}else if (controls[s].sym == SDLK_DOWN) {
		sprintf(asym,"down");
		return asym;
	}else if (controls[s].sym == SDLK_RIGHT) {
		sprintf(asym,"right");
		return asym;
	}else if (controls[s].sym == SDLK_LEFT) {
		sprintf(asym,"left");
		return asym;
	}else{
		sprintf(asym,"???");
		return asym;
	}
	return NULL;
}

struct res_s {
	char* name;
	int x;
	int y;
} resolutions[6] = {
	{"1024x768",1024,768},
	{"1280x1024",1280,1024},
	{"1440x900",1440,900},
	{"1920x1080",1920,1080},
	{NULL,0,0},
	{NULL,0,0}
};

int c_res[2] = {5,5};

SDL_Rect hud_render_src = {0,0,0,0};
SDL_Rect hud_render_dest = {0,0,0,0};
SDL_Rect hud_font_dest = {0,0,0,0};
SDL_Surface *border;
SDL_Surface *button;
SDL_Surface *hud;
TTF_Font *hud_font;
TTF_Font *game_font;
TTF_Font *hud_title_font;
SDL_Color hud_color[4] = {
	{255,165,0},
	{0,0,0},
	{10,124,0},
	{165,0,2}
};

int menu_state = 0;

static void s_reset_render()
{
	hud_render_src.x = 0;
	hud_render_src.y = 0;
	hud_render_src.w = 0;
	hud_render_src.h = 0;
	hud_render_dest.x = 0;
	hud_render_dest.y = 0;
	hud_render_dest.w = 0;
	hud_render_dest.h = 0;
}

void hud_init()
{
	asprintf(&resolutions[4].name,"%dx%d",vi->current_w,vi->current_h);
	resolutions[4].x = vi->current_w;
	resolutions[4].y = vi->current_h;

	asprintf(&resolutions[5].name,"%dx%d",screen_size[0],screen_size[1]);
	resolutions[5].x = screen_size[0];
	resolutions[5].y = screen_size[1];

	border = IMG_Load("images/border.png");
	button = IMG_Load("images/button.png");
	hud_font = TTF_OpenFont("fonts/trek.ttf", 14);
	game_font = TTF_OpenFont("fonts/trek.ttf", 8);
	hud_title_font = TTF_OpenFont("fonts/trek.ttf", 28);
}

void hud_exit()
{
	TTF_CloseFont(hud_font);
	TTF_CloseFont(game_font);
	TTF_CloseFont(hud_title_font);
	SDL_FreeSurface(border);
	SDL_FreeSurface(button);
}

void hud_main()
{
	if (!game_state) {
		hud_menu();
	}else{
		hud_game();
	}
}

void hud_menu_exit()
{
	if (menu_state == 1) {
		c_res[0] = c_res[1];
		screen_size[0] = resolutions[c_res[0]].x;
		screen_size[1] = resolutions[c_res[0]].y;
		int opts = SDL_SWSURFACE | SDL_DOUBLEBUF;
		if (screen_size[2])
			opts |= SDL_FULLSCREEN;
		screen = SDL_SetVideoMode(screen_size[0],screen_size[1],32,opts);
	}else if (menu_state == 2) {
	}
	menu_state = 0;
}

void hud_menu()
{
	int i;
	s_reset_render();
	hud_render_src.x = 199;
	hud_render_src.w = 1;
	hud_render_src.h = 230;
	for (i=200; i<screen_size[0]; i++) {
		hud_render_dest.x = i;
		SDL_BlitSurface(border,&hud_render_src,screen,&hud_render_dest);
	}

	s_reset_render();
	hud_render_src.y = 229;
	hud_render_src.w = 200;
	hud_render_src.h = 1;
	for (i=230; i<screen_size[1]; i++) {
		hud_render_dest.y = i;
		SDL_BlitSurface(border,&hud_render_src,screen,&hud_render_dest);
	}

	s_reset_render();
	SDL_BlitSurface(border,NULL,screen,NULL);

	hud_print(-1,20,hud_title_font,ORANGE,"Voyager: Journey Home");

	s_reset_render();
	hud_render_dest.y = screen_size[1]-200;
	hud_render_src.w = 1;
	hud_render_src.h = 230;
	for (i=200; i<screen_size[0]; i++) {
		hud_render_dest.x = i;
		SDL_BlitSurface(border,&hud_render_src,screen,&hud_render_dest);
	}

	s_reset_render();
	hud_render_dest.x = screen_size[0]-200;
	hud_render_src.w = 200;
	hud_render_src.h = 1;
	for (i=230; i<screen_size[1]; i++) {
		hud_render_dest.y = i;
		SDL_BlitSurface(button,&hud_render_src,screen,&hud_render_dest);
	}
	hud_render_src.w = 200;
	hud_render_src.h = 64;
	hud_render_dest.y = 260;
	SDL_BlitSurface(button,&hud_render_src,screen,&hud_render_dest);
	hud_print(hud_render_dest.x+120,hud_render_dest.y+35,hud_font,BLACK,"Play");
	hud_render_dest.y += 70;
	SDL_BlitSurface(button,&hud_render_src,screen,&hud_render_dest);
	hud_print(hud_render_dest.x+98,hud_render_dest.y+35,hud_font,BLACK,"Options");
	hud_render_dest.y += 70;
	SDL_BlitSurface(button,&hud_render_src,screen,&hud_render_dest);
	hud_print(hud_render_dest.x+120,hud_render_dest.y+35,hud_font,BLACK,"Help");
	hud_render_dest.y += 70;
	SDL_BlitSurface(button,&hud_render_src,screen,&hud_render_dest);
	hud_print(hud_render_dest.x+120,hud_render_dest.y+35,hud_font,BLACK,"Quit");

	if (menu_state) {
		hud = SDL_CreateRGBSurface(SDL_SWSURFACE,(screen_size[0]-400),(screen_size[1]-430),32,0,0,0,0);
		s_reset_render();
		hud_render_dest.x = 200;
		hud_render_dest.y = 230;
		SDL_BlitSurface(hud,NULL,screen,&hud_render_dest);
		SDL_FreeSurface(hud);

		s_reset_render();
		hud_render_src.w = 200;
		hud_render_src.h = 64;
		hud_render_dest.x = 500;
		hud_render_dest.y = 400;
		SDL_BlitSurface(button,&hud_render_src,screen,&hud_render_dest);

		if (menu_state == 1) {
			hud_print(230,250,NULL,ORANGE,"Resolution: %s",resolutions[c_res[1]].name);
			hud_print(hud_render_dest.x+50,hud_render_dest.y+35,NULL,BLACK,"Save/Apply");
			int y;
			for (i=0; i<CTRL_COUNT; i++) {
				y = 280+(25*i);
				hud_print(230,y,NULL,ORANGE,"%s: ",control_map[i]);
				if (set_event[0] && set_event[1] == i) {
					hud_print(350,y,NULL,ORANGE,"-- %s --",sym_to_string(i));
				}else{
					hud_print(350,y,NULL,ORANGE,"   %s",sym_to_string(i));
				}
			}
		}else if (menu_state == 2) {
			hud_print(230,250,NULL,ORANGE,"Umm, we left the help menu in the Alpha Quadrant");
			hud_print(hud_render_dest.x+140,hud_render_dest.y+35,NULL,BLACK,"Ok");
		}
	}
}

void hud_game()
{
	int i;
	s_reset_render();
	hud_render_src.x = 199;
	hud_render_src.w = 1;
	hud_render_src.h = 50;
	hud_render_dest.y = screen_size[1]-50;
	for (i=0; i<screen_size[0]; i++) {
		hud_render_dest.x = i;
		SDL_BlitSurface(border,&hud_render_src,screen,&hud_render_dest);
	}

	hud_print(-1,screen_size[1]-49,game_font,ORANGE,"Structural Integrity: %3d%%   Sheilds: %3d%%",ships[VOYAGER].structural_integrity,ships[VOYAGER].sheild_state[1]);
	hud_print(-1,screen_size[1]-34,game_font,ORANGE,"Torpedos: %2d     Lives: %d",ships[VOYAGER].weapons[4],lives);

	i = (screen_size[1]+140)/2;
	if (ships[VOYAGER].drives[3]) {
		hud_print(i+25,screen_size[1]-19,game_font,GREEN,"TransWarp");
		hud_print(i+95,screen_size[1]-19,game_font,ORANGE,"/");
	}
	hud_print(i+100,screen_size[1]-19,game_font,ships[VOYAGER].drives[2] ? GREEN : RED,"Warp");
	hud_print(i+135,screen_size[1]-19,game_font,ORANGE,"/");
	hud_print(i+143,screen_size[1]-19,game_font,ships[VOYAGER].drives[1] ? GREEN : RED,"Impulse");
	hud_print(i+200,screen_size[1]-19,game_font,ORANGE,"Weapons:");
	hud_print(i+260,screen_size[1]-19,game_font,ships[VOYAGER].weapons[0] ? GREEN : RED,"1");
	hud_print(i+265,screen_size[1]-19,game_font,ships[VOYAGER].weapons[1] ? GREEN : RED,"2");
	hud_print(i+272,screen_size[1]-19,game_font,ships[VOYAGER].weapons[2] ? GREEN : RED,"3");
	hud_print(i+279,screen_size[1]-19,game_font,ships[VOYAGER].weapons[3] ? GREEN : RED,"4");
}

void hud_print(int x, int y, TTF_Font *font, int c, char* fmt, ...)
{
	char tmp[512];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(tmp, 512, fmt, ap);
	va_end(ap);

	if (!font)
		font = hud_font;

	hud = TTF_RenderText_Blended(font, tmp, hud_color[c]);

	hud_font_dest.x = 0;
	hud_font_dest.y = 0;
	hud_font_dest.w = 0;
	hud_font_dest.h = 0;

	if (x > -1 && y > -1) {
		hud_font_dest.x = x;
		hud_font_dest.y = y;
	}else{
		if (x > -1) {
			hud_font_dest.x = x;
		}else{
			hud_font_dest.x = (screen_size[0]-hud->w)/2;
		}
		if (y > -1) {
			hud_font_dest.y = y;
		}else{
			hud_font_dest.y = (screen_size[1]-hud->h)/2;
		}
	}
		SDL_BlitSurface(hud,NULL,screen,&hud_font_dest);
	SDL_FreeSurface(hud);
}
