struct Point {
    unsigned char pad0;
    unsigned char color;
    char pad2[2];
    int x;
    int y;
    int pad0c;
    int size;
};

struct Field {
    char pad0[0x42];
    unsigned short base_y;
    char pad44;
    signed char alpha;
    char pad46[0x1e];
    int poly_id;
    char pad68[0x50];
    struct Point *points;
    char padbc[8];
    unsigned char half;
};

extern void *func_02008444(void);
extern int func_0202c394(void *object);
extern int func_0203435c(void *object);
extern int func_02010b54(struct Field *field);
extern void *func_0202970c(void);
extern int func_02029708(void *object);
extern int func_0200f5c8(void *object);
extern int func_0200f5e8(void *object);
extern void func_02048734(void);
extern void func_02048740(int first, int second, int third);
extern void func_02048750(int format, int gen, int s, int t, int repeat, int flip, int color0, unsigned int address);
extern void func_0204878c(int a, int b, int c, int d, int e, int f);
extern void func_020487b0(int value);
extern void *func_0200eb78(void);
extern int func_02022ca4(void *owner, int value);
extern void func_020487bc(int value);
extern void func_020487c8(short x, short y, short z);
extern void func_020487e4(void);

void func_020485c8(struct Field *field)
{
    unsigned int i;
    unsigned int count;
    int depth;
    int x;
    int y;
    struct Point *point;

    if (func_0202c394(func_02008444()) != 0 || func_0203435c(func_02008444()) != 0 || func_02010b54(field) == 0) {
        return;
    }
    count = 0x80;
    if (field->half != 0) {
        count = 0x40;
    }
    depth = -(func_02029708(func_0202970c()) - field->base_y);
    for (i = 0; i < count; i++) {
        x = field->points[i].x - func_0200f5c8(func_02008444()) - 0x80000;
        y = -field->points[i].y + func_0200f5e8(func_02008444());
        func_02048734();
        func_02048740(x >> 7, ((y + 0x60000) / 3) >> 5, depth);
        func_02048750(0, 0, 0, 0, 0, 0, 0, 0);
        func_0204878c(0, 0, 3, field->alpha, field->points[i].color, field->poly_id);
        func_020487b0(1);
        func_020487bc(func_02022ca4(func_0200eb78(), 0x7fff));
        func_020487c8(-16, field->points[i].size >> 8, 0);
        func_020487c8(-16, -(field->points[i].size >> 8), 0);
        func_020487c8(16, -(field->points[i].size >> 8), 0);
        func_020487c8(16, field->points[i].size >> 8, 0);
        func_020487e4();
    }
}
