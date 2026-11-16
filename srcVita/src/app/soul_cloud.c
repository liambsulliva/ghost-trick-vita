#include <string.h>
#include "soul_cloud.h"

#define WIDE_MASK 127
#define SHRINK_PER_TICK 0x59a
#define SEED_START 0x2f6b1d3u

static unsigned int next_random(struct soul_cloud *cloud)
{
    cloud->seed = cloud->seed * 1103515245u + 12345u;
    return (cloud->seed >> 16) & 0x7fffu;
}

static int drawn(struct soul_cloud *cloud, int mask)
{
    if (cloud->level > 0) {
        return mask / 2 + (int)(next_random(cloud) & (unsigned int)(mask / 2));
    }
    return (int)(next_random(cloud) & (unsigned int)mask);
}

static void respawn(struct soul_cloud *cloud, int index, int soul_x, int soul_y)
{
    int jitter_x;
    int jitter_y;

    jitter_x = (drawn(cloud, WIDE_MASK) - 64) * 128;
    jitter_y = (drawn(cloud, WIDE_MASK) - 64) * 128;
    cloud->x[index] = soul_x + jitter_x;
    cloud->y[index] = soul_y + jitter_y;
    cloud->tx[index] = cloud->x[index];
    cloud->vx[index] = (drawn(cloud, WIDE_MASK) - 64) * 256;
    cloud->vy[index] = -(drawn(cloud, WIDE_MASK) * 64);
    cloud->w[index] = drawn(cloud, WIDE_MASK) * 256;
}

void soul_cloud_reset(struct soul_cloud *cloud)
{
    memset(cloud, 0, sizeof(*cloud));
    cloud->seed = SEED_START;
}

void soul_cloud_set_on(struct soul_cloud *cloud, int is_on)
{
    cloud->is_on = is_on;
    if (is_on) {
        cloud->level = SOUL_CLOUD_BURST_LEVEL;
        cloud->delay_left = SOUL_CLOUD_BURST_DELAY;
        cloud->hold = 0;
    }
}

void soul_cloud_step(struct soul_cloud *cloud, float soul_x, float soul_y)
{
    int fixed_x;
    int fixed_y;
    int index;

    fixed_x = (int)(soul_x * (float)SOUL_CLOUD_ONE);
    fixed_y = (int)(soul_y * (float)SOUL_CLOUD_ONE);
    if (cloud->level > 0) {
        cloud->delay_left = cloud->delay_left - 1;
        if (cloud->delay_left <= 0) {
            cloud->level = cloud->level - 1;
            cloud->delay_left = SOUL_CLOUD_BURST_DELAY;
        }
    }
    if (cloud->hold > 0) {
        cloud->hold = cloud->hold - 1;
    }
    for (index = 0; index < SOUL_CLOUD_PARTICLES; index++) {
        cloud->x[index] = cloud->x[index] + cloud->vx[index];
        cloud->y[index] = cloud->y[index] + cloud->vy[index];
        cloud->vx[index] = cloud->vx[index] + ((cloud->tx[index] - cloud->x[index]) >> 1);
        cloud->w[index] = cloud->w[index] - SHRINK_PER_TICK;
        if (cloud->w[index] <= 0) {
            cloud->w[index] = 0;
            if (cloud->is_on && cloud->hold <= 0) {
                respawn(cloud, index, fixed_x, fixed_y);
                cloud->hold = cloud->level;
            }
        }
    }
}

int soul_cloud_particles(const struct soul_cloud *cloud, struct soul_cloud_particle *particles)
{
    int count;
    int index;

    count = 0;
    for (index = 0; index < SOUL_CLOUD_PARTICLES; index++) {
        if (cloud->w[index] <= 0) {
            continue;
        }
        particles[count].x = (float)cloud->x[index] / (float)SOUL_CLOUD_ONE;
        particles[count].y = (float)cloud->y[index] / (float)SOUL_CLOUD_ONE;
        particles[count].size = (float)SOUL_CLOUD_QUAD_SIZE / (float)SOUL_CLOUD_ONE * (float)cloud->w[index] / (float)SOUL_CLOUD_ONE;
        count = count + 1;
    }
    return count;
}
