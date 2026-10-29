extern unsigned char data_020c9e88[];
void func_ov004_020d707c(int);

void func_ov004_020d743c(void)
{
    func_ov004_020d707c(0x10);
    *(unsigned short *)(data_020c9e88 + 0x12) = 0x10;
}
