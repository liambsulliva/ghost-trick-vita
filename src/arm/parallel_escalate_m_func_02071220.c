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

void func_02071220(int bg)
{
    data_020d0e84.lcdc = (unsigned short)(~bg & (data_020d0e84.lcdc | data_020d0e84.bg));
    data_020d0e84.bg = (unsigned short)bg;

    switch (bg) {
    case 0x08:
        *(volatile unsigned char *)0x04000243 = 0x81;
        break;
    case 0x0c:
        *(volatile unsigned char *)0x04000243 = 0x89;
    case 0x04:
        *(volatile unsigned char *)0x04000242 = 0x81;
        break;
    case 0x0e:
        *(volatile unsigned char *)0x04000243 = 0x91;
    case 0x06:
        *(volatile unsigned char *)0x04000242 = 0x89;
    case 0x02:
        *(volatile unsigned char *)0x04000241 = 0x81;
        break;
    case 0x0f:
        *(volatile unsigned char *)0x04000243 = 0x99;
    case 0x07:
        *(volatile unsigned char *)0x04000242 = 0x91;
    case 0x03:
        *(volatile unsigned char *)0x04000241 = 0x89;
    case 0x01:
        *(volatile unsigned char *)0x04000240 = 0x81;
        break;
    case 0x0b:
        *(volatile unsigned char *)0x04000240 = 0x81;
        *(volatile unsigned char *)0x04000241 = 0x89;
        *(volatile unsigned char *)0x04000243 = 0x91;
        break;
    case 0x0d:
        *(volatile unsigned char *)0x04000243 = 0x91;
    case 0x05:
        *(volatile unsigned char *)0x04000240 = 0x81;
        *(volatile unsigned char *)0x04000242 = 0x89;
        break;
    case 0x09:
        *(volatile unsigned char *)0x04000240 = 0x81;
        *(volatile unsigned char *)0x04000243 = 0x89;
        break;
    case 0x0a:
        *(volatile unsigned char *)0x04000241 = 0x81;
        *(volatile unsigned char *)0x04000243 = 0x89;
        break;
    case 0x70:
        *(volatile unsigned char *)0x04000246 = 0x99;
    case 0x30:
        *(volatile unsigned char *)0x04000245 = 0x91;
    case 0x10:
        *(volatile unsigned char *)0x04000244 = 0x81;
        break;
    case 0x50:
        *(volatile unsigned char *)0x04000246 = 0x91;
        *(volatile unsigned char *)0x04000244 = 0x81;
        break;
    case 0x60:
        *(volatile unsigned char *)0x04000246 = 0x89;
    case 0x20:
        *(volatile unsigned char *)0x04000245 = 0x81;
        break;
    case 0x40:
        *(volatile unsigned char *)0x04000246 = 0x81;
        break;
    case 0:
        break;
    default:
        break;
    }
    func_02071188(data_020d0e84.lcdc);
}
