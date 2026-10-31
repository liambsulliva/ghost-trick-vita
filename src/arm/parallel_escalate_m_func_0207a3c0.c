typedef void (*voidfunc)(void);
extern int data_020d3648;
extern int data_020d0f28[];
extern int data_020d3600[];
extern int data_020d3624[];
extern int data_020d34f0[];
extern voidfunc data_020d3500[];
extern int func_02074630(void *);
extern void func_02074588(void *);
extern void func_020745d8(void *);
extern void func_0207a5fc(int);
extern void func_0207c5b8(void);

void func_0207a3c0(void)
{
    voidfunc f;

    if (func_02074630(&data_020d3648) == 0) {
        data_020d3600[0] = *(int *)(data_020d0f28[1] + 0x6c);
        data_020d3624[0] = 1;
    }
    else if (data_020d3600[0] == *(int *)(data_020d0f28[1] + 0x6c)) {
        data_020d3624[0]++;
    }
    else {
        func_02074588(&data_020d3648);
        data_020d3600[0] = *(int *)(data_020d0f28[1] + 0x6c);
        data_020d3624[0] = 1;
    }
    while (data_020d34f0[2] > 0) {
        data_020d3500[--data_020d34f0[2]]();
    }
    if (--data_020d3624[0] == 0) {
        func_020745d8(&data_020d3648);
    }
    f = (voidfunc)data_020d34f0[0];
    if (f != 0) {
        f();
        data_020d34f0[0] = 0;
    }
    func_0207a5fc(0);
    func_0207c5b8();
}
