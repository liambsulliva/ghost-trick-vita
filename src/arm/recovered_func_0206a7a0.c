extern int func_02000b64();
extern int func_02069b6c();
extern int func_02069d7c();
extern int func_02069f74();
extern int func_02075f84();

void func_0206a7a0(unsigned int param_1,unsigned int param_2,unsigned int param_3)

{
    unsigned int uVar1;

    func_02000b64(((unsigned int)0x02000bacu));
    func_02069b6c();
    uVar1 = func_02069d7c();
    if (param_1 != (param_1 & uVar1)) {
        func_02075f84();
    }
    func_02069f74(((unsigned int)0x020d02a0u),1,param_2,param_3);
    return;
}
