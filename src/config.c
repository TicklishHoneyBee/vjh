/************************************************************************
* config.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* config.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* config.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

static int s_key_to_type(char* k)
{
	if (!strncmp(k,"mouse",5)) {
		return CTRL_MOUSE;
	}else{
		return CTRL_KEYBOARD;
	}
}

static int s_key_to_int(char* k)
{
	if (!strcmp(k,"mouse 1")) {
		return SDL_BUTTON_LEFT;
	}else if (!strcmp(k,"mouse 2")) {
		return SDL_BUTTON_RIGHT;
	}else if (!k[0]) {
		return 0;
	}else if (!k[1] && isgraph(k[0])) {
		return k[0];
	}else if (!strcmp(k,"esc")) {
		return SDLK_ESCAPE;
	}else if (!strcmp(k,"spc")) {
		return SDLK_SPACE;
	}else if (!strcmp(k,"kp-0")) {
		return SDLK_KP0;
	}else if (!strcmp(k,"kp-1")) {
		return SDLK_KP1;
	}else if (!strcmp(k,"kp-2")) {
		return SDLK_KP2;
	}else if (!strcmp(k,"kp-3")) {
		return SDLK_KP3;
	}else if (!strcmp(k,"kp-4")) {
		return SDLK_KP4;
	}else if (!strcmp(k,"kp-5")) {
		return SDLK_KP5;
	}else if (!strcmp(k,"kp-6")) {
		return SDLK_KP6;
	}else if (!strcmp(k,"kp-7")) {
		return SDLK_KP7;
	}else if (!strcmp(k,"kp-8")) {
		return SDLK_KP8;
	}else if (!strcmp(k,"kp-9")) {
		return SDLK_KP9;
	}else if (!strcmp(k,"up")) {
		return SDLK_UP;
	}else if (!strcmp(k,"down")) {
		return SDLK_DOWN;
	}else if (!strcmp(k,"right")) {
		return SDLK_RIGHT;
	}else if (!strcmp(k,"left")) {
		return SDLK_LEFT;
	}else{
		return 0;
	}
}

static void s_config_parse(char* name, char* value)
{

	int i = strlen(name)-1;
	while (isspace(name[i]) && i) {
		name[i] = 0;
		i--;
	}
	i = strlen(value)-1;
	while (isspace(value[i]) && i) {
		value[i] = 0;
		i--;
	}
	if (!strcmp(name,"r_width")) {
		i = atoi(value);
		if (i)
			screen_size[0] = i;
	}else if (!strcmp(name,"r_height")) {
		i = atoi(value);
		if (i)
			screen_size[1] = i;
	}else if (!strcmp(name,"r_fullscreen")) {
		i = atoi(value);
		if (i)
			screen_size[2] = 1;
	}else if (!strcmp(name,"c_up")) {
		controls[CTRL_UP].sym = s_key_to_int(value);
		controls[CTRL_UP].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_down")) {
		controls[CTRL_DOWN].sym = s_key_to_int(value);
		controls[CTRL_DOWN].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_left")) {
		controls[CTRL_LEFT].sym = s_key_to_int(value);
		controls[CTRL_LEFT].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_right")) {
		controls[CTRL_RIGHT].sym = s_key_to_int(value);
		controls[CTRL_RIGHT].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_thruster")) {
		controls[CTRL_THRUSTERS].sym = s_key_to_int(value);
		controls[CTRL_THRUSTERS].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_impulse")) {
		controls[CTRL_IMPULSE].sym = s_key_to_int(value);
		controls[CTRL_IMPULSE].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_warp")) {
		controls[CTRL_WARP].sym = s_key_to_int(value);
		controls[CTRL_WARP].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_transwarp")) {
		controls[CTRL_TRANSWARP].sym = s_key_to_int(value);
		controls[CTRL_TRANSWARP].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_phaser")) {
		controls[CTRL_PHASER].sym = s_key_to_int(value);
		controls[CTRL_PHASER].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_torpedo")) {
		controls[CTRL_TORPEDO].sym = s_key_to_int(value);
		controls[CTRL_TORPEDO].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_pause")) {
		controls[CTRL_PAUSE].sym = s_key_to_int(value);
		controls[CTRL_PAUSE].type = s_key_to_type(value);
	}else if (!strcmp(name,"c_quit")) {
		controls[CTRL_QUIT].sym = s_key_to_int(value);
		controls[CTRL_QUIT].type = s_key_to_type(value);
	}else if (!strcmp(name,"s_vol")) {
		i = atoi(value);
		if (i > -1 && i < MIX_MAX_VOLUME)
			Mix_Volume(-1,i);
	}
}

void config_load()
{
	char* home = getenv("HOME");
	char filename[strlen(home)+15];
	sprintf(filename,"%s/.voyager.cfg",home);

	FILE *f = fopen(filename,"r");
	if (!f)
		return;

	char name[255];
	char value[255];
	int o = 0;
	int v = 0;
	int cmt = 0;
	char c;
	while ((c = fgetc(f)) != EOF) {
		if (!o && !v && c == '#') {
			cmt = 1;
		}else if (c == '\n') {
			if (v && o && !cmt) {
				value[o] = 0;
				s_config_parse(name,value);
			}
			v = 0;
			o = 0;
			cmt = 0;
		}else if (cmt) {
			continue;
		}else if (!v && c == '=') {
			if (!o)
				cmt = 1;
			name[o] = 0;
			v = 1;
			o = 0;
		}else if (o > 254 || (!o && c == ' ')) {
			continue;
		}else if (v) {
			value[o++] = c;
		}else{
			name[o++] = c;
		}
	}

	fclose(f);
}

void config_save()
{
	char* home = getenv("HOME");
	char filename[strlen(home)+15];
	sprintf(filename,"%s/.voyager.cfg",home);

	FILE *f = fopen(filename,"w+");
	if (!f)
		return;

	fprintf(f,"r_width = %d\nr_height = %d\nr_fullscreen = 0\n",screen_size[0],screen_size[1]);

	char* opts[CTRL_COUNT] = {"c_up","c_down","c_left","c_right","c_quit","c_pause","c_thruster","c_impulse","c_warp","c_transwarp","c_phaser","c_torpedo"};

	int i;
	for (i=0; i<CTRL_COUNT; i++) {
		fprintf(f,"%s = %s\n",opts[i],sym_to_string(i));
	}

	fprintf(f,"s_vol = %d\n",Mix_Volume(-1,-1));

	fclose(f);
}
