/************************************************************************
* event_stubs.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* event_stubs.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* event_stubs.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

void stub_up()
{
	key[CTRL_UP] = 1;
}

void stub_down()
{
	key[CTRL_DOWN] = 1;
}

void stub_left()
{
	key[CTRL_LEFT] = 1;
}

void stub_right()
{
	key[CTRL_RIGHT] = 1;
}

void stub_up_release()
{
	key[CTRL_UP] = 0;
}

void stub_down_release()
{
	key[CTRL_DOWN] = 0;
}

void stub_left_release()
{
	key[CTRL_LEFT] = 0;
}

void stub_right_release()
{
	key[CTRL_RIGHT] = 0;
}

void stub_thruster()
{
	if (ships[VOYAGER].drives[0])
		ships[VOYAGER].speed[1] = ships[VOYAGER].drives[0];
}

void stub_impulse()
{
	if (ships[VOYAGER].drives[1])
		ships[VOYAGER].speed[1] = ships[VOYAGER].drives[1];
}

void stub_warp()
{
	if (ships[VOYAGER].drives[2])
		ships[VOYAGER].speed[1] = ships[VOYAGER].drives[2];
}

void stub_transwarp()
{
	if (ships[VOYAGER].drives[3]) {
		transwarp = 1;
		ships[VOYAGER].speed[1] = 90;
		ships[VOYAGER].drives[3]--;
		pickup_add(screen_size[0]/2,screen_size[1]/2,PU_TRANSWARP,level);
	}
}

void stub_fire_phaser()
{
	if (game_state == 1)
		phaser(VOYAGER,key[CTRL_X],key[CTRL_Y]);
}

void stub_fire_torpedo()
{
	if (game_state == 1)
		torpedo(VOYAGER,key[CTRL_X],key[CTRL_Y]);
}

void stub_pause()
{
	sound_play(SND_BEEP);
	game_state = (game_state == 2) ? 1 : 2;
}

void stub_quit()
{
	if (!game_state) {
		menu_state = 0;
	}else{
		game_exit();
	}
}
