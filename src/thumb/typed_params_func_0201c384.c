extern unsigned char * func_02009c1c();
extern unsigned int * func_0201003c();
extern int func_0202f1bc();

void func_0201c384(int param_1)

{
    unsigned int *puVar1;

    func_0201003c();
    puVar1 = (unsigned int *)func_02009c1c();
    func_0202f1bc(puVar1,param_1 + 0x3c,*((unsigned int *)0x020b9d04u),0x20,4);
    return;
}
