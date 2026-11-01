typedef struct {
    unsigned char pad0[0x36];
    volatile unsigned short state;
    volatile unsigned short err_flg;
    volatile unsigned short command_flg;
} TPStatus;

extern TPStatus data_020d2e84;

void func_02077fcc(unsigned int command_flgs)
{
    while (data_020d2e84.command_flg & command_flgs) {
    }
}
