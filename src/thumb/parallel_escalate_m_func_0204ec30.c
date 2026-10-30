extern int func_02029708();
extern unsigned char *func_0202970c();
extern int func_020501f4();
void func_0204ec30(int param_1)
{
    short level;
    unsigned char *object;
    int value;
    level = *(short *)(param_1 + 0x48);
    object = func_0202970c();
    value = func_02029708(object);
    func_020501f4(0, 0, value - (unsigned int)*(unsigned short *)(param_1 + 0x42), level * 100, 0x30, 0x1f,
                  (short)((0x1f * (0x40 - level)) / 0x40), (int)*(signed char *)(param_1 + 0x45), 0);
}
