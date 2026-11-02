struct Vec {
    int x;
    int y;
    int z;
};

struct Mover {
    char pad00[0x10];
    unsigned int flags;
    char pad14[0x14];
    struct Vec pos;
    char pad34[0x48];
    struct Vec dir;
    int scale_x;
    int scale_y;
    char pad90[0x10];
    unsigned char alpha;
    char pada1[7];
    int alpha_fx;
    int end_scale_x;
    int end_scale_y;
    int end_pos_x;
    int end_pos_y;
    int end_dir_x;
    int end_dir_y;
    int end_alpha;
    char padc8[4];
    int step_scale_x;
    int step_scale_y;
    int scale_count;
    int step_pos_x;
    int step_pos_y;
    int pos_count;
    int step_dir_x;
    int step_dir_y;
    int dir_count;
    int step_alpha;
    char padf4[4];
    int alpha_count;
};

extern void func_020632f0(struct Mover *mover, struct Vec *pos, struct Vec *dir, int scale_x, int scale_y);
extern void func_0206346c(struct Mover *mover);

void func_0205a234(struct Mover *mover)
{
    if (mover->flags != 0) {
        if (mover->flags & 1) {
            if (mover->scale_count >= 0) {
                mover->scale_x += mover->step_scale_x;
                mover->scale_y += mover->step_scale_y;
                func_020632f0(mover, &mover->pos, &mover->dir, mover->scale_x, mover->scale_y);
                mover->scale_count--;
            } else {
                func_020632f0(mover, &mover->pos, &mover->dir, mover->end_scale_x, mover->end_scale_y);
                mover->flags &= ~1;
            }
        }
        if (mover->flags & 2) {
            if (mover->pos_count >= 0) {
                mover->pos.x += mover->step_pos_x;
                mover->pos.y += mover->step_pos_y;
                func_020632f0(mover, &mover->pos, &mover->dir, mover->scale_x, mover->scale_y);
                mover->pos_count--;
            } else {
                mover->pos.x = mover->end_pos_x;
                mover->pos.y = mover->end_pos_y;
                func_020632f0(mover, &mover->pos, &mover->dir, mover->scale_x, mover->scale_y);
                mover->flags &= ~2;
            }
        }
        if (mover->flags & 4) {
            if (mover->dir_count >= 0) {
                mover->dir.x += mover->step_dir_x;
                mover->dir.y += mover->step_dir_y;
                func_020632f0(mover, &mover->pos, &mover->dir, mover->scale_x, mover->scale_y);
                mover->dir_count--;
            } else {
                mover->dir.x = mover->end_dir_x;
                mover->dir.y = mover->end_dir_y;
                func_020632f0(mover, &mover->pos, &mover->dir, mover->scale_x, mover->scale_y);
                mover->flags &= ~4;
            }
        }
        if (mover->flags & 8) {
            if (mover->alpha_count >= 0) {
                mover->alpha_fx += mover->step_alpha;
                mover->alpha = mover->alpha_fx >> 12;
                mover->alpha_count--;
            } else {
                mover->alpha = mover->end_alpha >> 12;
                mover->flags &= ~8;
            }
        }
    }
    func_0206346c(mover);
}
