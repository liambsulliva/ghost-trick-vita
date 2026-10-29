extern void func_0200f7ac(int object,int flag);

void func_0200f780(int object,int value)
{
    int disp;

    if (value != 0) {
        disp = 0xde4;
        func_0200f7ac(object + disp,0);
        return;
    }
    disp = 0xe3c;
    *(int *)(object + disp) = 0;
    disp = disp - 0x58;
    func_0200f7ac(object + disp,1);
}
