extern unsigned int func_020176b4(void);
extern unsigned int func_020176c8(void);

void func_0201767c(char *param_1)
{
    unsigned int mode;
    int size;

    if (*(unsigned char *)(param_1 + 0x22) == 1) {
        mode = func_020176b4();
    } else {
        mode = func_020176c8();
    }
    switch (mode) {
    case 0x10:
        *(unsigned short *)(param_1 + 0x24) = *(unsigned short *)(param_1 + 0x26) << 2;
        return;
    case 0x200010:
        size = *(unsigned short *)(param_1 + 0x26) << 2;
        *(unsigned short *)(param_1 + 0x24) = (unsigned int)size >> 2;
        return;
    }
}
