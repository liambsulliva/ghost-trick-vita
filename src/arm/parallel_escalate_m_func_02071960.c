typedef struct {
    unsigned short lcdc;
    unsigned short other[8];
    unsigned short subBg;
} VRamMng02071960;

extern VRamMng02071960 data_020d0e84;
void func_02071188(unsigned int param_1);

void func_02071960(int bank)
{
    data_020d0e84.lcdc = (unsigned short)(~bank & (data_020d0e84.lcdc | data_020d0e84.subBg));
    data_020d0e84.subBg = (unsigned short)bank;
    switch (bank) {
    case 0:
        break;
    case 4:
        *(volatile unsigned char *)0x04000242 = 0x84;
        break;
    case 0x180:
        *(volatile unsigned char *)0x04000249 = 0x81;
    case 0x80:
        *(volatile unsigned char *)0x04000248 = 0x81;
        break;
    default:
        break;
    }
    func_02071188(data_020d0e84.lcdc);
}
