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

void func_020716e8(int tex)
{
    data_020d0e84.lcdc = (unsigned short)(~tex & (data_020d0e84.lcdc | data_020d0e84.tex));
    data_020d0e84.tex = (unsigned short)tex;

    if (tex == 0) {
        *(volatile unsigned short *)0x04000060 &= ~0x3001;
    } else {
        *(volatile unsigned short *)0x04000060 = (unsigned short)((*(volatile unsigned short *)0x04000060 & ~0x3000) | 1);
        switch (tex) {
        case 5:
            *(volatile unsigned char *)0x04000240 = 0x83;
            *(volatile unsigned char *)0x04000242 = 0x8b;
            break;
        case 9:
            *(volatile unsigned char *)0x04000240 = 0x83;
            *(volatile unsigned char *)0x04000243 = 0x8b;
            break;
        case 10:
            *(volatile unsigned char *)0x04000241 = 0x83;
            *(volatile unsigned char *)0x04000243 = 0x8b;
            break;
        case 11:
            *(volatile unsigned char *)0x04000240 = 0x83;
            *(volatile unsigned char *)0x04000241 = 0x8b;
            *(volatile unsigned char *)0x04000243 = 0x93;
            break;
        case 13:
            *(volatile unsigned char *)0x04000240 = 0x83;
            *(volatile unsigned char *)0x04000242 = 0x8b;
            *(volatile unsigned char *)0x04000243 = 0x93;
            break;
        case 8:
            *(volatile unsigned char *)0x04000243 = 0x83;
            break;
        case 12:
            *(volatile unsigned char *)0x04000243 = 0x8b;
        case 4:
            *(volatile unsigned char *)0x04000242 = 0x83;
            break;
        case 14:
            *(volatile unsigned char *)0x04000243 = 0x93;
        case 6:
            *(volatile unsigned char *)0x04000242 = 0x8b;
        case 2:
            *(volatile unsigned char *)0x04000241 = 0x83;
            break;
        case 15:
            *(volatile unsigned char *)0x04000243 = 0x9b;
        case 7:
            *(volatile unsigned char *)0x04000242 = 0x93;
        case 3:
            *(volatile unsigned char *)0x04000241 = 0x8b;
        case 1:
            *(volatile unsigned char *)0x04000240 = 0x83;
            break;
        default:
            break;
        }
    }
    func_02071188(data_020d0e84.lcdc);
}
