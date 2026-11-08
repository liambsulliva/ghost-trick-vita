#include "animation_library.h"
#include "audio_port.h"
#include "audio_thread.h"
#include "fakes/fake_audio.h"
#include "fakes/fake_platform.h"
#include "fakes/fake_reader.h"
#include "fakes/fake_renderer.h"
#include "fixtures/fixture_facts.h"
#include "fixtures/sine_mono_ogg.h"
#include "fixtures/sine_stereo_ogg.h"
#include "music_engine.h"
#include "run_log.h"
#include "scene_player.h"
#include "sound_engine.h"
#include "test_support.h"
#include "texture_cache.h"

#define FRAME_US 16667
#define GIVE_UP_US 10000001ULL

static const char *log_text(void)
{
    unsigned int size;

    return (const char *)fake_files_contents("run.log", &size);
}

static int count_in_log(const char *wanted)
{
    const char *found;
    int count;

    count = 0;
    found = strstr(log_text(), wanted);
    while (found != NULL) {
        count = count + 1;
        found = strstr(found + 1, wanted);
    }
    return count;
}

static void add_scene(const char *text)
{
    fake_files_add("assets/scenes/opening.scene", (const unsigned char *)text, (unsigned int)strlen(text));
}

static void add_song(const char *path)
{
    fake_files_add(path, sine_stereo_ogg, sizeof(sine_stereo_ogg));
}

static void add_ping(void)
{
    fake_files_add("assets/sounds/ping.ogg", sine_mono_ogg, sizeof(sine_mono_ogg));
}

static void start_engines(void)
{
    texture_cache_init("assets/textures", TEXTURE_CACHE_DEFAULT_BUDGET_BYTES);
    animation_library_init("assets/animations");
    scene_player_init("assets/scenes");
    music_engine_init("assets/music");
    sound_engine_init("assets/sounds");
}

static void set_up(void)
{
    fake_files_reset();
    fake_memory_reset();
    fake_audio_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_clock_set_us(0);
    run_log_open("run.log");
    start_engines();
}

static void pump_workers(int steps)
{
    fake_audio_thread_pump(AUDIO_THREAD_MUSIC, steps);
    fake_audio_thread_pump(AUDIO_THREAD_SOUND, steps);
}

static void run_frames(int frame_count)
{
    int frame;

    for (frame = 0; frame < frame_count; frame++) {
        fake_clock_advance_us(FRAME_US);
        scene_player_update(FRAME_US);
    }
}

static void get_scene_running(void)
{
    int round;

    for (round = 0; round < 10 && scene_player_status() == SCENE_PLAYER_PRELOADING; round++) {
        pump_workers(1);
        run_frames(1);
    }
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
}

static void finish(void)
{
    scene_player_shutdown();
    music_shutdown();
    sound_shutdown();
    animation_library_clear();
    texture_cache_clear();
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    CHECK_INT(fake_memory_blocks_in_use(), 0);
}

static void test_a_scene_without_audio_does_not_wait_for_the_workers(void)
{
    set_up();
    add_scene("scene opening\nwait 60\n");
    CHECK(scene_player_play("opening"));
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    finish();
}

static void test_preload_waits_for_the_music(void)
{
    set_up();
    add_song("assets/music/song.ogg");
    add_scene("scene opening\nmusic song 1\nwait 60\n");
    CHECK(scene_player_play("opening"));
    run_frames(30);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_PRELOADING);
    CHECK(strstr(log_text(), "scene ready") == NULL);
    fake_audio_thread_pump(AUDIO_THREAD_MUSIC, 2);
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    finish();
}

static void test_preload_waits_for_every_sound(void)
{
    set_up();
    add_ping();
    fake_files_add("assets/sounds/pong.ogg", sine_mono_ogg, sizeof(sine_mono_ogg));
    add_scene("scene opening\nsound ping\nwait 60\nsound pong\n");
    CHECK(scene_player_play("opening"));
    run_frames(30);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_PRELOADING);
    fake_audio_thread_pump(AUDIO_THREAD_SOUND, 1);
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_PRELOADING);
    fake_audio_thread_pump(AUDIO_THREAD_SOUND, 1);
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    finish();
}

static void test_missing_files_never_hang_the_preload_and_the_scene_runs_silently(void)
{
    set_up();
    add_scene("scene opening\nmusic gone 1\nsound missing\nwait 6\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    CHECK_HAS_TEXT(log_text(), "music file_missing name=gone");
    CHECK_HAS_TEXT(log_text(), "sound file_missing name=missing");
    run_frames(10);
    pump_workers(5);
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    CHECK_HAS_TEXT(log_text(), "sound dropped name=missing reason=not_loaded");
    finish();
}

static void test_damaged_files_never_hang_the_preload_and_the_scene_runs_silently(void)
{
    static unsigned char damaged_song[sizeof(sine_stereo_ogg)];
    static unsigned char damaged_ping[sizeof(sine_mono_ogg)];

    set_up();
    memcpy(damaged_song, sine_stereo_ogg, sizeof(sine_stereo_ogg));
    damaged_song[SINE_STEREO_VENDOR_LENGTH_TOP_BYTE_OFFSET] ^= 0x80;
    fake_files_add("assets/music/song.ogg", damaged_song, sizeof(damaged_song));
    memcpy(damaged_ping, sine_mono_ogg, sizeof(sine_mono_ogg));
    damaged_ping[SINE_MONO_VENDOR_LENGTH_TOP_BYTE_OFFSET] ^= 0x80;
    fake_files_add("assets/sounds/ping.ogg", damaged_ping, sizeof(damaged_ping));
    add_scene("scene opening\nmusic song 1\nsound ping\nwait 6\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    CHECK_HAS_TEXT(log_text(), "music decoder_failed name=song error=-1");
    CHECK_HAS_TEXT(log_text(), "sound decoder_failed name=ping error=-1");
    run_frames(10);
    pump_workers(5);
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    CHECK_HAS_TEXT(log_text(), "sound dropped name=ping reason=not_loaded");
    finish();
}

static void test_a_sound_of_only_headers_never_hangs_the_preload(void)
{
    set_up();
    fake_files_add("assets/sounds/ping.ogg", sine_mono_ogg, SINE_MONO_HEADER_PAGES_BYTES);
    add_scene("scene opening\nsound ping\nwait 6\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    CHECK_HAS_TEXT(log_text(), "sound decoder_failed name=ping error=no_frames");
    finish();
}

static void test_workers_that_never_answer_only_hold_the_preload_for_ten_seconds(void)
{
    set_up();
    add_scene("scene opening\nmusic song 1\nsound ping\nwait 60\n");
    CHECK(scene_player_play("opening"));
    fake_clock_advance_us(GIVE_UP_US - FRAME_US - 1);
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_PRELOADING);
    fake_clock_advance_us(2);
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK_HAS_TEXT(log_text(), "scene audio_wait_gave_up name=opening");
    finish();
}

static void test_workers_that_cannot_start_do_not_hold_the_preload(void)
{
    fake_files_reset();
    fake_memory_reset();
    fake_audio_reset();
    fake_renderer_reset();
    fake_reader_reset();
    fake_clock_set_us(0);
    run_log_open("run.log");
    fake_audio_thread_fail_start(AUDIO_THREAD_MUSIC, -1);
    fake_audio_thread_fail_start(AUDIO_THREAD_SOUND, -1);
    start_engines();
    add_scene("scene opening\nmusic song 1\nsound ping\nwait 60\n");
    CHECK(scene_player_play("opening"));
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    finish();
}

static void test_audio_steps_fire_at_their_place_and_do_not_hold_the_script(void)
{
    set_up();
    add_song("assets/music/song.ogg");
    add_ping();
    add_scene("scene opening\nwait 6\nmusic song 1\nsound ping\nwait 6000\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    run_frames(2);
    pump_workers(1);
    CHECK(strstr(log_text(), "music started") == NULL);
    CHECK(strstr(log_text(), "sound started") == NULL);
    run_frames(8);
    CHECK_INT(scene_player_next_step(), 4);
    pump_workers(1);
    CHECK_HAS_TEXT(log_text(), "music started name=song looping=1");
    CHECK_HAS_TEXT(log_text(), "sound started name=ping voice=0");
    finish();
}

static void test_a_music_step_with_loop_zero_says_so(void)
{
    set_up();
    add_song("assets/music/song.ogg");
    add_scene("scene opening\nmusic song 0\nwait 6000\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    run_frames(2);
    pump_workers(1);
    CHECK_HAS_TEXT(log_text(), "music started name=song looping=0");
    finish();
}

static void test_a_music_stop_step_fades_for_its_own_ticks_and_the_scene_goes_on(void)
{
    set_up();
    add_song("assets/music/song.ogg");
    add_scene("scene opening\nmusic song 1\nwait 6\nmusic_stop 4\nwait 6000\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    run_frames(2);
    pump_workers(2);
    run_frames(10);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    pump_workers(1);
    CHECK_HAS_TEXT(log_text(), "frames=2940 from=32768 to=0");
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=stop ");
    finish();
}

static void test_replaying_stops_the_sound_and_the_preload_waits_again(void)
{
    set_up();
    add_song("assets/music/song.ogg");
    add_ping();
    add_scene("scene opening\nmusic song 1\nsound ping\nwait 6000\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    run_frames(2);
    pump_workers(2);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(scene_player_play("opening"));
    run_frames(1);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_PRELOADING);
    pump_workers(1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=stop ");
    CHECK_HAS_TEXT(log_text(), "sound stopped reason=stop ");
    get_scene_running();
    run_frames(2);
    pump_workers(1);
    CHECK_INT(count_in_log("music started name=song"), 2);
    CHECK_INT(count_in_log("sound started name=ping"), 2);
    CHECK_INT(count_in_log("music loaded"), 1);
    CHECK_INT(count_in_log("sound loaded"), 1);
    finish();
}

static void test_leaving_the_scene_stops_the_sound(void)
{
    set_up();
    add_song("assets/music/song.ogg");
    add_ping();
    add_scene("scene opening\nmusic song 1\nsound ping\nwait 6000\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    run_frames(2);
    pump_workers(2);
    scene_player_stop();
    pump_workers(2);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_SOUND));
    pump_workers(2);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 1);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_SOUND), 1);
    finish();
}

static void test_the_music_stops_when_the_scene_finishes(void)
{
    set_up();
    add_song("assets/music/song.ogg");
    add_scene("scene opening\nmusic song 1\nwait 6\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    run_frames(2);
    pump_workers(2);
    CHECK(fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    run_frames(10);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_FINISHED);
    pump_workers(1);
    CHECK(!fake_audio_port_is_open(AUDIO_PORT_MUSIC));
    CHECK_HAS_TEXT(log_text(), "music stopped name=song reason=stop ");
    finish();
}

static void test_skipping_holds_past_audio_steps_breaks_nothing(void)
{
    set_up();
    add_song("assets/music/song.ogg");
    add_ping();
    add_scene("scene opening\nwait 6000\nmusic song 1\nsound ping\nwait 6000\nmusic_stop 4\nsound ping\nwait 6000\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    run_frames(1);
    scene_player_skip_hold();
    run_frames(1);
    scene_player_skip_hold();
    run_frames(1);
    CHECK_INT(scene_player_next_step(), 7);
    pump_workers(2);
    CHECK_INT(scene_player_status(), SCENE_PLAYER_RUNNING);
    CHECK_INT(fake_audio_port_open_count(AUDIO_PORT_MUSIC), 0);
    CHECK_INT(count_in_log("sound started name=ping"), 2);
    finish();
}

static void test_a_later_track_loads_when_its_step_runs(void)
{
    set_up();
    add_song("assets/music/first.ogg");
    add_song("assets/music/second.ogg");
    add_scene("scene opening\nmusic first 0\nwait 6\nmusic second 0\nwait 6000\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    CHECK_HAS_TEXT(log_text(), "music loaded name=first");
    CHECK(strstr(log_text(), "music loaded name=second") == NULL);
    run_frames(2);
    pump_workers(1);
    run_frames(10);
    pump_workers(2);
    CHECK_HAS_TEXT(log_text(), "music loaded name=second");
    CHECK_HAS_TEXT(log_text(), "music started name=second");
    finish();
}

static void test_the_debug_pause_leaves_the_music_playing(void)
{
    int grains_before;

    set_up();
    add_song("assets/music/song.ogg");
    add_scene("scene opening\nmusic song 1\nwait 6000\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    run_frames(2);
    pump_workers(1);
    scene_player_set_paused(1);
    grains_before = fake_audio_port_grain_count(AUDIO_PORT_MUSIC);
    pump_workers(3);
    CHECK_INT(fake_audio_port_grain_count(AUDIO_PORT_MUSIC), grains_before + 3);
    finish();
}

static void test_sound_never_reaches_the_music_port(void)
{
    set_up();
    add_ping();
    add_scene("scene opening\nsound ping\nwait 6000\n");
    CHECK(scene_player_play("opening"));
    get_scene_running();
    run_frames(2);
    pump_workers(3);
    CHECK(fake_audio_port_grain_count(AUDIO_PORT_SOUND) > 0);
    CHECK_INT(fake_audio_port_grain_count(AUDIO_PORT_MUSIC), 0);
    CHECK(strstr(log_text(), "music window") == NULL);
    finish();
}

int main(void)
{
    test_a_scene_without_audio_does_not_wait_for_the_workers();
    test_preload_waits_for_the_music();
    test_preload_waits_for_every_sound();
    test_missing_files_never_hang_the_preload_and_the_scene_runs_silently();
    test_damaged_files_never_hang_the_preload_and_the_scene_runs_silently();
    test_a_sound_of_only_headers_never_hangs_the_preload();
    test_workers_that_never_answer_only_hold_the_preload_for_ten_seconds();
    test_workers_that_cannot_start_do_not_hold_the_preload();
    test_audio_steps_fire_at_their_place_and_do_not_hold_the_script();
    test_a_music_step_with_loop_zero_says_so();
    test_a_music_stop_step_fades_for_its_own_ticks_and_the_scene_goes_on();
    test_replaying_stops_the_sound_and_the_preload_waits_again();
    test_leaving_the_scene_stops_the_sound();
    test_the_music_stops_when_the_scene_finishes();
    test_skipping_holds_past_audio_steps_breaks_nothing();
    test_a_later_track_loads_when_its_step_runs();
    test_the_debug_pause_leaves_the_music_playing();
    test_sound_never_reaches_the_music_port();
    fake_audio_reset();
    fake_files_reset();
    return test_finish("unit/scene_audio");
}
