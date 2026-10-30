extern char data_02083e2c[];
extern char data_02083e34[];
extern void *func_02008444(void);
extern int func_0200f1c0(void *system);
extern void func_02012400(unsigned char *object, char *a, char *b);

void func_020110b8(unsigned char *object)
{
    if (func_0200f1c0(func_02008444()) != 0) {
        return;
    }
    if (*(int *)(object + 0xe58) != 0) {
        return;
    }
    *(int *)(object + 0xe58) = 0x18;
    func_02012400(object, data_02083e2c, data_02083e34);
}
