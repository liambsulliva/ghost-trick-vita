#define SWAP32(x) ((((x) & 0xff000000) >> 24) | (((x) & 0xff0000) >> 8) | (((x) & 0xff00) << 8) | ((x) << 24))

void func_0206a8c8(unsigned int op, unsigned int addr)
{
    unsigned int cmd1;
    unsigned int cmd2;

    cmd1 = (addr >> 8) | (op << 24);
    cmd2 = addr << 24;
    while ((*(volatile unsigned int *)0x040001a4 & 0x80000000) != 0) {
    }
    *(volatile unsigned short *)0x040001a0 = (unsigned short)((*(volatile unsigned short *)0x040001a0 & ~0x2000) | 0xc000);
    *(volatile unsigned int *)0x040001a8 = SWAP32(cmd1);
    *(volatile unsigned int *)0x040001ac = SWAP32(cmd2);
}
