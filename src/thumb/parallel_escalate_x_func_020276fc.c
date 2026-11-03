typedef struct Vec276fc {
    int x;
    int y;
    int z;
} Vec276fc;

extern void *func_020083a0(void *obj);
extern void func_0205958c(Vec276fc *dst, void *obj, int index);
extern void func_02008378(Vec276fc *dst, void *mtx, Vec276fc *src);
extern int func_02008350(void *obj);
extern void func_02052ad8(int a, int x, int y, int d, int e, int f, void *obj, int index, short g, unsigned short h, int flip, short j, short k);

void func_020276fc(unsigned short a, int x, int y, unsigned short d, int e, int f, void *obj, int index, short g, unsigned short h, int flip, short j, unsigned short use, short k)
{
    void *mtx;
    Vec276fc pos;
    Vec276fc local;

    if (obj != 0 && index >= 0 && use != 0) {
        mtx = func_020083a0(obj);
        func_0205958c(&local, obj, index);
        func_02008378(&pos, mtx, &local);
        if (func_02008350(obj) != 0) {
            pos.x = pos.x - x;
        } else {
            pos.x = pos.x + x;
        }
        pos.y = pos.y + y;
        if (func_02008350(obj) != 0 && index < 0) {
            flip = !flip;
        }
        func_02052ad8(a, pos.x, pos.y, d, e, f, 0, index, g, h, flip, j, k);
    } else {
        func_02052ad8(a, x, y, d, e, f, obj, index, g, h, flip, j, k);
    }
}
