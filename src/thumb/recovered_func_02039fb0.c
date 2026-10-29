extern int func_02031254();

void func_02039fb0(void)

{
    unsigned int *puVar1;

    puVar1 = (unsigned int *)func_02031254(0xc);
    if (puVar1 != (unsigned int *)0x0) {
        *puVar1 = 0;
        puVar1[1] = 0;
        puVar1[2] = 0;
    }
    return;
}
