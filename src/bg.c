/************************************************************************
* bg.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* bg.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* bg.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

SDL_Rect bg_pos = {0,0,0,0};
SDL_Rect bg_src = {0,0,0,0};

void bg_init()
{
	bg = IMG_Load("images/starfield.png");
	bg_src.w = bg_pos.w = bg->w;
	bg_src.h = bg_pos.h = bg->h;
}

void bg_main()
{
	if (game_state != 2) {
		bg_src.x += ships[VOYAGER].speed[0];
		if (bg_src.x > 2048)
			bg_src.x = 0;

		bg_pos.x = bg_src.x-2048;
	}

	SDL_BlitSurface(bg,&bg_src,screen,NULL);
	SDL_BlitSurface(bg,&bg_pos,screen,NULL);
}
