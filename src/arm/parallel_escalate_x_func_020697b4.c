struct Frame {
    int data;
    int time;
};

struct Seq {
    unsigned short count;
    unsigned short loop_start;
    int pad04;
    int pad08;
    struct Frame *frames;
};

struct Ctrl {
    struct Frame *current;
    struct Frame *active;
    int reverse;
    int pad0c;
    int timer;
    int speed;
    int pad18;
    struct Seq *seq;
};

extern void func_020695c8();

void func_020697b4(struct Ctrl *ctrl)
{
    if ((ctrl->speed > 0) ^ ctrl->reverse) {
        ctrl->current = &ctrl->seq->frames[ctrl->seq->loop_start];
    } else {
        ctrl->current = ctrl->seq->frames + ctrl->seq->count - 1;
    }
    ctrl->active = ctrl->current;
    ctrl->timer = 0;
    func_020695c8(ctrl);
}
