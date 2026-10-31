typedef struct {
    unsigned short lcdc;
    unsigned short bg;
    unsigned short obj;
    unsigned short arm7;
    unsigned short tex;
    unsigned short texPltt;
    unsigned short clrImg;
    unsigned short bgExtPltt;
    unsigned short objExtPltt;
    unsigned short subBg;
    unsigned short subObj;
    unsigned short subBgExtPltt;
    unsigned short subObjExtPltt;
} VRamCnt;

extern VRamCnt data_020d0e84;
extern void func_02071188(int lcdc);

void func_02071554(int bgExtPltt)
{
    data_020d0e84.lcdc = (unsigned short)(~bgExtPltt & (data_020d0e84.lcdc | data_020d0e84.bgExtPltt));
    data_020d0e84.bgExtPltt = (unsigned short)bgExtPltt;

    switch (bgExtPltt) {
    case 0x10:
        *(volatile unsigned int *)0x04000000 |= 0x40000000;
        *(volatile unsigned char *)0x04000244 = 0x84;
        break;
    case 0x40:
        *(volatile unsigned int *)0x04000000 |= 0x40000000;
        *(volatile unsigned char *)0x04000246 = 0x8c;
        break;
    case 0x60:
        *(volatile unsigned char *)0x04000246 = 0x8c;
    case 0x20:
        *(volatile unsigned char *)0x04000245 = 0x84;
        *(volatile unsigned int *)0x04000000 |= 0x40000000;
        break;
    case 0:
        *(volatile unsigned int *)0x04000000 &= ~0x40000000;
        break;
    default:
        break;
    }
    func_02071188(data_020d0e84.lcdc);
}
