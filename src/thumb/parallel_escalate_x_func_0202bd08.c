typedef struct {
    int x;
    int y;
    int z;
} Vec;

typedef struct {
    short x;
    short y;
    short z;
    short pad;
} Point;

struct View {
    unsigned char pad0[0x4c];
    int mtx[12];
    unsigned char pad7c[0x4];
    unsigned char *camera;
};

struct Work {
    unsigned char pad0[0x14];
    Vec pos;
};

extern Vec data_020ca1ec;
extern struct Work data_020ca1d8;
extern void func_0206e930(Vec *src, int *mtx, Vec *dst);
extern unsigned int func_0202beb0(unsigned char *camera);
extern void *func_02008444(void);
extern int func_0200f5c8(void *a);
extern int func_0200f5e8(void *a);

int func_0202bd08(struct View *view, Point *out, Vec *corners, int x, int y)
{
    int top;
    int bottom;
    int left;
    int right;
    int center_x;
    int base_x;
    int base_y;
    Point *point;
    int center_y;
    unsigned int i;
    int dx;
    int dy;
    int px;
    int py;

    top = 0;
    bottom = 0;
    left = 0;
    right = 0;
    center_x = 0;
    base_x = x << 12;
    base_y = y << 12;
    center_y = 0;
    for (i = 0; i < 4; i++) {
        func_0206e930(&corners[i], view->mtx, &data_020ca1ec);
        dx = data_020ca1d8.pos.x - base_x;
        dy = base_y - data_020ca1d8.pos.y;
        px = dx & 0xfffff000;
        if ((dx & 0xfff) >= 0x7ff) {
            px += 0x1000;
        }
        py = dy & 0xfffff000;
        if ((dy & 0xfff) >= 0x7ff) {
            py += 0x1000;
        }
        point = &out[i];
        point->z = 0;
        if (func_0202beb0(view->camera) == 0) {
            px -= func_0200f5c8(func_02008444());
            py += func_0200f5e8(func_02008444());
        }
        if (px > 0x2ee000) {
            return 0;
        }
        if (px < -0x2ee000) {
            return 0;
        }
        if (py > 0x2ee000) {
            return 0;
        }
        if (py < -0x2ee000) {
            return 0;
        }
        if (center_x == 0) {
            if (px > 0x84000) {
                left = 1;
            } else if (px < -0x84000) {
                right = 1;
            } else {
                center_x = 1;
            }
        }
        if (center_y == 0) {
            if (py > 0x64000) {
                top = 1;
            } else if (py < -0x64000) {
                bottom = 1;
            } else {
                center_y = 1;
            }
        }
        point->x = px >> 7;
        point->y = py >> 7;
    }
    if (left && !right && !center_x) {
        return 0;
    }
    if (!left && right && !center_x) {
        return 0;
    }
    if (top && !bottom && !center_y) {
        return 0;
    }
    if (!top && bottom && !center_y) {
        return 0;
    }
    return 1;
}
