typedef struct Disp02023360 {
    unsigned int dispcnt;
    unsigned short bg0cnt;
    unsigned short bg1cnt;
    unsigned short bg2cnt;
    unsigned short bg3cnt;
    unsigned short bg0hofs;
    unsigned short bg0vofs;
    unsigned short bg1hofs;
    unsigned short bg1vofs;
    unsigned short bg2hofs;
    unsigned short bg2vofs;
    unsigned short bg3hofs;
    unsigned short bg3vofs;
    unsigned short bg2pa;
    unsigned short bg2pb;
    unsigned short bg2pc;
    unsigned short bg2pd;
    unsigned int bg2x;
    unsigned int bg2y;
    unsigned short bg3pa;
    unsigned short bg3pb;
    unsigned short bg3pc;
    unsigned short bg3pd;
    unsigned int bg3x;
    unsigned int bg3y;
    unsigned short win0h;
    unsigned short win1h;
    unsigned short win0v;
    unsigned short win1v;
    unsigned short winin;
    unsigned short winout;
    unsigned short mosaic;
    unsigned short pad4a;
    unsigned short bldcnt;
    unsigned short bldalpha;
    unsigned short bldy;
    short bright;
} Disp02023360;

extern void func_020236c0(short bright);
extern void func_020236d0(Disp02023360 *dst, Disp02023360 *src);

void func_02023360(Disp02023360 *cur, Disp02023360 *old, unsigned char *force)
{
    if (cur->dispcnt != old->dispcnt || *force == 0) {
        *(volatile unsigned int *)0x04000000 = cur->dispcnt;
    }
    if (cur->bg0cnt != old->bg0cnt || *force == 0) {
        *(volatile unsigned short *)0x04000008 = cur->bg0cnt;
    }
    if (cur->bg1cnt != old->bg1cnt || *force == 0) {
        *(volatile unsigned short *)0x0400000a = cur->bg1cnt;
    }
    if (cur->bg2cnt != old->bg2cnt || *force == 0) {
        *(volatile unsigned short *)0x0400000c = cur->bg2cnt;
    }
    if (cur->bg3cnt != old->bg3cnt || *force == 0) {
        *(volatile unsigned short *)0x0400000e = cur->bg3cnt;
    }
    if (cur->bg0hofs != old->bg0hofs || *force == 0) {
        *(volatile unsigned short *)0x04000010 = cur->bg0hofs;
    }
    if (cur->bg0vofs != old->bg0vofs || *force == 0) {
        *(volatile unsigned short *)0x04000012 = cur->bg0vofs;
    }
    if (cur->bg1hofs != old->bg1hofs || *force == 0) {
        *(volatile unsigned short *)0x04000014 = cur->bg1hofs;
    }
    if (cur->bg1vofs != old->bg1vofs || *force == 0) {
        *(volatile unsigned short *)0x04000016 = cur->bg1vofs;
    }
    if (cur->bg2hofs != old->bg2hofs || *force == 0) {
        *(volatile unsigned short *)0x04000018 = cur->bg2hofs;
    }
    if (cur->bg2vofs != old->bg2vofs || *force == 0) {
        *(volatile unsigned short *)0x0400001a = cur->bg2vofs;
    }
    if (cur->bg3hofs != old->bg3hofs || *force == 0) {
        *(volatile unsigned short *)0x0400001c = cur->bg3hofs;
    }
    if (cur->bg3vofs != old->bg3vofs || *force == 0) {
        *(volatile unsigned short *)0x0400001e = cur->bg3vofs;
    }
    if (cur->bg2pa != old->bg2pa || *force == 0) {
        *(volatile unsigned short *)0x04000020 = cur->bg2pa;
    }
    if (cur->bg2pb != old->bg2pb || *force == 0) {
        *(volatile unsigned short *)0x04000022 = cur->bg2pb;
    }
    if (cur->bg2pc != old->bg2pc || *force == 0) {
        *(volatile unsigned short *)0x04000024 = cur->bg2pc;
    }
    if (cur->bg2pd != old->bg2pd || *force == 0) {
        *(volatile unsigned short *)0x04000026 = cur->bg2pd;
    }
    if (cur->bg2x != old->bg2x || *force == 0) {
        *(volatile unsigned int *)0x04000028 = cur->bg2x;
    }
    if (cur->bg2y != old->bg2y || *force == 0) {
        *(volatile unsigned int *)0x0400002c = cur->bg2y;
    }
    if (cur->bg3pa != old->bg3pa || *force == 0) {
        *(volatile unsigned short *)0x04000030 = cur->bg3pa;
    }
    if (cur->bg3pb != old->bg3pb || *force == 0) {
        *(volatile unsigned short *)0x04000032 = cur->bg3pb;
    }
    if (cur->bg3pc != old->bg3pc || *force == 0) {
        *(volatile unsigned short *)0x04000034 = cur->bg3pc;
    }
    if (cur->bg3pd != old->bg3pd || *force == 0) {
        *(volatile unsigned short *)0x04000036 = cur->bg3pd;
    }
    if (cur->bg3x != old->bg3x || *force == 0) {
        *(volatile unsigned int *)0x04000038 = cur->bg3x;
    }
    if (cur->bg3y != old->bg3y || *force == 0) {
        *(volatile unsigned int *)0x0400003c = cur->bg3y;
    }
    if (cur->win0h != old->win0h || *force == 0) {
        *(volatile unsigned short *)0x04000040 = cur->win0h;
    }
    if (cur->win1h != old->win1h || *force == 0) {
        *(volatile unsigned short *)0x04000042 = cur->win1h;
    }
    if (cur->win0v != old->win0v || *force == 0) {
        *(volatile unsigned short *)0x04000044 = cur->win0v;
    }
    if (cur->win1v != old->win1v || *force == 0) {
        *(volatile unsigned short *)0x04000046 = cur->win1v;
    }
    if (cur->winin != old->winin || *force == 0) {
        *(volatile unsigned short *)0x04000048 = cur->winin;
    }
    if (cur->winout != old->winout || *force == 0) {
        *(volatile unsigned short *)0x0400004a = cur->winout;
    }
    if (cur->bldcnt != old->bldcnt || *force == 0) {
        *(volatile unsigned short *)0x04000050 = cur->bldcnt;
    }
    if (cur->mosaic != old->mosaic || *force == 0) {
        *(volatile unsigned short *)0x0400004c = cur->mosaic;
    }
    if (cur->bldalpha != old->bldalpha || *force == 0) {
        *(volatile unsigned short *)0x04000052 = cur->bldalpha;
    }
    if (cur->bldy != old->bldy || *force == 0) {
        *(volatile unsigned short *)0x04000054 = cur->bldy;
    }
    func_020236c0(cur->bright);
    func_020236d0(old, cur);
    *force = 1;
}
