typedef struct Seg62510 {
    short x;
    short y;
    short cx;
    short cy;
} Seg62510;

typedef struct Obj62510 {
    char pad0[0xc];
    unsigned char state;
    char padd[0x27];
    int sx;
    int sy;
    char pad3c[4];
    unsigned short angle;
    char pad42[0x3e];
    int r;
    int alpha;
    int x;
    int y;
    int fade;
    unsigned short depth;
    char pad96[2];
    unsigned char outline;
    unsigned char mode;
    unsigned char keep;
    signed char lim[4];
    char pad9f;
    int pts[4];
    struct Obj62510 *near[16];
    Seg62510 *segs;
} Obj62510;

extern int data_0209dd44[];
extern void *func_02008444(void);
extern int func_0202c394(void *object);
extern int func_02010b54(Obj62510 *obj);
extern int func_0202c358(Obj62510 *obj);
extern int func_0206f28c(int a, int b);
extern int func_0206ef28(int a, int b);
extern int func_0200f5c8(void *object);
extern int func_0200f5e8(void *object);
extern int func_020629d8(int value);
extern int func_020629e8(int value);
extern void *func_0202970c(void);
extern int func_02029708(void *object);
extern void func_02061fc4(void);
extern void func_02061fd0(int a, int b, int c);
extern void func_020571e4(Obj62510 *obj);
extern void func_02061fe0(int value);
extern void func_02061fec(int value);
extern void func_02061ff8(int a, int b);
extern void func_02062018(int x, int y, int z);
extern void func_02062034(void);

void func_02062510(Obj62510 *obj)
{
    unsigned int i;
    int found;
    Obj62510 *other;
    int dx;
    int dy;
    int rr;
    int tmp[4];
    int cx;
    int cy;
    int sn;
    int cs;
    int rx;
    int ry;
    unsigned int k;
    int idx;
    int c;
    int x;
    int y;
    int py;
    int px;
    int depth;

    if (func_0202c394(func_02008444()) != 0 && obj->keep == 0) {
        return;
    }
    if (obj->state != 3 || func_02010b54(obj) == 0 || func_0202c358(obj) == 0 || obj->alpha < 0x1000) {
        return;
    }
    if (obj->r == 0 || obj->sx == 0 || obj->sy == 0) {
        return;
    }
    found = 0;
    for (i = 0; i < 16; i++) {
        if (obj->near[i] == 0 || func_02010b54(obj->near[i]) == 0) {
            continue;
        }
        other = obj->near[i];
        if (other->alpha < 0x1000 || other->r < obj->r || other->sx == 0 || other->sy == 0) {
            continue;
        }
        dx = (other->x - obj->x) >> 12;
        dy = (other->y - obj->y) >> 12;
        rr = func_0206f28c(other->r, other->sx);
        rr = ((rr + func_0206f28c(obj->r, obj->sx)) >> 7) * 3;
        if (dx * dx + dy * dy < rr * rr) {
            found = 1;
            break;
        }
    }
    if (found) {
        obj->fade -= 0x200;
        if (obj->fade < 0) {
            obj->fade = 0;
        }
    } else {
        obj->fade += 0x200;
        if (obj->fade > 0x1000) {
            obj->fade = 0x1000;
        }
    }
    if (obj->fade == 0) {
        return;
    }
    for (i = 0; i < 4; i++) {
        tmp[i] = obj->pts[i];
    }
    if (obj->mode == 1) {
        for (i = 0; i < 2; i++) {
            tmp[i * 2] = obj->pts[i * 2] - 0x80000;
            tmp[i * 2 + 1] = 0x60000 - obj->pts[i * 2 + 1];
        }
    } else {
        for (i = 0; i < 2; i++) {
            tmp[i * 2] = obj->pts[i * 2] - func_0200f5c8(func_02008444()) - 0x80000;
            tmp[i * 2 + 1] = -obj->pts[i * 2 + 1] + func_0200f5e8(func_02008444()) + 0x60000;
        }
    }
    cx = obj->x >> 7;
    cy = (obj->y / 3) >> 5;
    for (i = 0; i < 2; i++) {
        tmp[i * 2] = tmp[i * 2] >> 7;
        tmp[i * 2 + 1] = (tmp[i * 2 + 1] / 3) >> 5;
    }
    sn = func_020629d8(obj->angle);
    cs = func_020629e8(obj->angle);
    rx = obj->r;
    ry = obj->r;
    if (obj->sx != 0x1000) {
        rx = func_0206f28c(ry, obj->sx);
    }
    if (obj->sy != 0x1000) {
        ry = func_0206f28c(ry, obj->sy);
    }
    if (obj->fade != 0x1000) {
        rx = func_0206f28c(rx, obj->fade);
        ry = func_0206f28c(ry, obj->fade);
    }
    for (k = 0; k < 0x21; k++) {
        idx = k * 2;
        if (idx >= 0x40) {
            idx -= 0x40;
        }
        c = func_0206f28c(rx, data_0209dd44[idx + 1]);
        y = func_0206f28c(ry, data_0209dd44[idx]);
        x = c;
        if (obj->angle != 0) {
            x = func_0206f28c(c, cs) - func_0206f28c(y, sn);
            y = func_0206f28c(c, sn) + func_0206f28c(y, cs);
        }
        x = x * 3 >> 2;
        px = x + cx;
        py = y + cy;
        if (obj->lim[0] != 0 && px > tmp[0]) {
            py = func_0206f28c(y, func_0206ef28(tmp[0] - cx, x));
            py += cy;
            px = tmp[0];
        }
        if (obj->lim[1] != 0 && py < tmp[1]) {
            px = func_0206f28c(x, func_0206ef28(tmp[1] - cy, y));
            px += cx;
            py = tmp[1];
        }
        if (obj->lim[2] != 0 && px < tmp[2]) {
            py = func_0206f28c(y, func_0206ef28(tmp[2] - cx, x));
            py += cy;
            px = tmp[2];
        }
        if (obj->lim[3] != 0 && py > tmp[3]) {
            px = func_0206f28c(x, func_0206ef28(tmp[3] - cy, y));
            px += cx;
            py = tmp[3];
        }
        obj->segs[k].x = ((px & 0x80000000) >> 16) | (px & 0x7fff);
        obj->segs[k].cx = ((cx & 0x80000000) >> 16) | (cx & 0x7fff);
        obj->segs[k].y = ((py & 0x80000000) >> 16) | (py & 0x7fff);
        obj->segs[k].cy = ((cy & 0x80000000) >> 16) | (cy & 0x7fff);
    }
    depth = func_02029708(func_0202970c());
    func_02061fc4();
    func_02061fd0(0, 0, -(depth - obj->depth));
    func_020571e4(obj);
    func_02061fe0(2);
    func_02061fec(0);
    for (k = 0; k < 0x21; k++) {
        if (obj->outline != 0) {
            func_02061ff8(0, 0x1f000 - obj->alpha);
        }
        func_02062018(obj->segs[k].x, obj->segs[k].y, 0);
        if (obj->outline != 0) {
            func_02061ff8(0, 0x3e000);
        }
        func_02062018(obj->segs[k].cx, obj->segs[k].cy, 0);
    }
    func_02062034();
}
