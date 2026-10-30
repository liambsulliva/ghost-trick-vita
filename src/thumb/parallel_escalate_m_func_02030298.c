typedef char *va_list;

extern char *data_020a5574;
extern void func_020302f0(void *object, int value);
extern int func_02073acc(char *buffer, int size, const char *format, va_list args);
extern void func_0203030c(int mode, int a, int b, int c, int length);

void func_02030298(int a, int b, int c, const char *format, ...)
{
    va_list args;
    int length;

    func_020302f0(data_020a5574 + 0x20, 0x100);
    args = (va_list)(((unsigned int)&format & ~3) + 4);
    length = func_02073acc(data_020a5574 + 0x20, 0xff, format, args);
    func_0203030c(0, a, b, c, length);
    data_020a5574[0x1120] = 1;
}
