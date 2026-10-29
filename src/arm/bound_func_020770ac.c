extern char data_020d2e20;
typedef int code();

void func_020770ac(unsigned int param_1)

{
    unsigned int *puVar1;

    puVar1 = (unsigned int *)((param_1 & 0xff) * 0xc + ((int)&data_020d2e20));
    if (((int)param_1 >> 8 & 0xffU) != (unsigned int)*(unsigned char *)(puVar1 + 2)) {
        return;
    }
    if ((code *)*puVar1 == (code *)0x0) {
        return;
    }
    (*(code *)*puVar1)(puVar1[1]);
    return;
}
