/*
 *  Copyright (C) 2006 Stas Sergeev <stsp@users.sourceforge.net>
 *
 * The below copyright strings have to be distributed unchanged together
 * with this file. This prefix can not be modified or separated.
 */

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * Purpose: WAV file sound output plugin.
 *
 * Author: Stas Sergeev.
 */

#include "emu.h"
#include "init.h"
#include "sound/sound.h"
#include "sound/sndpcm.h"
#include <stdio.h>
#include <sndfile.h>
#include <pthread.h>

#define ENABLED 0

static const char *wavsnd_name = "Sound Output: WAV file writer";
static SNDFILE *wav;
struct player_params params;
static int started;
static int handle;

static int wavsnd_open(void)
{
#if ENABLED
    SF_INFO info;
#endif
    params.rate = 44100;
    params.format = PCM_FORMAT_S16_LE;
    params.channels = 2;
#if ENABLED
    info.samplerate = params.rate;
    info.channels = params.channels;
    info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    wav = sf_open("/tmp/a.wav", SFM_WRITE, &info);
    return !!wav;
#else
    return 0;
#endif
}

static void wavsnd_close(void)
{
    sf_close(wav);
}

static size_t wavsnd_write(void *data, size_t size)
{
    return sf_write_raw(wav, data, size);
}

static void wavsnd_start(void)
{
    started = 1;
}

static void wavsnd_stop(void)
{
    started = 0;
}

static void wavsnd_timer(double dtime)
{
    #define BUF_SIZE 4096
    char buf[BUF_SIZE];
    ssize_t size;
    if (!started)
	return;
    size = pcm_frag_size(dtime, &params);
    if (size < BUF_SIZE)
	return;
    if (size > BUF_SIZE)
	size = BUF_SIZE;
    size = pcm_data_get(buf, size, &params, handle);
    if (size > 0)
	wavsnd_write(buf, size);
}

CONSTRUCTOR(static void wavsnd_init(void))
{
    struct clocked_player player = {};
    player.name = wavsnd_name;
    player.open = wavsnd_open;
    player.close = wavsnd_close;
    player.start = wavsnd_start;
    player.stop = wavsnd_stop;
    player.timer = wavsnd_timer;
    handle = pcm_register_clocked_player(player);
}
