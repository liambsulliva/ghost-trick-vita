extern unsigned int func_02027c28(int value);
extern void func_0200d80c(char *object, int value);
extern void func_0200d818(char *object, int value);

void func_02050fd4(int unused, char *object, char *table, int index)
{
    int offset;
    unsigned int value;

    offset = index * 2;
    value = func_02027c28(*(unsigned short *)(table + offset));
    func_0200d80c(object + 4, value);
    value = func_02027c28(*(unsigned short *)((unsigned char *)table + offset + 2));
    func_0200d818(object + 4, value);
}
