struct VramBank {
    unsigned short lcdc;
    unsigned short bg;
    unsigned short obj;
    unsigned short arm7;
    unsigned short tex;
    unsigned short texPltt;
    unsigned short clearImage;
    unsigned short bgExtPltt;
    unsigned short objExtPltt;
};

extern struct VramBank data_020d0e84;
extern void func_02071188(unsigned int lcdc);

void func_0207164c(int objExtPltt)
{
    data_020d0e84.lcdc = (unsigned short)(~objExtPltt & (data_020d0e84.lcdc | data_020d0e84.objExtPltt));
    data_020d0e84.objExtPltt = (unsigned short)objExtPltt;
    switch (objExtPltt) {
    case 0x20:
        *(volatile unsigned int *)0x04000000 |= 0x80000000;
        *(volatile unsigned char *)0x04000245 = 0x85;
        break;
    case 0x40:
        *(volatile unsigned int *)0x04000000 |= 0x80000000;
        *(volatile unsigned char *)0x04000246 = 0x85;
        break;
    case 0:
        *(volatile unsigned int *)0x04000000 &= ~0x80000000;
        break;
    default:
        break;
    }
    func_02071188(data_020d0e84.lcdc);
}
