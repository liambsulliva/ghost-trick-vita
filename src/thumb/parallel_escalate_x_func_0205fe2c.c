typedef struct Vec5fe2c {
    int x;
    int y;
    int z;
} Vec5fe2c;

typedef struct Obj5fe2c {
    char pad0[0x7c];
    unsigned char low7c : 2;
    unsigned char shown : 1;
    unsigned char high7c : 5;
    char pad7d;
    unsigned char locked;
    unsigned char count;
    unsigned char moved;
    char pad81[0xb];
    short cursor_x;
    short cursor_y;
    int cam_x;
    int cam_y;
    char pad98[0x38];
    Vec5fe2c *points;
} Obj5fe2c;

extern unsigned int data_020c9d50;
extern void *func_02010b7c(void);
extern int func_02010b60(void *touch);
extern int func_02010b64(void *touch);
extern void *func_02008444(void);
extern int func_0200f5c8(void *game);
extern int func_0200f5e8(void *game);
extern void func_020082d8(Vec5fe2c *dst, Vec5fe2c *src);
extern int func_02060014(int value);
extern void func_0200f70c(Vec5fe2c *v);

void func_0205fe2c(Vec5fe2c *out, Obj5fe2c *obj, int mode)
{
    short x;
    short y;
    int cam_x;
    int cam_y;
    short dx;
    short dy;
    int ax;
    int ay;
    Vec5fe2c target;
    Vec5fe2c pos;

    x = func_02010b60(func_02010b7c());
    y = func_02010b64(func_02010b7c());
    cam_x = func_0200f5c8(func_02008444());
    cam_y = func_0200f5e8(func_02008444());
    if (mode == 0) {
        func_020082d8(&target, &obj->points[obj->count - 1]);
        if (data_020c9d50 & 0x100) {
            target.y -= 0x6000;
        } else if (data_020c9d50 & 0x200) {
            target.y += 0x6000;
        }
        if (data_020c9d50 & 0x800) {
            target.x += 0x6000;
        } else if (data_020c9d50 & 0x400) {
            target.x -= 0x6000;
        }
        obj->cursor_x = (target.x - func_0200f5c8(func_02008444())) >> 12;
        obj->cursor_y = (target.y - func_0200f5e8(func_02008444())) >> 12;
        obj->cam_x = cam_x;
        obj->cam_y = cam_y;
        obj->moved = 0;
        if (obj->locked == 0) {
            obj->shown = 1;
        } else {
            obj->shown = 0;
        }
        func_020082d8(out, &target);
        return;
    }
    dx = func_02060014(x - obj->cursor_x);
    dy = func_02060014(y - obj->cursor_y);
    ax = cam_x - obj->cam_x;
    if (ax < 0) {
        ax = -ax;
    }
    ay = cam_y - obj->cam_y;
    if (ay < 0) {
        ay = -ay;
    }
    if (dx < 3 && dy < 3 && ax < 0x19a && ay < 0x19a) {
        x = obj->cursor_x;
        y = obj->cursor_y;
    } else {
        obj->cursor_x = x;
        obj->cursor_y = y;
        obj->cam_x = cam_x;
        obj->cam_y = cam_y;
        obj->moved = 0;
        if (obj->locked == 0) {
            obj->shown = 1;
        } else {
            obj->shown = 0;
        }
    }
    func_0200f70c(&pos);
    pos.x = (x << 12) + func_0200f5c8(func_02008444());
    pos.y = (y << 12) + func_0200f5e8(func_02008444());
    pos.z = 0;
    func_020082d8(out, &pos);
}
