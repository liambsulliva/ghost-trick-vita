extern unsigned int func_0200bc10();
extern int func_0200bb8c();

void func_02058fdc(int param_1)
{
    unsigned int ret;
    unsigned int id;

    ret = func_0200bc10(param_1 + 0x28);
    id = 0x8a;
    if (ret == 0) {
        id = 0x84;
    }
    func_0200bb8c(param_1 + 0x28, (unsigned short)id);
    return;
}
