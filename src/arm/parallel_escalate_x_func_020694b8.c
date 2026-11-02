struct Frame {
    int data;
    int time;
};

struct Seq {
    unsigned short count;
    unsigned short loop_start;
    int type;
    int mode;
    struct Frame *frames;
};

struct Ctrl {
    struct Frame *current;
    struct Frame *active;
    int reverse;
    int is_active;
    int time;
    int speed;
    int mode;
    struct Seq *seq;
    int cb_type;
    int cb_param;
    void (*cb_func)(int param, int time);
    int cb_frame;
};

extern void func_020697b4(struct Ctrl *ctrl);

void func_020694b8(struct Ctrl *ctrl)
{
    int mode;
    struct Seq *seq;
    struct Frame *cur;

    mode = ctrl->mode != 0 ? ctrl->mode : ctrl->seq->mode;
    switch (mode) {
    case 3:
    case 4:
        ctrl->reverse ^= 1;
        if (ctrl->current <= ctrl->seq->frames + ctrl->seq->loop_start - 1) {
            if (ctrl->cb_type == 1) {
                ctrl->cb_func(ctrl->cb_param, ctrl->time);
            }
            mode = ctrl->mode != 0 ? ctrl->mode : ctrl->seq->mode;
            if (mode != 2 && mode != 4) {
                ctrl->is_active = 0;
            } else {
                func_020697b4(ctrl);
            }
        }
        break;
    default:
        if (ctrl->cb_type == 1) {
            ctrl->cb_func(ctrl->cb_param, ctrl->time);
        }
        mode = ctrl->mode != 0 ? ctrl->mode : ctrl->seq->mode;
        if (mode != 2 && mode != 4) {
            ctrl->is_active = 0;
        } else {
            func_020697b4(ctrl);
        }
        break;
    }
    seq = ctrl->seq;
    cur = ctrl->current;
    if (cur > seq->frames + seq->count - 1) {
        ctrl->current = seq->frames + seq->count - 1;
    } else if (cur < seq->frames) {
        ctrl->current = seq->frames;
    }
}
