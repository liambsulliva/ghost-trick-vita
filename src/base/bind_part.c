void func_02008fa4(void);
void func_02073ed4(void *part, void (*callback)(void), void *object, void *other, int scale, int count);
void func_02074188(void *part);

int func_02008fb0(char *object)
{
    object[6] = 0;
    func_02073ed4(object + 0x41c, func_02008fa4, object, object + 0x14dc, 1 << 12, 5);
    func_02074188(object + 0x41c);
    return 1;
}
