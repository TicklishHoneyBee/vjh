/************************************************************************
* main.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* main.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* main.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

int frame = 0;
int ticks = 0;

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	vi = SDL_GetVideoInfo();
	screen_size[0] = DEFAULT_SCREEN_WIDTH;
	screen_size[1] = DEFAULT_SCREEN_HEIGHT;
	screen_size[2] = 0;

	sound_init();

	config_load();

	game_state = 0;

	if (vi->current_w < screen_size[0])
		screen_size[0] = vi->current_w;
	if (vi->current_h < screen_size[1])
		screen_size[1] = vi->current_h;

	int opts = SDL_SWSURFACE | SDL_DOUBLEBUF;
	if (screen_size[2])
		opts |= SDL_FULLSCREEN;
	screen = SDL_SetVideoMode(screen_size[0],screen_size[1],32,opts);
	SDL_WM_SetCaption("Voyager: Journey Home", NULL);

	sound_play(SND_SIGNATURE);

	explosion = IMG_Load("images/explosion.png");
	pickup = IMG_Load("images/pickup.png");

	ships_init();
	bg_init();
	hud_init();

	while (game_state > -1) {
		if (!frame)
			ticks = SDL_GetTicks();

		events_main();

		bg_main();

		ships_main();

		game_main();

		weapon_main();

		collision_detect();

		hud_main();

		SDL_Flip(screen);

		frame++;

		if ((SDL_GetTicks()-ticks) < (1000/30)) {
			SDL_Delay((1000/30)-(SDL_GetTicks()-ticks));
		}
	}

	SDL_FreeSurface(bg);
	SDL_FreeSurface(explosion);
	SDL_FreeSurface(pickup);

	hud_exit();
	ships_exit();
	config_save();
	sound_exit();

	TTF_Quit();
	SDL_Quit();

	return 0;
}
