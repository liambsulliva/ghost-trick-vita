extern char data_00000800[];
extern char data_027e0000[];

void func_0207364c(void)
{
    *(unsigned int *)((unsigned int)data_027e0000 + 0x3f7c) = 0xfddb597d;
    *(unsigned int *)((unsigned int)data_027e0000 + 0x3f80 - (int)data_00000800) = 0x7bf9dd5b;
}
