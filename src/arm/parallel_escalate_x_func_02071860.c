typedef struct {
    unsigned short lcdc;
    unsigned short other[4];
    unsigned short texPltt;
} VRamMng02071860;

extern VRamMng02071860 data_020d0e84;
void func_02071188(unsigned int param_1);

void func_02071860(int texPltt)
{
    data_020d0e84.lcdc = (unsigned short)(~texPltt & (data_020d0e84.lcdc | data_020d0e84.texPltt));
    data_020d0e84.texPltt = (unsigned short)texPltt;
    switch (texPltt) {
    case 0:
        break;
    case 0x60:
        *(volatile unsigned char *)0x04000246 = 0x8b;
    case 0x20:
        *(volatile unsigned char *)0x04000245 = 0x83;
        break;
    case 0x40:
        *(volatile unsigned char *)0x04000246 = 0x83;
        break;
    case 0x70:
        *(volatile unsigned char *)0x04000246 = 0x9b;
    case 0x30:
        *(volatile unsigned char *)0x04000245 = 0x93;
    case 0x10:
        *(volatile unsigned char *)0x04000244 = 0x83;
        break;
    default:
        break;
    }
    func_02071188(data_020d0e84.lcdc);
}
