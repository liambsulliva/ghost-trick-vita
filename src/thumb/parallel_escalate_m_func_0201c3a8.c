extern void func_0201c350(char *object);
extern unsigned short *func_0201c478(char *object);
extern unsigned int func_0201c47c(char *object);
extern void func_0201c04c(int target, signed char level);

void func_0201c3a8(char *object)
{
    unsigned short *pal;
    unsigned int k;
    unsigned int i;
    unsigned short v;
    unsigned short lo;
    unsigned short mid;
    unsigned short hi;
    unsigned short lo2;
    unsigned short mid2;
    unsigned short hi2;

    func_0201c350(object);
    pal = func_0201c478(object);
    if (pal != 0) {
        k = func_0201c47c(object);
        i = 1;
        do {
            v = pal[i];
            lo = (v & 0x1f) << 3;
            lo = lo + (((0xff - lo) * k) >> 4);
            mid = ((v & 0x3e0) >> 5) << 3;
            mid = mid + (((0xff - mid) * k) >> 4);
            hi = ((v & 0x7c00) >> 10) << 3;
            hi = hi + (((0xff - hi) * k) >> 4);
            lo2 = lo > 0xff ? 0xff : lo;
            mid2 = mid > 0xff ? 0xff : mid;
            hi2 = hi > 0xff ? 0xff : hi;
            pal[i] = (hi2 >> 3) << 10 | ((lo2 >> 3) | (mid2 >> 3) << 5);
            i++;
        } while (i < 0x10);
        if (*(int *)(object + 0x28) != 0) {
            func_0201c04c(*(int *)(object + 0x28), (signed char)(0x1f - ((k * 0x1f) >> 4)));
        }
    }
}
