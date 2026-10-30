extern void func_02075f84(void);
extern int func_02075c98(void);
extern unsigned long long func_020753bc(void);
extern void func_02075504(unsigned int *alarm, unsigned long long fire);
extern void func_02075cac(int enabled);

void func_0207562c(unsigned int *alarm, unsigned long long tick, unsigned int handler, unsigned int arg)
{
    int enabled;

    if (alarm == 0 || alarm[0] != 0) {
        func_02075f84();
    }
    enabled = func_02075c98();
    alarm[7] = 0;
    alarm[8] = 0;
    alarm[0] = handler;
    alarm[1] = arg;
    func_02075504(alarm, func_020753bc() + tick);
    func_02075cac(enabled);
}
