#ifndef _VOYAGER_H
#define _VOYAGER_H

#include "config.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

typedef struct ship_s {
	SDL_Surface *surface;
	SDL_Surface *sheilds;
	int sheild_state[2];
	int structural_integrity;
	SDL_Rect pos;
	int drives[4];
	int speed[4];
	int onscreen;
	int y_mot;
	int phaser[4][2];
	int weapons[6];
	SDL_Color phaser_color;
} ship_t;

SDL_Surface *screen;
int screen_size[3];
SDL_Surface *bg;
SDL_Surface *explosion;
SDL_Surface *pickup;

enum {
	PU_NONE,
	PU_PHASER,
	PU_TORPEDO,
	PU_SHEILDS,
	PU_PARTS,
	PU_LIFE,
	PU_TRANSWARP
};

enum {
	VOYAGER,
	KAZON,
	HIROGEN,
	BORG_CUBE,
	S8472,
	BORG_SPHERE,
	SHIP_COUNT
};

enum {
	SND_BEEP,
	SND_EXPLOSION,
	SND_PHASER,
	SND_SIGNATURE,
	SND_TORPEDO
};

enum {
	ORANGE,
	BLACK,
	GREEN,
	RED
};

enum {
	WEAPON_INACTIVE,
	WEAPON_PHASER,
	WEAPON_TORPEDO
};
enum {
	CTRL_UP,
	CTRL_DOWN,
	CTRL_LEFT,
	CTRL_RIGHT,
	CTRL_QUIT,
	CTRL_X = 4,
	CTRL_PAUSE,
	CTRL_Y = 5,
	CTRL_THRUSTERS,
	CTRL_IMPULSE,
	CTRL_WARP,
	CTRL_TRANSWARP,
	CTRL_PHASER,
	CTRL_TORPEDO,
	CTRL_COUNT
};

enum {
	CTRL_MOUSE,
	CTRL_KEYBOARD
};

typedef struct weapon_s {
	int type;
	int tx;
	int ty;
	int ox;
	int oy;
	int ship;
	int bank;
	int pos;
	int ticks;
} weapon_t;

typedef struct control_s {
	int type;
	int sym;
	void (*handler)(void);
	void (*handler_r)(void);
} control_t;

const SDL_VideoInfo *vi;

/* defined in bg.c */
void bg_init(void);
void bg_main(void);

/* defined in events.c */
extern int key[6];
extern int set_event[2];
extern control_t controls[CTRL_COUNT];
char* control_map[CTRL_COUNT];
void events_main(void);
void events_mouse(void);
void events_key_down(void);
void events_key_up(void);

/* defined in event_stubs.c */
void stub_up(void);
void stub_down(void);
void stub_left(void);
void stub_right(void);
void stub_up_release(void);
void stub_down_release(void);
void stub_left_release(void);
void stub_right_release(void);
void stub_thruster(void);
void stub_impulse(void);
void stub_warp(void);
void stub_transwarp(void);
void stub_fire_phaser(void);
void stub_fire_torpedo(void);
void stub_pause(void);
void stub_quit(void);

/* defined in ships.c */
extern ship_t *ships;
extern int ships_cnt;
void ships_init(void);
void ships_exit(void);
void ships_main(void);
void ship_move(int id, int xinc, int yinc);
int ship_clone(int type);

/* defined in hud.c */
extern int menu_state;
extern int c_res[2];
void hud_init(void);
void hud_exit(void);
void hud_main(void);
void hud_menu(void);
void hud_menu_exit(void);
void hud_game(void);
void hud_print(int x, int y, TTF_Font *font, int c, char* fmt, ...);
char* sym_to_string(int s);

/* defined in weapon.c */
extern weapon_t shots_active[50];
int weapon_get_bank(int ship, int x, int y);
void phaser(int ship, int x, int y);
void draw_phaser(int id);
void torpedo(int ship, int x, int y);
void draw_torpedo(int id);
void weapon_main(void);

/* defined in collision.c */
void pickup_add(int x, int y, int type, int value);
void movement(int id, int x, int y);
void collision_detect(void);
void collision(int sid1, int sid2, int wid1, int wid2, int x, int y);

/* defined in sound.c */
void sound_init(void);
void sound_exit(void);
void sound_play(int snd);

/* defined in game.c */
extern int lives;
extern int level;
extern int boss_mode;
extern int boss_ships[20];
extern time_t s_time;
extern time_t w_time;
extern int active_ship;
extern int game_state;
extern int eog[4];
void game_start(void);
void game_exit(void);
void game_main(void);
int rand_range(int low, int high);

/* defined in config.c */
void config_load(void);
void config_save(void);

#endif
