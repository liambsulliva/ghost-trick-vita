extern void func_0200a68c();
extern void func_02009830();
extern char data_020a2bbc;
extern char data_020c9e48;
extern int func_02009848();
extern int func_02009968();
extern int func_0200a424();
extern int func_0200a614();
extern int func_0201e2cc();
extern int func_02009850();
extern int func_0207dd08();

int func_02009858(int param_1)

{
    int iVar1;
    unsigned int *puVar2;

    func_0207dd08(param_1,0x10,0x10,((unsigned int)func_02009830),((unsigned int)func_0200a68c));
    func_0200a614(param_1 + 0x100);
    iVar1 = ((int)0x0000aa04u);
    func_0201e2cc(param_1 + ((int)0x0000aa04u));
    func_02009848(param_1 + iVar1);
    func_0200a424(param_1);
    func_02009968(param_1);
    puVar2 = ((unsigned int *)&data_020c9e48);
    *((unsigned int *)&data_020c9e48) = ((unsigned int)0xc021171du);
    *(unsigned short *)((int)puVar2 + 10) = (unsigned short)((unsigned int)0x0000c013u);
    func_02009850(((unsigned int)&data_020a2bbc));
    return param_1;
}
