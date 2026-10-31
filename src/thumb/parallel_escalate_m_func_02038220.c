extern int func_0200847c();
extern int func_020283fc();
extern int func_0202a0c0();
extern int func_02038ba4();
extern int func_02039e1c();
extern int func_02039e54();
extern int func_02039ea0();
extern void func_02039eb8(unsigned char *param_1, unsigned int param_2, int param_3);
extern int func_0203a4dc();

struct State {
    int *obj;
    int pad4[2];
    int *child;
    int pad10[3];
    int pos;
    int pad20;
    short pad24;
    unsigned short timer;
    int mode;
    int pad2c;
    signed char step;
    unsigned char count;
    char pad32[3];
    unsigned char sound;
};

int func_02038220(struct State *s)
{
    int mode;
    int r;

    if (s->timer != 0) {
        s->timer = s->timer - 1;
        return 0;
    }
    mode = s->mode;
    if (mode == 1) {
        func_02039e1c(s->obj, -1);
        func_02039e54(s->obj, 5);
        if (s->count == 0) {
            s->mode = 0;
            return 1;
        }
        func_02038ba4(s);
        func_0203a4dc(s->child, s->step);
        func_0202a0c0(func_0200847c(), 1);
        s->count = s->count - 1;
        s->timer = 10;
        return 0;
    }
    if (mode == 2) {
        if (s->count != 0) {
            s->pos = s->pos - s->step;
            func_02039eb8((unsigned char *)s->obj, 2, s->step);
            func_0202a0c0(func_0200847c(), 1);
            s->count = s->count - 1;
        }
        return 0;
    }
    if (mode == 3) {
        r = func_02039ea0(s->obj);
        if (r != 0 && func_020283fc(r) != 0x33) {
            return 0;
        }
        if (s->count == 0) {
            func_02039e1c(s->obj, -1);
            func_02039e54(s->obj, 5);
            s->mode = 0;
            return 1;
        }
        func_02038ba4(s);
        if (s->sound != 0) {
            func_0202a0c0(func_0200847c(), 1);
            s->sound = 0;
        }
        s->count = s->count - 1;
        return 0;
    }
    return 0;
}
