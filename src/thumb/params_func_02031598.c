extern char data_027e0000;
void func_02031598(unsigned int param_1)

{
    *(unsigned int *)(((int)&data_027e0000) + ((int)0x00003ff8u)) = param_1 | *(unsigned int *)(((int)&data_027e0000) + ((int)0x00003ff8u));
    return;
}
