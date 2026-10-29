struct State {
    unsigned int before_flags[21];
    unsigned int flags;
};
extern struct State data_020c9e48;
extern char data_020c9e88;
extern char data_020ce320;
extern unsigned int * func_02008444();
extern int func_02010194();
extern int func_0201132c();
extern int func_02040604();
extern int func_02058ab8();

void func_0204050c(int param_1)

{
    unsigned int *puVar1;
    unsigned int uVar2;
    int iVar3;
    unsigned short *puVar4;

    puVar1 = func_02008444();
    uVar2 = func_0201132c((int)puVar1);
    *(char *)(param_1 + 0x2c) = (char)uVar2;
    puVar4 = (unsigned short *)0x0;
    data_020c9e48.flags = ((unsigned int)0x80001800u) | data_020c9e48.flags & ((unsigned int)0x3ffffdffu);
    *(unsigned short *)(((int)&data_020c9e88) + 0x1e) = (unsigned short)((unsigned int)0x000002c3u);
    *((unsigned char *)&data_020ce320) = 0;
    puVar1 = func_02008444();
    iVar3 = func_02010194((int)puVar1);
    if (iVar3 != 0) {
        *(int *)(param_1 + 0x40) = iVar3;
        puVar4 = (unsigned short *)func_02058ab8(iVar3);
    }
    func_02040604(param_1,puVar4);
    return;
}
