typedef struct {
    unsigned short lcdc;
    unsigned short other[9];
    unsigned short bank20;
} VRamMng020719fc;

extern VRamMng020719fc data_020d0e84;
void func_02071188(unsigned int param_1);

void func_020719fc(int bank)
{
    data_020d0e84.lcdc = (unsigned short)(~bank & (data_020d0e84.lcdc | data_020d0e84.bank20));
    data_020d0e84.bank20 = (unsigned short)bank;
    switch (bank) {
    case 0:
        break;
    case 8:
        *(volatile unsigned char *)0x04000243 = 0x84;
        break;
    case 0x100:
        *(volatile unsigned char *)0x04000249 = 0x82;
        break;
    default:
        break;
    }
    func_02071188(data_020d0e84.lcdc);
}
