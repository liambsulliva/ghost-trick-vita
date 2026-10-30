extern unsigned int data_020d2eec;
extern unsigned int data_020b17a8;
extern int func_02075cfc();
extern int func_02075cf0();
extern void func_020762b8();

void func_02077fe4(void)
{
    unsigned int *counter;

    counter = &data_020d2eec;
    if (*counter != 0) {
        do {
            if (((data_020b17a8 & 2) != 0 && func_02075cfc() == 18) ||
                ((data_020b17a8 & 4) != 0 && func_02075cf0() == 128) ||
                ((data_020b17a8 & 8) != 0 && *(volatile unsigned short *)0x04000208 == 0)) {
                func_020762b8();
            }
        } while (*counter != 0);
    }
}
