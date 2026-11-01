void func_02014744(int select)
{
    *(volatile unsigned short *)0x04000304 = (unsigned short)((*(volatile unsigned short *)0x04000304 & ~0x8000) | (select << 15));
}
