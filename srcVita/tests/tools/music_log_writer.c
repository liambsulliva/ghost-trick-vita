#include <stdio.h>
#include <stdlib.h>
#include "audio_port.h"
#include "audio_thread.h"
#include "clock.h"
#include "fakes/fake_audio.h"
#include "fakes/fake_platform.h"
#include "file.h"
#include "fixtures/sine_mono_ogg.h"
#include "fixtures/sine_stereo_ogg.h"
#include "music_engine.h"
#include "run_log.h"
#include "sound_engine.h"

#define MUSIC_GRAINS_PLAYED 220
#define SOUND_GRAINS_PER_MUSIC_GRAIN 8
#define FIRST_SOUND_AT_GRAIN 20
#define SECOND_SOUND_AT_GRAIN 90
#define CAPTURE_AT_GRAIN 60
#define STARVE_AT_GRAIN 150
#define GRAINS_PER_STATS_LINE 11
#define STOP_STEPS_MAX 8

static unsigned long long sound_clock_us;

static void pump_sound_on_its_own_clock(int steps)
{
    unsigned long long music_clock_us;

    music_clock_us = clock_now_us();
    fake_clock_set_us(sound_clock_us);
    fake_audio_thread_pump(AUDIO_THREAD_SOUND, steps);
    sound_clock_us = clock_now_us();
    fake_clock_set_us(music_clock_us);
}

static void write_file(const char *path, const unsigned char *data, unsigned int size)
{
    FILE *output;

    output = fopen(path, "wb");
    if (output == NULL || fwrite(data, 1, size, output) != size) {
        fprintf(stderr, "cannot write %s\n", path);
        exit(2);
    }
    fclose(output);
}

static void play_the_scene(void)
{
    int grain;

    for (grain = 0; grain < MUSIC_GRAINS_PLAYED; grain++) {
        if (grain == FIRST_SOUND_AT_GRAIN) {
            sound_request_play("ping");
        }
        if (grain == SECOND_SOUND_AT_GRAIN) {
            sound_request_play("pong");
        }
        if (grain == CAPTURE_AT_GRAIN) {
            run_log_write("capture name=stall result=0 took_us=447409");
        }
        if (grain == STARVE_AT_GRAIN) {
            run_log_write("command music_starve ms=300");
            music_request_starve(300);
        }
        fake_audio_thread_pump(AUDIO_THREAD_MUSIC, 1);
        pump_sound_on_its_own_clock(SOUND_GRAINS_PER_MUSIC_GRAIN);
        if (grain % GRAINS_PER_STATS_LINE == 0) {
            run_log_write("stats fps=59.9 frames=60 slow=0 frame_us_min=16000 frame_us_avg=16680 frame_us_max=17500 update_us=40 draw_us=400 present_us=16000");
        }
    }
}

int main(int argc, char **argv)
{
    const unsigned char *log_bytes;
    unsigned int log_size;

    if (argc != 3) {
        fprintf(stderr, "usage: music_log_writer <log path> <ogg path>\n");
        return 2;
    }
    write_file(argv[2], sine_stereo_ogg, sizeof(sine_stereo_ogg));
    fake_files_add("music/song.ogg", sine_stereo_ogg, sizeof(sine_stereo_ogg));
    fake_files_add("music/song.loop", (const unsigned char *)"11025 33074\n", 12);
    fake_files_add("sounds/ping.ogg", sine_mono_ogg, sizeof(sine_mono_ogg));
    fake_files_add("sounds/pong.ogg", sine_mono_ogg, sizeof(sine_mono_ogg));
    fake_clock_set_us(1000);
    sound_clock_us = 1000;
    run_log_open("run.log");
    music_engine_init("music");
    sound_engine_init("sounds");
    run_log_write("scene started name=opening steps=6");
    music_request_prepare("song");
    sound_request_prepare("ping");
    sound_request_prepare("pong");
    fake_audio_thread_pump(AUDIO_THREAD_MUSIC, 1);
    pump_sound_on_its_own_clock(3);
    run_log_write("scene ready name=opening preload_us=1000 texture_bytes=0 missing=0");
    music_request_play("song", 1);
    play_the_scene();
    music_request_stop(4);
    fake_audio_thread_pump(AUDIO_THREAD_MUSIC, STOP_STEPS_MAX);
    run_log_write("scene finished name=opening");
    sound_request_stop();
    pump_sound_on_its_own_clock(1);
    music_shutdown();
    sound_shutdown();
    run_log_write("loop ended frames=1000");
    run_log_write("exit clean");
    log_bytes = fake_files_contents("run.log", &log_size);
    write_file(argv[1], log_bytes, log_size);
    fake_files_reset();
    fake_audio_reset();
    return 0;
}
