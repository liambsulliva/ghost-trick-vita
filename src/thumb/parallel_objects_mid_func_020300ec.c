struct State {
    unsigned int first;
    unsigned char before_flags[80];
    unsigned int flags;
};
extern struct State data_020c9e48;
extern int func_02008824();
extern unsigned char * func_0200883c();
extern unsigned char * func_02009c1c();
extern unsigned long long func_0201e7f8();
extern int func_0202f1bc();
extern int func_0203018c();
extern int func_020301e0();

void func_020300ec(void)

{
    unsigned int *puVar1;
    int iVar2;
    unsigned char *puVar3;
    int *piVar4;
    unsigned int *puVar5;

    data_020c9e48.first = data_020c9e48.first | 0x200;
    data_020c9e48.flags = data_020c9e48.flags | 0x200;
    puVar3 = func_0200883c();
    piVar4 = (int *)func_02008824(puVar3);
    puVar1 = ((unsigned int *)0x020a5574u);
    func_0201e7f8(piVar4,0,4,(unsigned char *)*((unsigned int *)0x020a5574u));
    puVar5 = (unsigned int *)func_02009c1c();
    func_0202f1bc(puVar5,*puVar1,0x1e0,0x20,7);
    puVar5 = (unsigned int *)func_02009c1c();
    func_0202f1bc(puVar5,*puVar1,0x1e0,0x20,0x15);
    func_020301e0(0,0,1);
    func_020301e0(1,0,9);
    func_020301e0(2,0,4);
    func_020301e0(3,0,7);
    func_0203018c();
    iVar2 = ((int)0x00001120u);
    *(unsigned char *)(*puVar1 + ((int)0x00001120u)) = 1;
    *(unsigned char *)(*puVar1 + iVar2 + 1) = 1;
    *(unsigned char *)(*puVar1 + iVar2 + 2) = 0;
    return;
}
