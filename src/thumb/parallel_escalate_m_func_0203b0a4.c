extern void func_0203b190(void *s, int a);

struct Mover {
    int unk_00[4];
    unsigned char state;
    unsigned char unk_11[0x28 - 0x11];
    int pos;
    int vel;
    unsigned char unk_30[0xc8 - 0x30];
    int mode;
    unsigned char unk_cc[0xd8 - 0xcc];
    int target;
    unsigned char unk_dc[0xe0 - 0xdc];
    int goal;
    int goal2;
    unsigned char unk_e8[0xed - 0xe8];
    unsigned char wait;
};

void func_0203b0a4(struct Mover *s)
{
    int goal;
    int target;

    if (s->wait != 0) {
        s->wait--;
        return;
    }
    if (s->state == 3) {
        return;
    }
    if (s->state == 0) {
        s->pos = s->goal;
        s->vel = s->goal2;
        s->state = 1;
        s->wait = 5;
        return;
    }
    if (s->state == 1) {
        goal = s->goal;
        target = s->target;
        if (target > goal) {
            if (s->pos < target) {
                s->pos = s->pos + 0x3000;
                return;
            }
            s->pos = target;
            s->state = 4;
            return;
        }
        if (target < goal) {
            if (s->pos > target) {
                s->pos = s->pos - 0x3000;
                return;
            }
            s->pos = target;
            s->state = 4;
        }
        return;
    }
    if (s->state == 2) {
        goal = s->goal;
        target = s->target;
        if (target > goal) {
            if (s->pos > goal) {
                s->pos = s->pos - 0x3000;
                return;
            }
            s->pos = goal;
            s->state = 6;
            return;
        }
        if (target < goal) {
            if (s->pos < goal) {
                s->pos = s->pos + 0x3000;
                return;
            }
            s->pos = goal;
            s->state = 6;
        }
        return;
    }
    if (s->state == 5) {
        if (s->mode == 1) {
            func_0203b190(s, 0x52);
        } else if (s->mode == 2) {
            func_0203b190(s, 0x54);
        }
        s->state = 4;
    }
}
