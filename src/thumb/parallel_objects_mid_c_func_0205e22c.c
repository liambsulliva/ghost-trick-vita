struct Triple {
    unsigned short a;
    unsigned short b;
    unsigned short c;
};

extern char data_0209dacc;

void func_0205e22c(int param_1, unsigned char param_2)
{
    struct Triple local;

    *(unsigned char *)(param_1 + 0x4e) = param_2;
    local = *(struct Triple *)&data_0209dacc;
    *(unsigned short *)(param_1 + 0x48) = ((unsigned short *)&local)[*(unsigned char *)(param_1 + 0x4e)];
}
