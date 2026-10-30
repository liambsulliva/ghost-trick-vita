extern char *data_020a5574;
extern void func_020301d0(void *object, int value, int extra);

void func_0203018c(void)
{
    int offset;

    func_020301d0(data_020a5574 + 0x120,0x1000,0x800);
    func_020301d0(data_020a5574 + 0x920,0x1000,0x800);
    offset = 0x1120;
    *(unsigned char *)(data_020a5574 + offset) = 1;
    *(unsigned char *)(data_020a5574 + offset + 1) = 1;
}
