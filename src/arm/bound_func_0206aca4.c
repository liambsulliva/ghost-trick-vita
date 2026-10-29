extern char data_020d02a0;
extern char data_020d08e0;
typedef int code();
extern int func_0206a948();
extern int func_02073480();
extern int func_020735c0();
extern int func_02073620();
extern int func_02075c98();
extern int func_02075cac();

void func_0206aca4(int param_1)

{
    unsigned int uVar1;
    unsigned int uVar2;

    uVar2 = func_02075c98();
    uVar1 = ((unsigned int)0x0206ac04u);
    *(int *)(((int)&data_020d08e0) + 8) = param_1;
    func_02073480(0x80000,uVar1);
    func_02073620(0x80000);
    func_020735c0(0x80000);
    func_02075cac(uVar2);
    (*(code *)**(unsigned int **)(((int)&data_020d02a0) + 0x50c))
                        (*(unsigned int *)(((int)&data_020d02a0) + 0x508),((unsigned int)0x04100010u),*(unsigned int *)(param_1 + 0x10),
                          0x200);
    func_0206a948(*(unsigned int *)(param_1 + 0xc));
    return;
}
