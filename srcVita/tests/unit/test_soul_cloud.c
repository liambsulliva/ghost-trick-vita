#include "soul_cloud.h"
#include "test_support.h"

#define TICKS_FOR_A_PARTICLE_TO_LIVE_OUT 30
#define SOUL_X 100.0f
#define SOUL_Y 80.0f
#define LONGEST_RISE_DS_PIXELS 46.0f
#define WIDEST_SWAY_DS_PIXELS 12.0f
#define LARGEST_SIZE_DS_PIXELS 19.1f

static void test_an_off_cloud_shows_nothing(void)
{
    struct soul_cloud cloud;
    struct soul_cloud_particle particles[SOUL_CLOUD_PARTICLES];
    int tick;

    soul_cloud_reset(&cloud);
    for (tick = 0; tick < 10; tick++) {
        soul_cloud_step(&cloud, SOUL_X, SOUL_Y);
    }
    CHECK(soul_cloud_particles(&cloud, particles) == 0);
}

static void test_the_cloud_rises_from_the_soul_and_stays_within_native_reach(void)
{
    struct soul_cloud cloud;
    struct soul_cloud_particle particles[SOUL_CLOUD_PARTICLES];
    int tick;
    int count;
    int index;
    int rose;

    soul_cloud_reset(&cloud);
    soul_cloud_set_on(&cloud, 1);
    rose = 0;
    for (tick = 0; tick < 120; tick++) {
        soul_cloud_step(&cloud, SOUL_X, SOUL_Y);
        count = soul_cloud_particles(&cloud, particles);
        for (index = 0; index < count; index++) {
            CHECK(particles[index].y <= SOUL_Y + 2.0f);
            CHECK(particles[index].y >= SOUL_Y - LONGEST_RISE_DS_PIXELS);
            CHECK(particles[index].x >= SOUL_X - WIDEST_SWAY_DS_PIXELS && particles[index].x <= SOUL_X + WIDEST_SWAY_DS_PIXELS);
            CHECK(particles[index].size > 0.0f && particles[index].size <= LARGEST_SIZE_DS_PIXELS);
            if (particles[index].y < SOUL_Y - 10.0f) {
                rose = 1;
            }
        }
    }
    CHECK(soul_cloud_particles(&cloud, particles) > SOUL_CLOUD_PARTICLES / 2);
    CHECK(rose);
}

static void test_a_cloud_turned_off_dies_out(void)
{
    struct soul_cloud cloud;
    struct soul_cloud_particle particles[SOUL_CLOUD_PARTICLES];
    int tick;

    soul_cloud_reset(&cloud);
    soul_cloud_set_on(&cloud, 1);
    for (tick = 0; tick < 60; tick++) {
        soul_cloud_step(&cloud, SOUL_X, SOUL_Y);
    }
    soul_cloud_set_on(&cloud, 0);
    for (tick = 0; tick < TICKS_FOR_A_PARTICLE_TO_LIVE_OUT; tick++) {
        soul_cloud_step(&cloud, SOUL_X, SOUL_Y);
    }
    CHECK(soul_cloud_particles(&cloud, particles) == 0);
}

int main(void)
{
    test_an_off_cloud_shows_nothing();
    test_the_cloud_rises_from_the_soul_and_stays_within_native_reach();
    test_a_cloud_turned_off_dies_out();
    return test_finish("unit/soul_cloud");
}
