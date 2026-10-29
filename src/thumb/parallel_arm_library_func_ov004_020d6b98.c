unsigned short func_ov004_020d61f0(void);
unsigned short func_ov004_020d6200(unsigned short);
int func_0202f25c(void);
void func_ov004_020d6bbc(int, int);

unsigned short func_ov004_020d6b98(int param_1)

{
    unsigned short uVar1;
    unsigned short uVar2;

    uVar1 = func_ov004_020d61f0();
    uVar2 = 1;
    if (uVar1 == 0) {
        uVar2 = 0;
    }
    func_ov004_020d6bbc(func_0202f25c(), param_1);
    return func_ov004_020d6200(uVar2);
}
