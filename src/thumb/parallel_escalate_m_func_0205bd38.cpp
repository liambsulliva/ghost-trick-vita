extern "C" {
extern unsigned char data_020cff60[];
extern void func_02008398(int *, unsigned int, unsigned int, unsigned int);

void func_0205bd38(unsigned char *p)
{
    int loc1[3];
    int loc2[3];
    int loc3[3];
    int obj;
    int sum;

    switch (p[0x10]) {
    case 1:
        if (*(int *)(p + 0x28) != 0) {
            *(unsigned short *)(p + 0x2c) = (unsigned short)(p[0x11] + ((p[0x13] & 0xf) << 8));
            *(unsigned short *)(p + 0x2e) = (unsigned short)(p[0x12] + 0xc0 * (p[0x13] >> 4));
            func_02008398(loc1, *(unsigned short *)(p + 0x2c) << 12, *(unsigned short *)(p + 0x2e) << 12, 0);
            obj = *(int *)(p + 0x28);
            (*(void (**)(int, int *, unsigned int, unsigned int))(*(int *)obj + 0xb8))(obj, loc1, 0, 0x1f000);
        }
        p[0x10] = 0;
        break;
    case 2:
        if (*(int *)(p + 0x28) != 0) {
            *(int *)(p + 0x30) = p[0x11];
            func_02008398(loc2, *(unsigned short *)(p + 0x2c) << 12, *(unsigned short *)(p + 0x2e) << 12, 0);
            obj = *(int *)(p + 0x28);
            (*(void (**)(int, int *, unsigned int, unsigned int))(*(int *)obj + 0xb8))(obj, loc2, *(int *)(p + 0x30) << 6, 0x1f000);
        }
        p[0x10] = p[0x10] + 1;
        break;
    case 3:
        if (*(int *)(p + 0x28) != 0) {
            func_02008398(loc3, *(unsigned short *)(p + 0x2c) << 12, *(unsigned short *)(p + 0x2e) << 12, 0);
            obj = *(int *)(p + 0x28);
            (*(void (**)(int, int *, unsigned int, unsigned int))(*(int *)obj + 0xb8))(obj, loc3, *(int *)(p + 0x30) << 6, 0x1f000);
        }
        sum = *(int *)(p + 0x30) + p[0x12] * (*(unsigned char *)((int)data_020cff60 + 1) + 1);
        *(int *)(p + 0x30) = sum;
        p[0x12] = (unsigned char)(p[0x12] + (unsigned char)(p[0x13] * (*(unsigned char *)((int)data_020cff60 + 1) + 1)));
        if ((unsigned int)(sum << 6) >= 0x6000) p[0x10] = 0;
        break;
    case 4:
        obj = *(int *)(p + 0x28);
        (*(void (**)(int, int))(*(int *)obj + 0x20))(obj, 0);
        *(int *)(p + 0x28) = 0;
        p[0x10] = 0;
        break;
    }
}
}
