extern unsigned int *func_02008444();
extern int func_020328e4();
extern int func_0205c21c();
extern int func_0205c230();
extern void func_0205f074();
extern void func_02058a80();
extern unsigned int *func_020083b0();
extern void func_0205c244();
extern unsigned char *func_0202970c();
extern int func_02029708();
extern void func_0205eb78();

void func_0205c164(int param_1)
{
    unsigned int *puVar1;
    int iVar2;
    int auStack_1c[3];

    if (func_020328e4(func_02008444()) != 0 || (func_0205c21c(func_02008444()) != 0 && *(unsigned char *)(param_1 + 0x50) != 2)) {
        func_0205f074(*(unsigned char **)(param_1 + 0x48), 0x76c0);
    }
    else if (func_0205c230(func_02008444()) != 0 && *(unsigned char *)(param_1 + 0x50) == 2) {
        func_0205f074(*(unsigned char **)(param_1 + 0x48), 0x76c0);
    }
    else {
        func_0205f074(*(unsigned char **)(param_1 + 0x48), 0x3500);
    }
    if (*(int *)(param_1 + 0x4c) != 0) {
        func_02058a80(auStack_1c, *(int *)(param_1 + 0x4c));
        puVar1 = func_020083b0(param_1 + 0x6c, auStack_1c);
        puVar1 = func_020083b0(param_1 + 0x60, puVar1);
        func_020083b0(param_1 + 0x54, puVar1);
        *(int *)(param_1 + 0x60) = *(int *)(param_1 + 0x60) + 0x1000;
    }
    func_0205c244(*(unsigned char **)(param_1 + 0x48), *(unsigned char *)(param_1 + 0x50));
    iVar2 = func_02029708(func_0202970c());
    func_0205eb78(*(unsigned char **)(param_1 + 0x48), 1, iVar2, param_1 + 0x54, param_1 + 0x6c);
}
