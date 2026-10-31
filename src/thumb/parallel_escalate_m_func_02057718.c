extern int func_0202c374(void *map);
extern int func_0202c36c(void *map);
extern unsigned char *func_02043fa0(void *map);
extern int func_0206ef28(int a, int b);
extern int func_0206f28c(int a, int b);

int func_02057718(unsigned char *p, int x, int y)
{
    int px;
    int py;
    int left;
    int top;
    int right;
    int bottom;
    int base;
    int ix;
    int iy;
    unsigned char *tbl;
    unsigned char v;
    unsigned char n;
    int stride;

    px = (x - 0x80) * 0x1000;
    py = (0x60 - y) * 0x1000;
    left = *(short *)(p + 0x7c) * 0x80;
    top = *(short *)(p + 0x7e) * 0x80;
    right = *(short *)(p + 0x8c) * 0x80;
    bottom = *(short *)(p + 0x8e) * 0x80;
    if (*(void **)(p + 0x48) == 0 ||
        (func_0202c374(*(void **)(p + 0x48)) != 3 && func_0202c374(*(void **)(p + 0x48)) != 4)) {
        px = px - *(int *)(p + 0x28);
        py = py - *(int *)(p + 0x2c);
        if (px < left) {
            return 0;
        }
        if (px > right) {
            return 0;
        }
        if (py < bottom) {
            return 0;
        }
        if (py > top) {
            return 0;
        }
        return 1;
    }
    px = func_0206ef28(px - left - *(int *)(p + 0x28), right - left);
    base = *(int *)(p + 0xa4);
    px = base + func_0206f28c(px, *(int *)(p + 0xb4) - base);
    ix = px >> 12;
    if (px < base) {
        return 0;
    }
    if (px >= *(int *)(p + 0xb4)) {
        return 0;
    }
    py = func_0206ef28(py - top - *(int *)(p + 0x2c), bottom - top);
    base = *(int *)(p + 0xa8);
    py = base + func_0206f28c(py, *(int *)(p + 0xb8) - base);
    iy = py >> 12;
    if (py < base) {
        return 0;
    }
    if (py >= *(int *)(p + 0xb8)) {
        return 0;
    }
    tbl = func_02043fa0(*(void **)(p + 0x48));
    if (func_0202c374(*(void **)(p + 0x48)) == 3) {
        stride = 4 << func_0202c36c(*(void **)(p + 0x48));
        v = tbl[(ix >> 1) + stride * iy];
        if (ix & 1) {
            n = v >> 4;
        } else {
            n = v & 0xf;
        }
        if (n == 0) {
            return 0;
        }
        return 1;
    }
    stride = 8 << func_0202c36c(*(void **)(p + 0x48));
    if (tbl[ix + stride * iy] != 0) {
        return 1;
    }
    return 0;
}
