struct Scan
{
    unsigned int state[2];
    unsigned char flag;
    unsigned char pad;
    unsigned short f1;
    unsigned short f2;
    unsigned short pad2;
    unsigned int tail;
};

extern void func_02077518();
extern int func_02077538();
extern int func_02077718();
extern void func_020776e4();

void func_02002820(int param_1, int param_2, unsigned int param_3)
{
    struct Scan s;
    func_02077518(&s, param_1);
    while (func_02077538(param_1, &s.flag, &s) != 0) {
        if (s.flag == 1 && s.f2 == param_3) {
            if (func_02077718(param_2, s.f1) != 0) {
                func_020776e4(param_2, s.f1, 0);
            }
        }
    }
}
