typedef struct Sprite29030 {
    short x;
    short y;
    char pad4[0x20];
    int attr;
} Sprite29030;

typedef struct Group29030 {
    unsigned int count;
    unsigned int texture;
    void *data;
    int padc;
    Sprite29030 *sprites;
    void *owner;
    char pad18[0x20];
    unsigned char flip;
    unsigned char visible;
    unsigned char ready;
    unsigned char translucent;
    short left;
    short top;
    short right;
    short bottom;
} Group29030;

typedef struct Mtx29030 {
    int m[9];
    int x;
    int y;
    int z;
} Mtx29030;

typedef struct Vec29030 {
    int x;
    int y;
    int z;
} Vec29030;

extern Mtx29030 data_020a4d28;
extern void *func_0200eb78(void);
extern int func_02022ca4(void *owner, int value);
extern void func_020292b0(int value);
extern void *func_02008444(void);
extern int func_0200f5c8(void *camera);
extern int func_0200f5e8(void *camera);
extern void *func_020083a0(void *object);
extern void func_020082d8(Vec29030 *dst, void *src);
extern void func_020292bc(int a, int b, int c, int d, int e, int f, int g, int h);
extern void func_020292f8(unsigned int texture, int mode);
extern void func_02029314(int a, int b, int c, int d, int e, int f);
extern void func_0206f77c(Mtx29030 *m);
extern void func_02029338(void);
extern void func_02029344(int x, int y, int z);
extern void func_02029354(int x, int y, int z);
extern void func_02029364(int mode);
extern void func_02029370(int s, int t);
extern void func_02029390(int x, int y, int z);
extern void func_020293ac(void);

void func_02029030(Group29030 *g, int depth, int color, int image, int scale, unsigned short base, unsigned char fixed, int fixed_x, int fixed_y)
{
    Vec29030 pos;
    int width;
    int height;
    int offset_x;
    int offset_y;
    int half_w;
    int half_h;
    int cam_x;
    int cam_y;
    int y;
    int z;
    int size;
    int x;
    unsigned int i;
    Sprite29030 *s;

    if (g->visible == 0 || g->ready == 0 || g->data == 0 || g->sprites == 0) {
        return;
    }
    func_020292b0(func_02022ca4(func_0200eb78(), color));
    width = g->left + g->right;
    offset_x = width << 11;
    height = g->top + g->bottom;
    offset_y = height << 11;
    half_w = scale * width >> 1;
    half_h = scale * height >> 1;
    cam_x = func_0200f5c8(func_02008444()) + 0x80000;
    cam_y = func_0200f5e8(func_02008444()) + 0x60000;
    if (fixed) {
        cam_x = fixed_x;
        cam_y = fixed_y;
    }
    func_020082d8(&pos, func_020083a0(g->owner));
    z = -(depth - base);
    size = scale * 8;
    for (i = 0; i < g->count; i++) {
        s = &g->sprites[i];
        x = offset_x + (scale * s->x - half_w);
        x = x + pos.x - cam_x;
        y = offset_y + (scale * s->y - half_h);
        y = cam_y - (y + pos.y);
        if (x > 0x88000 || x < -0x88000 || y > 0x68000 || y < -0x68000) {
            continue;
        }
        if (g->translucent) {
            func_020292bc(2, 1, 0, 0, 3, 0, 1, s->attr);
            func_020292f8(g->texture, 2);
        } else {
            func_020292bc(3, 1, 0, 0, 3, 0, 1, s->attr);
            func_020292f8(g->texture, 3);
        }
        func_02029314(0, 0, 3, 0x31, image, 0);
        if (scale == 0x1000) {
            data_020a4d28.x = x >> 7;
            data_020a4d28.y = (y / 3) >> 5;
            data_020a4d28.z = z;
            func_0206f77c((Mtx29030 *)0x020a4d28);
        } else {
            func_02029338();
            func_02029344(x >> 7, (y / 3) >> 5, z);
            func_02029354(size, size, 0x1000);
        }
        func_02029364(1);
        if (g->flip) {
            func_02029370(0x7000, 0);
            func_02029390(0, 0, 0);
            func_02029370(0x7000, 0x8000);
            func_02029390(0, -0x2c, 0);
            func_02029370(-0x1000, 0x8000);
            func_02029390(0x20, -0x2c, 0);
            func_02029370(-0x1000, 0);
            func_02029390(0x20, 0, 0);
        } else {
            func_02029370(0, 0);
            func_02029390(0, 0, 0);
            func_02029370(0, 0x8000);
            func_02029390(0, -0x2c, 0);
            func_02029370(0x8000, 0x8000);
            func_02029390(0x20, -0x2c, 0);
            func_02029370(0x8000, 0);
            func_02029390(0x20, 0, 0);
        }
        func_020293ac();
    }
}
