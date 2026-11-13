#ifndef SOUL_CHAIN_H
#define SOUL_CHAIN_H

#define SOUL_CHAIN_POINTS 8
#define SOUL_CHAIN_TIP (SOUL_CHAIN_POINTS - 1)
#define SOUL_CHAIN_CLOUD_BLOBS 64

enum soul_chain_state {
    SOUL_CHAIN_RESTING,
    SOUL_CHAIN_AIMING,
    SOUL_CHAIN_JUMPING,
    SOUL_CHAIN_RETRACTING
};

struct soul_chain {
    float x[SOUL_CHAIN_POINTS];
    float y[SOUL_CHAIN_POINTS];
    float trail_x[SOUL_CHAIN_POINTS];
    float trail_y[SOUL_CHAIN_POINTS];
    float head_x;
    float head_y;
    enum soul_chain_state state;
    int still_ticks;
    int is_first_aiming_tick;
};

struct soul_chain_blob {
    float x;
    float y;
    float radius;
};

void soul_chain_rest_at(struct soul_chain *chain, float x, float y);
void soul_chain_hold_root(struct soul_chain *chain, float x, float y);
void soul_chain_aim(struct soul_chain *chain, float head_x, float head_y, float reach);
void soul_chain_pick(struct soul_chain *chain, float core_x, float core_y);
void soul_chain_jump(struct soul_chain *chain, float core_x, float core_y);
void soul_chain_retract(struct soul_chain *chain);
void soul_chain_step(struct soul_chain *chain);
int soul_chain_tip_is_at_head(const struct soul_chain *chain);
int soul_chain_cloud(const float *x, const float *y, float scale, unsigned int step, struct soul_chain_blob *blobs);

#endif
