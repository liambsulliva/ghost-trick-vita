extern void func_0206a8c8();
void func_0206aa9c(void)
{
    unsigned int v;

    func_0206a8c8(181, 0);
    v = *(unsigned int *)0x02fffae0;
    v = v & ~0x07000000;
    v = v | 0xa0000000;
    *(unsigned int *)0x040001a4 = v & 0xffffe000;
    do {
    } while ((*(volatile unsigned int *)0x040001a4 & 0x80000000) != 0);
}
