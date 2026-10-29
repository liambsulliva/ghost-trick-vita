extern int func_02010b60();
extern int func_02010b64();
extern int func_02010b7c();

void func_0202ed3c(char *param_1)

{
    unsigned int uVar1;

    if (*param_1 == '\x04') {
        *param_1 = '\x01';
        param_1[0xc] = '\x01';
    }
    if (param_1[1] == '\v') {
        func_02010b7c();
        uVar1 = func_02010b60();
        *(unsigned int *)(param_1 + 4) = uVar1;
    }
    if (param_1[1] == '\f') {
        func_02010b7c();
        uVar1 = func_02010b64();
        *(unsigned int *)(param_1 + 4) = uVar1;
    }
    if ((unsigned char)(param_1[1] + 0xf8U) <= 1) {
        *param_1 = '\x02';
    }
    return;
}
