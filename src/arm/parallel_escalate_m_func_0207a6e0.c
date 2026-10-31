typedef void (*sigfunc)(int);
extern int data_020d36f0;
extern int data_020d0f28[];
extern int data_020d3600[];
extern int data_020d3624[];
extern sigfunc data_020d3a20[];
extern int func_02074630(void *);
extern void func_02074588(void *);
extern void func_020745d8(void *);
extern void func_0207a37c(int);

int func_0207a6e0(int sig)
{
    sigfunc f;

    if (sig < 1 || sig > 7) {
        return -1;
    }
    if (func_02074630(&data_020d36f0) == 0) {
        data_020d3600[7] = *(int *)(data_020d0f28[1] + 0x6c);
        data_020d3624[7] = 1;
    }
    else if (data_020d3600[7] == *(int *)(data_020d0f28[1] + 0x6c)) {
        data_020d3624[7]++;
    }
    else {
        func_02074588(&data_020d36f0);
        data_020d3600[7] = *(int *)(data_020d0f28[1] + 0x6c);
        data_020d3624[7] = 1;
    }
    f = data_020d3a20[sig - 1];
    if (f != (sigfunc)1) {
        data_020d3a20[sig - 1] = 0;
    }
    if (--data_020d3624[7] == 0) {
        func_020745d8(&data_020d36f0);
    }
    if (f == (sigfunc)1 || (f == 0 && sig == 1)) {
        return 0;
    }
    if (f == 0) {
        func_0207a37c(0);
    }
    f(sig);
    return 0;
}
