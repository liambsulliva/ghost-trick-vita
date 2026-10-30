typedef int code();

typedef struct {
    int count;
    int busy;
    code *callback;
} State0206b090;

extern State0206b090 data_020d0ba0;
extern void func_02069e44(unsigned int, unsigned int);
extern void func_0206b130(void);
extern void func_02075f84();

void func_0206b090(unsigned int param_1, unsigned int param_2)
{
    int result;

    if ((param_2 & 0x3f) == 0x11) {
        if (data_020d0ba0.busy != 0) {
            return;
        }
        data_020d0ba0.busy = 1;
        func_02069e44(1, 0);
        result = 1;
        if (data_020d0ba0.callback != 0) {
            result = data_020d0ba0.callback();
        }
        if (result != 0) {
            func_0206b130();
        }
        return;
    }
    if ((param_2 & 0x3f) == 2) {
        data_020d0ba0.count++;
        data_020d0ba0.busy = 0;
        func_02069e44(2, 0);
        return;
    }
    func_02075f84();
}
