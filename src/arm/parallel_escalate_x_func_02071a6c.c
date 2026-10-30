typedef struct {
    unsigned short lcdc;
    unsigned short other[10];
    unsigned short subObj;
    unsigned short subObjExtPltt;
} VRamMng;

extern VRamMng data_020d0e84;
void func_02071188(unsigned int param_1);

void func_02071a6c(int bank)
{
    data_020d0e84.lcdc = (unsigned short)(~bank & (data_020d0e84.lcdc | data_020d0e84.subObj));
    data_020d0e84.subObj = (unsigned short)bank;
    switch (bank) {
    case 0x80:
        *(volatile unsigned int *)0x04001000 |= 0x40000000;
        *(volatile unsigned char *)0x04000248 = 0x82;
        break;
    case 0:
        *(volatile unsigned int *)0x04001000 &= ~0x40000000;
        break;
    default:
        break;
    }
    func_02071188(data_020d0e84.lcdc);
}
