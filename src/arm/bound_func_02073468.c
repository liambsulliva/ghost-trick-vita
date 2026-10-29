extern char data_027e0068;
void func_02073468(void)

{
    unsigned int *puVar1;

    puVar1 = ((unsigned int *)&data_027e0068);
    ((unsigned int *)&data_027e0068)[1] = 0;
    *puVar1 = 0;
    return;
}
