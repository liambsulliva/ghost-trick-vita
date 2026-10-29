extern int func_0202d68c();
extern int func_0202d6a4();
extern int func_0202d700();
extern int func_0202d710();

void func_0202ee20(char *param_1)

{
    char cVar1;
    int iVar2;
    int iVar3;

    if (*param_1 == '\x01') {
        cVar1 = param_1[1];
        *param_1 = '\x02';
        if ((cVar1 != '\b') && (cVar1 != '\t')) {
            if ((param_1[0xc] == '\0') || ((unsigned char)(cVar1 + 0xf5U) <= 1)) {
                *(int *)(param_1 + 4) = *(int *)(param_1 + 4) << 0xc;
                return;
            }
            iVar2 = func_0202d68c(*(unsigned int *)(param_1 + 8),*(unsigned int *)(param_1 + 4));
            iVar3 = func_0202d6a4(*(unsigned int *)(param_1 + 8),*(unsigned int *)(param_1 + 4));
            if (iVar3 == 1) {
                func_0202d700(*(unsigned int *)(param_1 + 8),*(unsigned int *)(param_1 + 4),iVar2 << 0xc);
                func_0202d710(*(unsigned int *)(param_1 + 8),*(unsigned int *)(param_1 + 4),2);
            }
        }
    }
    return;
}
