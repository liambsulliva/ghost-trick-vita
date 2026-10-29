extern int func_02010b54();
extern int func_02029708();
extern int func_0202970c();

void func_0205e1bc(int param_1)

{
    char cVar1;
    int iVar2;

    iVar2 = func_02010b54();
    if ((((iVar2 != 0) && (cVar1 = *(char *)(param_1 + 0x4c), cVar1 != '\0')) && (cVar1 != '\x01')) &&
          (cVar1 == '\x02')) {
        func_0202970c();
        func_02029708();
    }
    return;
}
