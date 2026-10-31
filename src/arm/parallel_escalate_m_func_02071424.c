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

void func_02071424(int obj)
{
    data_020d0e84.lcdc = (unsigned short)(~obj & (data_020d0e84.lcdc | data_020d0e84.obj));
    data_020d0e84.obj = (unsigned short)obj;

    switch (obj) {
    case 3:
        *(volatile unsigned char *)0x04000241 = 0x8a;
    case 1:
        *(volatile unsigned char *)0x04000240 = 0x82;
        break;
    case 2:
        *(volatile unsigned char *)0x04000241 = 0x82;
        break;
    case 0x70:
        *(volatile unsigned char *)0x04000246 = 0x9a;
    case 0x30:
        *(volatile unsigned char *)0x04000245 = 0x92;
    case 0x10:
        *(volatile unsigned char *)0x04000244 = 0x82;
        break;
    case 0x50:
        *(volatile unsigned char *)0x04000246 = 0x92;
        *(volatile unsigned char *)0x04000244 = 0x82;
        break;
    case 0x60:
        *(volatile unsigned char *)0x04000246 = 0x8a;
    case 0x20:
        *(volatile unsigned char *)0x04000245 = 0x82;
        break;
    case 0x40:
        *(volatile unsigned char *)0x04000246 = 0x82;
        break;
    case 0:
    default:
        break;
    }
    func_02071188(data_020d0e84.lcdc);
}
