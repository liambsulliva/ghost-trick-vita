extern int func_0201bd78();
extern int func_0201bdd4();
extern int func_0201c248();

void func_0201c084(int param_1,unsigned int param_2,unsigned int param_3)

{
    int iVar1;
    unsigned int *puVar2;
    unsigned int uVar3;
    unsigned int uVar4;

    iVar1 = func_0201bd78(param_1 + 4);
    if (iVar1 != 0) {
        for (uVar4 = 0; uVar3 = func_0201bd78(param_1 + 4), uVar4 < uVar3; uVar4 = uVar4 + 1) {
            puVar2 = (unsigned int *)func_0201bdd4(param_1 + 4,uVar4);
            func_0201c248(*puVar2,param_2,param_3);
        }
    }
    return;
}
