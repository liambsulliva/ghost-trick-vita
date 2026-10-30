extern int func_02073480();
extern int func_0206f46c();
extern int func_020735c0();

void func_02075a54(int param_1)
{
    unsigned int id;

    id = 4;
    func_02073480(id,0x02075a94);
    func_0206f46c((int)*(short *)(param_1 + 16));
    *(unsigned short *)(0x04000000 + id) = *(unsigned short *)(0x04000000 + id) | 32;
    func_020735c0(id);
}
