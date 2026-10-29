extern char data_0209e33c;
extern void func_02075f84();

unsigned int func_0206a038(unsigned int flags, int unused)
{
    unsigned int result;
    int helper;

    result = 0;
    helper = (flags & 0x10) != 0;
    flags = flags & 0xffffffef;
    if (flags <= 3) {
        if (helper == 0) {
            result = (unsigned int)&data_0209e33c;
        }
        else {
            func_02075f84();
        }
    }
    return result;
}
