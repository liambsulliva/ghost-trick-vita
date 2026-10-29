extern unsigned int *data_020d3a3c;
typedef int code();

void func_0207d9ac(void)

{
    unsigned int *puVar2;

    puVar2 = data_020d3a3c;
    if (puVar2 == (unsigned int *)0x0) {
        return;
    }
    do {
        data_020d3a3c = (unsigned int *)*puVar2;
        (*(code *)puVar2[1])(puVar2[2],0xffffffff);
        puVar2 = data_020d3a3c;
    } while (puVar2 != (unsigned int *)0x0);
    return;
}
