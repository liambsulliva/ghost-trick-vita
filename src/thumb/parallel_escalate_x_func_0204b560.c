typedef struct Pt0204b560 {
    int pos;
    int vel;
    int acc;
} Pt0204b560;

typedef struct Obj0204b560 {
    char pad0[0x38];
    int level;
    char pad3c[0x7a];
    short delay;
    Pt0204b560 *pts;
    int area;
    int base;
    char padc4[4];
    unsigned char state;
} Obj0204b560;

extern char data_020ce858[];
extern char data_0209b7f4[];
extern char data_0209b878[];
extern char data_0209b880[];
extern char data_0209b888[];
extern char data_0209b894[];
extern char data_0209b810[];
extern void func_020471ec(int *vec, Obj0204b560 *self);
extern int func_020083b0();
extern int func_0200f330();
extern void *func_02008444(void);
extern void *func_02008eb0(void);
extern unsigned int func_01ff80b8(void *random);
extern int func_0200f5c8(void *camera);
extern void *func_020470bc(void *pool);
extern void func_0204b758(void *e, int *x, int *y, int *vel);
extern void *func_02009900(void);
extern int func_0202f7e8();
extern int func_0200da04();

void func_0204b560(Obj0204b560 *obj)
{
    int tmp[3];
    int x;
    int y;
    int k;
    int b;
    int lo;
    int i;
    void *e;
    int j;
    int cur;
    int prev;
    int next;
    int sum;
    int m;

    func_020471ec(tmp, obj);
    func_020083b0(&obj->area, tmp);
    if (func_0200f330(func_02008444()) != 0) {
        return;
    }
    if (obj->delay < 0) {
        obj->delay = 16;
    }
    if ((func_01ff80b8(func_02008eb0()) & 0x3f) == 0) {
        k = func_01ff80b8(func_02008eb0()) % 17;
        obj->pts[k].vel = obj->level;
        obj->state = 0;
        if (obj->level >= 0x4000) {
            b = (unsigned char)(func_0200f5c8(func_02008444()) >> 12);
            lo = b & 0xf;
            i = k - (b >> 4);
            if (i < 0) {
                i += 17;
            }
            x = ((i * 16 - lo) << 12) + func_0200f5c8(func_02008444());
            for (i = 0; i < 2; i++) {
                e = func_020470bc(data_020ce858);
                if (e != 0) {
                    y = obj->base - obj->pts[k].pos;
                    func_0204b758(e, &x, &y, &obj->pts[k].vel);
                    func_0202f7e8(func_02009900(), e, 7, 0);
                }
            }
        }
    }
    for (j = 0; j < 17; j++) {
        cur = obj->pts[j].pos;
        if (j > 0) {
            prev = obj->pts[j - 1].pos;
        } else {
            prev = obj->pts[16].pos;
        }
        if (j < 16) {
            next = obj->pts[j + 1].pos;
        } else {
            next = obj->pts[0].pos;
        }
        obj->pts[j].acc = -(obj->pts[j].pos >> 9);
        obj->pts[j].vel = obj->pts[j].vel + obj->pts[j].acc;
        obj->pts[j].vel = obj->pts[j].vel * 63 >> 6;
        cur += obj->pts[j].vel;
        obj->pts[j].pos = (prev + next + cur * 2) >> 2;
    }
    sum = 0;
    for (m = 0; m < 17; m++) {
        sum += obj->pts[m].pos;
    }
    if (obj->state < 2) {
        if (sum > obj->level * 51 >> 2) {
            obj->state = 2;
            func_0200da04(data_0209b7f4, data_0209b878, data_0209b880, data_0209b810);
        } else if (sum < obj->level * 17 >> 2 && obj->state == 1) {
            obj->state = 0;
            func_0200da04(data_0209b7f4, data_0209b878, data_0209b888, data_0209b810);
        }
    } else if (obj->state == 2) {
        if (sum < obj->level * 51 >> 2) {
            obj->state = 1;
            func_0200da04(data_0209b7f4, data_0209b878, data_0209b894, data_0209b810);
        }
    }
}
