typedef struct {
    int bank0;
    int pad[2];
    unsigned int offset;
    void *addr;
    int bank;
} ExtPltt;

extern ExtPltt data_020d0e48;
int func_02071ba4(void);

void func_02070b18(void)
{
    int bank;

    bank = func_02071ba4();
    data_020d0e48.bank = bank;
    switch (bank) {
    case 0:
        break;
    case 16:
        data_020d0e48.addr = (void *)0x06880000;
        data_020d0e48.offset = 0;
        break;
    case 32:
    case 96:
        data_020d0e48.addr = (void *)0x06890000;
        data_020d0e48.offset = 0;
        break;
    case 64:
        data_020d0e48.addr = (void *)0x06894000;
        data_020d0e48.offset = 0x4000;
        break;
    default:
        break;
    }
}
