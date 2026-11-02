struct Vec {
    int x;
    int y;
    int z;
};

struct Pair {
    int v[2];
};

struct Point {
    short x;
    short y;
    short u;
    short v;
};

struct Ribbon {
    char pad00[0x28];
    struct Vec pos;
    char pad34[0x48];
    struct Vec target;
    int width;
    int scale;
    char pad90[2];
    unsigned char active;
    char pad93[5];
    struct Point *points;
    unsigned char kind;
};

extern struct Pair data_0209dccc;
extern struct Pair data_0209dcb4;
extern struct Pair data_0209dcbc;
extern struct Pair data_0209dcc4;
extern struct Pair data_0209dcd4;
extern struct Pair data_0209dcdc;
extern int data_0209dd44[];

extern void func_02056cb8(struct Ribbon *ribbon);
extern int func_0202c358(struct Ribbon *ribbon);
extern void func_020082d8(struct Vec *dst, struct Vec *src);
extern void *func_02008444(void);
extern int func_0200f5c8(void *game);
extern int func_0200f5e8(void *game);
extern void func_020083c0(struct Vec *dst, struct Vec *a, struct Vec *b);
extern int func_0201f0dc(struct Vec *v);
extern int func_0206ef28(int a, int b);
extern void func_0201f0a0(struct Vec *dst, struct Vec *src);
extern int func_0206f28c(int a, int b);

void func_0206363c(struct Ribbon *ribbon)
{
    struct Vec start;
    struct Vec end;
    struct Vec diff;
    struct Vec normal;
    struct Pair a;
    struct Pair b;
    struct Pair c;
    struct Pair d;
    struct Pair e;
    struct Pair f;
    int turn;
    int cosine;
    int dir_x;
    int dir_y;
    int base_x;
    int base_y;
    int step_x;
    int step_y;
    int inner;
    int px;
    int width;
    int len;
    int sine;
    int rot;
    int py;
    int i;
    int angle;

    func_02056cb8(ribbon);
    if (func_0202c358(ribbon) == 0 || ribbon->active == 0) {
        return;
    }
    a = data_0209dccc;
    b = data_0209dcb4;
    c = data_0209dcbc;
    d = data_0209dcc4;
    e = data_0209dcd4;
    f = data_0209dcdc;
    func_020082d8(&start, &ribbon->pos);
    func_020082d8(&end, &ribbon->target);
    start.x = start.x - func_0200f5c8(func_02008444()) - 0x80000;
    start.y = -start.y + func_0200f5e8(func_02008444()) + 0x60000;
    end.x = end.x - func_0200f5c8(func_02008444()) - 0x80000;
    end.y = -end.y + func_0200f5e8(func_02008444()) + 0x60000;
    func_020083c0(&diff, &end, &start);
    len = func_0201f0dc(&diff);
    dir_x = func_0206ef28(diff.x, len);
    dir_y = func_0206ef28(diff.y, len);
    func_0201f0a0(&normal, &diff);
    base_x = start.x + (a.v[ribbon->kind] * normal.y + c.v[ribbon->kind] * normal.x);
    base_y = start.y + (b.v[ribbon->kind] * -normal.x + d.v[ribbon->kind] * normal.y);
    step_x = (e.v[ribbon->kind] * -normal.y) >> 4;
    step_y = (f.v[ribbon->kind] * normal.x) >> 4;
    end.x >>= 7;
    end.y = end.y / 3 >> 5;
    width = ribbon->width;
    inner = (width * 3) >> 2;
    for (i = 0; i < 17; i++) {
        angle = i * 2 + 0x30;
        if (angle >= 0x40) {
            angle -= 0x40;
        }
        cosine = func_0206f28c(data_0209dd44[angle + 1], ribbon->scale);
        sine = data_0209dd44[angle];
        rot = func_0206f28c(cosine, dir_x) - func_0206f28c(sine, dir_y);
        turn = func_0206f28c(cosine, dir_y);
        sine = func_0206f28c(sine, dir_x);
        px = end.x + func_0206f28c(inner, rot);
        py = end.y + func_0206f28c(width, turn + sine);
        ribbon->points[i].x = (base_x + step_x * i) >> 7;
        ribbon->points[i].u = px;
        ribbon->points[i].y = (base_y + step_y * i) / 3 >> 5;
        ribbon->points[i].v = py;
    }
}
