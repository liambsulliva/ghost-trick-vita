typedef struct Frame020695c8 {
    void *content;
    unsigned short frames;
    unsigned short pad;
} Frame020695c8;

typedef struct Seq020695c8 {
    unsigned short numFrames;
    unsigned short loopStart;
    unsigned int animType;
    unsigned int playMode;
    Frame020695c8 *frames;
} Seq020695c8;

typedef void (*AnimCallback020695c8)(unsigned int param, unsigned short frameIdx);

typedef struct Cb020695c8 {
    int type;
    unsigned int param;
    AnimCallback020695c8 func;
    unsigned short frame;
} Cb020695c8;

typedef struct Ctrl020695c8 {
    Frame020695c8 *cur;
    Frame020695c8 *active;
    int reverse;
    int isActive;
    int time;
    int speed;
    int pad18;
    Seq020695c8 *seq;
    Cb020695c8 cb;
} Ctrl020695c8;

extern int func_0207a540(int value);
extern void func_020694b8(Ctrl020695c8 *ctrl);

static inline int fx_mul(int a, int b)
{
    return (int)(((long long)a * b + 0x800) >> 12);
}

static inline int reached_frame_end(const Ctrl020695c8 *ctrl)
{
    if (ctrl->isActive && ctrl->time >= (ctrl->cur->frames << 12)) {
        return 1;
    }
    return 0;
}

static inline void call_back(const Cb020695c8 *cb, unsigned short idx)
{
    switch (cb->type) {
    case 2:
        if (idx == cb->frame) {
            cb->func(cb->param, idx);
        }
        break;
    case 3:
        cb->func(cb->param, idx);
        break;
    }
}

int func_020695c8(Ctrl020695c8 *ctrl, int frames)
{
    int changed = 0;
    unsigned short idx;
    Frame020695c8 *p;

    if (ctrl->isActive != 1) {
        return 0;
    }
    ctrl->time += func_0207a540(fx_mul(ctrl->speed, frames));
    while (reached_frame_end(ctrl)) {
        changed = 1;
        ctrl->time -= ctrl->cur->frames << 12;
        if ((ctrl->speed > 0) ^ ctrl->reverse) {
            p = ctrl->cur + 1;
        } else {
            p = ctrl->cur - 1;
        }
        ctrl->cur = p;
        if (((ctrl->speed > 0) ^ ctrl->reverse) ? (p >= ctrl->seq->frames + ctrl->seq->numFrames) : (p <= ctrl->seq->frames + ctrl->seq->loopStart - 1)) {
            func_020694b8(ctrl);
        }
        if (ctrl->cur->frames != 0) {
            ctrl->active = ctrl->cur;
        }
        if (ctrl->cb.type != 0) {
            idx = (unsigned short)(((unsigned int)ctrl->cur - (unsigned int)ctrl->seq->frames) >> 3);
            call_back(&ctrl->cb, idx);
        }
    }
    return changed;
}
