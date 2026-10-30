extern int func_02038cc0();
extern unsigned char * func_02031254();
extern unsigned char * func_0203a258();
extern int func_02038ac4();
extern int func_02038acc();
extern int func_0203a274();
extern unsigned char * func_02010b7c();
extern int func_0202f174();

void func_0203a204(int param_1)

{
    unsigned char *puVar1;
    unsigned char *puVar2;

    func_02038cc0(param_1);
    puVar1 = func_02031254(0x10);
    puVar2 = puVar1;
    if (puVar1 != (unsigned char *)0x0) {
        puVar2 = func_0203a258(puVar1);
    }
    *(unsigned char **)(param_1 + 0xcc) = puVar2;
    if (puVar2 != (unsigned char *)0x0) {
        func_02038ac4(puVar2);
        func_02038acc(*(unsigned char **)(param_1 + 0xcc),1);
        func_0203a274(*(unsigned char **)(param_1 + 0xcc),
                      (unsigned int)*(unsigned short *)(param_1 + 0xd0),
                      (unsigned int)*(unsigned short *)(param_1 + 0xd2));
        puVar2 = func_02010b7c();
        func_0202f174(puVar2,*(unsigned char **)(param_1 + 0xcc));
    }
    return;
}
