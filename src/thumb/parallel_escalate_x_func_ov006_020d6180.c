typedef struct Obj6180 {
    char pad0[0x25];
    unsigned char flags;
    unsigned char state;
    char pad27[0xb];
    signed char levels[2];
    char pad34[0x3ae];
    short x;
    short y;
    short timer;
    signed char level;
    char pad3e9[3];
    unsigned char ready;
    char pad3ed[0x3b];
    int handle;
} Obj6180;

extern void *func_02010b7c(void);
extern int func_02015154(void *a);
extern int func_0202cb64(void *a);
extern int func_02010b60(void *a);
extern int func_02010b64(void *a);
extern void func_ov006_020d7d74(Obj6180 *p, int arg);
extern void func_ov006_020d4268(int arg);
extern void func_0202f64c(int *handle);

void func_ov006_020d6180(Obj6180 *p, int arg)
{
    unsigned char side;
    short x;
    short y;
    short dx;
    short dy;
    int moved;

    side = p->flags & 1;
    if (func_02015154(func_02010b7c()) == 0 || func_0202cb64(func_02010b7c()) != 0) {
        if (p->ready != 0) {
            func_ov006_020d7d74(p, arg);
            return;
        }
        p->state = 0;
        return;
    }
    x = func_02010b60(func_02010b7c());
    y = func_02010b64(func_02010b7c());
    dx = x - p->x;
    dy = y - p->y;
    moved = 0;
    if (dx > 8) {
        moved = 1;
    }
    if (dx < -8) {
        moved = 1;
    }
    if (dy > 8) {
        moved = 1;
    }
    if (dy < -8) {
        moved = 1;
    }
    if (moved) {
        func_ov006_020d4268(arg);
        p->state = 10;
        p->x = x;
        p->y = y;
        p->level = p->levels[side];
        p->timer = 0;
        func_0202f64c(&p->handle);
    }
}
