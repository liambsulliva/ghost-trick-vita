extern void func_02075f84(void);
extern int func_02075c98(void);
extern void func_02075504(int *alarm,unsigned long long fire);
extern int func_02075cac(int enabled);

void func_02075698(int *alarm,unsigned long long start,unsigned long long period,int handler,int arg)
{
    int enabled;

    if (alarm == 0 || alarm[0] != 0) {
        func_02075f84();
    }
    enabled = func_02075c98();
    *(unsigned long long *)(alarm + 7) = period;
    *(unsigned long long *)(alarm + 9) = start;
    alarm[0] = handler;
    alarm[1] = arg;
    func_02075504(alarm,0);
    func_02075cac(enabled);
}
