extern void func_02074ff0(int, unsigned int);

void func_0207508c(int object, unsigned int mask, unsigned int value)
{
    func_02074ff0(object, value | (mask & ((0 - 0x1000) << ((value - 0x16) >> 1))) | 1);
}
