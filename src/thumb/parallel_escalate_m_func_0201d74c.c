extern void func_0201d6c8(void *object, int value);

void func_0201d74c(int *param_1)
{
    int size;
    unsigned int i;
    char *entry;

    *(unsigned short *)((char *)param_1 + 0x16) = 0xffff;
    size = 0x1d4c;
    param_1[0] = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = 0;
    *(unsigned short *)((char *)param_1 + 0x14) = 0;
    *(unsigned char *)((char *)param_1 + 0x18) = 0;
    *(unsigned char *)((char *)param_1 + 0x19) = 0;
    func_0201d6c8((char *)param_1 + 0x1a, size);
    i = 0;
    do {
        entry = (char *)param_1 + i * 0x14;
        *(int *)(entry + size + 0x1c) = 0;
        *(int *)(entry + size + 0x20) = 0;
        *(int *)(entry + size + 0x24) = 0;
        *(unsigned short *)(entry + size + 0x28) = 0;
        *(unsigned short *)(entry + size + 0x2a) = 0xffff;
        *(unsigned short *)(entry + size + 0x2c) = 0;
        i++;
    } while (i < 0x1e);
}
