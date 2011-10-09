/************************************************************************
* sound.c
* Copyright (C) Lisa Milne 2009 <lisa@ltmnet.com>
*
* sound.c is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* sound.c is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>
************************************************************************/

#include "voyager.h"

Mix_Chunk *sounds[5] = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

void sound_init()
{
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
	int audio_buffers = 4096;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
		return;

	sounds[SND_BEEP] = Mix_LoadWAV("sounds/beep.wav");
	sounds[SND_EXPLOSION] = Mix_LoadWAV("sounds/explosion.wav");
	sounds[SND_PHASER] = Mix_LoadWAV("sounds/phaser.wav");
	sounds[SND_SIGNATURE] = Mix_LoadWAV("sounds/signature.wav");
	sounds[SND_TORPEDO] = Mix_LoadWAV("sounds/torpedo.wav");
}

void sound_exit()
{
	Mix_CloseAudio();
}

void sound_play(int snd)
{
	if (snd > 4 || snd < 0 || !sounds[snd])
		return;

	Mix_PlayChannel(-1, sounds[snd], 0);
}
