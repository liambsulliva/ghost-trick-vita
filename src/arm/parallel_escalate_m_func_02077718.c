extern void func_020768a8(void);
extern void func_020768bc(void);

unsigned char *func_02077718(unsigned char *base, int index)
{
    unsigned int offset;
    unsigned char *result;

    func_020768a8();
    offset = *(unsigned int *)(base + index * 4 + 0x3c);
    if (offset != 0) {
        result = (unsigned char *)offset;
        if (offset < 0x2000000) {
            result = base + offset;
        }
    } else {
        result = 0;
    }
    func_020768bc();
    return result;
}
