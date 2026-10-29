struct ByteFlags {
    unsigned char flag : 1;
};
extern int func_020570cc();

void func_020460f0(unsigned int param_1)

{
    if (*(signed char *)(param_1 + 0x118) != -1) {
        if (*(signed char *)(param_1 + 0x118) == '\x01') {
            *(unsigned char *)(param_1 + 0x118) = 0;
            return;
        }
        if ((((struct ByteFlags *)(param_1 + 0x130))->flag == 0) && (*(char *)(param_1 + 199) == '\0')
              ) {
            func_020570cc(param_1);
        }
    }
    return;
}
