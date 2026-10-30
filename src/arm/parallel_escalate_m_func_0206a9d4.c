extern void func_0206a8c8();
unsigned int func_0206a9d4(void)
{
    unsigned int v;

    if ((*(unsigned int *)0x02fffc00 & 0x20000000) == 0) {
        return 0x20;
    }
    func_0206a8c8(214, 0);
    v = *(unsigned int *)0x02fffae0;
    v = v & ~0x07000000;
    v = v | 0xa7000000;
    *(unsigned int *)0x040001a4 = v & 0xffffe000;
    do {
    } while ((*(volatile unsigned int *)0x040001a4 & 0x00800000) == 0);
    return *(unsigned int *)0x04100010;
}
