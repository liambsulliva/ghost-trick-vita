extern unsigned char * func_02007bd8();
extern int func_0201f2cc();
extern int func_0202f364();

void func_0202f280(unsigned int *param_1)

{
    unsigned char *puVar1;
    unsigned int uVar2;
    unsigned int *puVar3;

    if (*(char *)((int)param_1 + ((int)0x00004028u)) == '\0') {
        puVar3 = param_1 + 1;
        uVar2 = 0;
        if (uVar2 < *param_1) {
            for (; uVar2 < *param_1; uVar2 = uVar2 + 1, puVar3 = puVar3 + 4) {
                func_0202f364(param_1,puVar3);
            }
        }
        puVar1 = func_02007bd8();
        func_0201f2cc((int)puVar1);
        *param_1 = 0;
    }
    return;
}
