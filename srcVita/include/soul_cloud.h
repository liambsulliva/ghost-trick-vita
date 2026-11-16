#ifndef SOUL_CLOUD_H
#define SOUL_CLOUD_H

#define SOUL_CLOUD_PARTICLES 64
#define SOUL_CLOUD_ONE 4096
#define SOUL_CLOUD_QUAD_SIZE 0x2666
#define SOUL_CLOUD_BURST_LEVEL 6
#define SOUL_CLOUD_BURST_DELAY 6

struct soul_cloud {
    int x[SOUL_CLOUD_PARTICLES];
    int y[SOUL_CLOUD_PARTICLES];
    int w[SOUL_CLOUD_PARTICLES];
    int vx[SOUL_CLOUD_PARTICLES];
    int vy[SOUL_CLOUD_PARTICLES];
    int tx[SOUL_CLOUD_PARTICLES];
    int is_on;
    int level;
    int delay_left;
    int hold;
    unsigned int seed;
};

struct soul_cloud_particle {
    float x;
    float y;
    float size;
};

void soul_cloud_reset(struct soul_cloud *cloud);
void soul_cloud_set_on(struct soul_cloud *cloud, int is_on);
void soul_cloud_step(struct soul_cloud *cloud, float soul_x, float soul_y);
int soul_cloud_particles(const struct soul_cloud *cloud, struct soul_cloud_particle *particles);

#endif
