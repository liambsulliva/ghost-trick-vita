extern unsigned short data_020d0e46;
extern void func_02075f00(unsigned short bits, unsigned short mask);

int func_02071c64(unsigned short *mask)
{
    int bits;

    bits = *mask;
    *mask = 0;
    if ((bits & 0x1) != 0) {
        *(volatile unsigned char *)0x04000240 = 0;
    }
    if ((bits & 0x2) != 0) {
        *(volatile unsigned char *)0x04000241 = 0;
    }
    if ((bits & 0x4) != 0) {
        *(volatile unsigned char *)0x04000242 = 0;
    }
    if ((bits & 0x8) != 0) {
        *(volatile unsigned char *)0x04000243 = 0;
    }
    if ((bits & 0x10) != 0) {
        *(volatile unsigned char *)0x04000244 = 0;
    }
    if ((bits & 0x20) != 0) {
        *(volatile unsigned char *)0x04000245 = 0;
    }
    if ((bits & 0x40) != 0) {
        *(volatile unsigned char *)0x04000246 = 0;
    }
    if ((bits & 0x80) != 0) {
        *(volatile unsigned char *)0x04000248 = 0;
    }
    if ((bits & 0x100) != 0) {
        *(volatile unsigned char *)0x04000249 = 0;
    }
    func_02075f00(bits, data_020d0e46);
    return bits;
}
