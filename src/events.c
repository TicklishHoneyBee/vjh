/************************************************************************
* events.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* events.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* events.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

SDL_Event current_event;

int set_event[2] = {0,0};

char* control_map[CTRL_COUNT] = {
	"up",
	"down",
	"left",
	"right",
	"quit",
	"pause",
	"thruster",
	"impulse",
	"warp",
	"trans-warp",
	"phaser",
	"torpedo"
};

control_t control_defaults[CTRL_COUNT] = {
	{CTRL_KEYBOARD,SDLK_w,stub_up,stub_up_release},
	{CTRL_KEYBOARD,SDLK_s,stub_down,stub_down_release},
	{CTRL_KEYBOARD,SDLK_a,stub_left,stub_left_release},
	{CTRL_KEYBOARD,SDLK_d,stub_right,stub_right_release},
	{CTRL_KEYBOARD,SDLK_ESCAPE,stub_quit,NULL},
	{CTRL_KEYBOARD,SDLK_p,stub_pause,NULL},
	{CTRL_KEYBOARD,SDLK_1,stub_thruster,NULL},
	{CTRL_KEYBOARD,SDLK_2,stub_impulse,NULL},
	{CTRL_KEYBOARD,SDLK_3,stub_warp,NULL},
	{CTRL_KEYBOARD,SDLK_4,stub_transwarp,NULL},
	{CTRL_MOUSE,SDL_BUTTON_LEFT,stub_fire_phaser,NULL},
	{CTRL_MOUSE,SDL_BUTTON_RIGHT,stub_fire_torpedo,NULL}
};

control_t controls[CTRL_COUNT] = {
	{CTRL_KEYBOARD,SDLK_w,stub_up,stub_up_release},
	{CTRL_KEYBOARD,SDLK_s,stub_down,stub_down_release},
	{CTRL_KEYBOARD,SDLK_a,stub_left,stub_left_release},
	{CTRL_KEYBOARD,SDLK_d,stub_right,stub_right_release},
	{CTRL_KEYBOARD,SDLK_ESCAPE,stub_quit,NULL},
	{CTRL_KEYBOARD,SDLK_p,stub_pause,NULL},
	{CTRL_KEYBOARD,SDLK_1,stub_thruster,NULL},
	{CTRL_KEYBOARD,SDLK_2,stub_impulse,NULL},
	{CTRL_KEYBOARD,SDLK_3,stub_warp,NULL},
	{CTRL_KEYBOARD,SDLK_4,stub_transwarp,NULL},
	{CTRL_MOUSE,SDL_BUTTON_LEFT,stub_fire_phaser,NULL},
	{CTRL_MOUSE,SDL_BUTTON_RIGHT,stub_fire_torpedo,NULL}
};

void s_event_set_control(int type, int sym)
{
	int i;
	for (i=0; i<CTRL_COUNT; i++) {
		if (i == set_event[1]) {
			controls[i].type = type;
			controls[i].sym = sym;
		}else if (controls[i].type == type && controls[i].sym == sym) {
			controls[i].type = CTRL_KEYBOARD;
			controls[i].sym = SDLK_UNKNOWN;
		}
	}

	set_event[0] = 0;
}

int key[6] = {0,0,0,0,0,0};

void events_main()
{
	while (SDL_PollEvent(&current_event)) {
		if (set_event[0] && (
			current_event.type == SDL_MOUSEBUTTONDOWN
			|| current_event.type == SDL_KEYDOWN
			|| current_event.type == SDL_KEYUP
		)) {
			if (current_event.type == SDL_MOUSEBUTTONDOWN) {
				s_event_set_control(CTRL_MOUSE,current_event.button.button);
			}else if (current_event.type == SDL_KEYDOWN) {
				s_event_set_control(CTRL_KEYBOARD,current_event.key.keysym.sym);
			}
		}else if (current_event.type == SDL_MOUSEMOTION) {
			key[CTRL_X] = current_event.motion.x;
			key[CTRL_Y] = current_event.motion.y;
		}else if (current_event.type == SDL_MOUSEBUTTONDOWN) {
			events_mouse();
		}else if (current_event.type == SDL_QUIT) {
			game_state = -1;
		}else if (current_event.type == SDL_KEYDOWN) {
			events_key_down();
		}else if (current_event.type == SDL_KEYUP) {
			events_key_up();
		}
	}
}


void events_mouse()
{
	if (!game_state) {
		if (current_event.motion.x > screen_size[0]-180 && current_event.motion.x < screen_size[0]-20) {
			if (current_event.motion.y > 280 && current_event.motion.y < 340) {
				sound_play(SND_BEEP);
				game_start();
			}else if (current_event.motion.y > 350 && current_event.motion.y < 410) {
				sound_play(SND_BEEP);
				menu_state = 1;
			}else if (current_event.motion.y > 420 && current_event.motion.y < 480) {
				sound_play(SND_BEEP);
				menu_state = 2;
			}else if (current_event.motion.y > 490 && current_event.motion.y < 550) {
				sound_play(SND_BEEP);
				game_state = -1;
			}
		}else if (menu_state) {
			if (menu_state == 1 && (current_event.motion.x > 250 && current_event.motion.x < (500))) {
				if (current_event.motion.y > 240 && current_event.motion.y < 280) {
					sound_play(SND_BEEP);
					c_res[1]++;
					if (c_res[1] > 5)
						c_res[1] = 0;
				}else{
					int y;
					int i;
					for (i=0; i<CTRL_COUNT; i++) {
						y = 280+(25*i);
						if (current_event.motion.y > y && current_event.motion.y < y+24) {
							sound_play(SND_BEEP);
							set_event[0] = 1;
							set_event[1] = i;
							break;
						}
					}
				}
			}else if (
				(current_event.motion.x > 500 && current_event.motion.x < 700)
				&& (current_event.motion.y > 400 && current_event.motion.y < 460)
			) {
				sound_play(SND_BEEP);
				hud_menu_exit();
			}
		}
		return;
	}
	int i;
	for (i=0; i<CTRL_COUNT; i++) {
		if (controls[i].type == CTRL_MOUSE && controls[i].sym == current_event.button.button) {
			if (controls[i].handler) {
				controls[i].handler();
			}
			break;
		}
	}
}

void events_key_down()
{
	int i;
	for (i=0; i<CTRL_COUNT; i++) {
		if (controls[i].type == CTRL_KEYBOARD && controls[i].sym == current_event.key.keysym.sym) {
			if (controls[i].handler) {
				controls[i].handler();
			}
			break;
		}
	}
}

void events_key_up()
{
	int i;
	for (i=0; i<CTRL_COUNT; i++) {
		if (controls[i].type == CTRL_KEYBOARD && controls[i].sym == current_event.key.keysym.sym) {
			if (controls[i].handler_r) {
				controls[i].handler_r();
			}
			break;
		}
	}
}
