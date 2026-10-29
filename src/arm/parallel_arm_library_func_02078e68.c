extern unsigned char data_020d2ec0[];

void func_02078e68(void)
{
    unsigned int value;

    value = *(unsigned int *)0x02fffc3c;
    *(unsigned int *)(data_020d2ec0 + 8) = value;
}
