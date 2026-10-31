typedef unsigned int u32;
typedef unsigned short u16;
typedef volatile u32 vu32;
typedef volatile u16 vu16;

extern u32 func_02075c98(void);
extern u32 func_02075cac(u32 bits);

void func_01ff80f4(u32 dmaNo, u32 src, u32 dest, u32 ctrl, u32 mode)
{
    u32 enabled;
    vu32 *p;

    if ((mode & 1) == 0) {
        enabled = func_02075c98();
    }
    p = (vu32 *)(0x040000b0 + dmaNo * 12);
    if ((mode & 0x10) != 0) {
        *(vu32 *)(0x040000e0 + dmaNo * 4) = src;
        src = 0x040000e0 + dmaNo * 4;
    } else if ((mode & 0x20) != 0) {
        *(vu16 *)(0x040000e0 + dmaNo * 4) = (u16)src;
        src = 0x040000e0 + dmaNo * 4;
    }
    p[0] = src;
    p[1] = dest;
    p[2] = ctrl;
    if ((mode & 2) != 0) {
        u32 dummy;
        dummy = *(vu32 *)0x040000b0;
        dummy = *(vu32 *)0x040000b0;
        if ((mode & 4) == 0 && dmaNo == 0) {
            p[0] = 0;
            p[1] = 0;
            p[2] = 0x81400001;
        }
    }
    if ((mode & 1) == 0) {
        func_02075cac(enabled);
    }
    if ((mode & 2) != 0) {
        u32 dummy;
        dummy = *(vu32 *)0x040000b0;
        dummy = *(vu32 *)0x040000b0;
    }
}
