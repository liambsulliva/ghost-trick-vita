void func_0202f600(int param_1,int param_2)

{
    if ((*(int *)(param_1 + 0x24) != 0) && (param_2 != *(int *)(param_1 + 0x24)) &&
          (param_2 != param_1)) {
        *(unsigned char *)(param_1 + 0xc) = 6;
        *(unsigned char *)(param_1 + 0xe) = 0;
        return;
    }
    *(unsigned char *)(param_1 + 0xc) = 4;
    *(unsigned char *)(param_1 + 0xe) = 0;
    return;
}
