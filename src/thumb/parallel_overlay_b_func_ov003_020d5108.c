typedef int code(void *);

extern char data_020cece8;
extern char data_020c9d50;
extern char data_ov003_020d5aec;

extern unsigned int func_0204ed1c(unsigned int value);
extern unsigned int func_02009900(void);
extern void func_0202f7e8(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
extern void func_0202f64c(unsigned int value);
extern void func_ov003_020d4818(char *param_1);
extern void func_02010b7c(void);
extern int func_0202cb64(void);
extern int func_02015188(void);
extern int func_ov003_020d56b0(char *param_1);
extern void func_ov003_020d4ce4(char *param_1, int value);
extern unsigned int func_0200847c(void);
extern void func_0202a0c0(unsigned int object, int value);
extern void func_ov003_020d4f70(char *param_1);
extern void func_020506f4(int a, int b, int c);
extern unsigned int func_02022dfc(char *param_1);
extern void func_02022f34(unsigned int object, unsigned int data, unsigned int value);

void func_ov003_020d5108(char *param_1)
{
    unsigned int handle;
    unsigned int result;
    int state;
    int i;

    if (*(unsigned int *)(param_1 + 0x154) == 0) {
        *(unsigned int *)(param_1 + 0x154) = func_0204ed1c((unsigned int)&data_020cece8);
        handle = func_02009900();
        func_0202f7e8(handle, *(unsigned int *)(param_1 + 0x154), 1, 0);
        return;
    }
    func_0202f64c((unsigned int)(param_1 + 0x38));
    i = 0;
    do {
        func_0202f64c((unsigned int)(param_1 + 0x160 + i * 4));
        i = i + 1;
    } while (i < 2);
    func_ov003_020d4818(param_1);
    state = 0;
    func_02010b7c();
    if (func_0202cb64() == 0) {
        func_02010b7c();
        if (func_02015188() != 0) {
            state = func_ov003_020d56b0(param_1);
            if ((**(code **)(**(int **)(param_1 + 0x154) + 0xe0))(*(void **)(param_1 + 0x154)) != 0) {
                state = 3;
            }
            i = 0;
            do {
                result = (**(code **)(**(int **)(param_1 + i * 4 + 0x158) + 0xd4))(*(void **)(param_1 + i * 4 + 0x158));
                if (result != 0) {
                    state = i + 1;
                }
                i = i + 1;
            } while (i < 2);
        }
    }
    if ((*(unsigned int *)&data_020c9d50 & 0x100) != 0) {
        state = 1;
    }
    if ((*(unsigned int *)&data_020c9d50 & 0x200) != 0) {
        state = 2;
    }
    if ((*(unsigned int *)((char *)&data_020c9d50 + 4) & 0x1000) != 0) {
        state = 6;
    }
    if ((*(unsigned int *)((char *)&data_020c9d50 + 4) & 0x2000) != 0) {
        state = 3;
    }
    if (state == 3) {
        (**(code **)(**(int **)(param_1 + 0x154) + 0xdc))(*(void **)(param_1 + 0x154));
        *(unsigned int *)(param_1 + 0x154) = 0;
        *(unsigned int *)(param_1 + 0x174) = 0;
        *(unsigned char *)(param_1 + 0x168) = 8;
        func_0202a0c0(func_0200847c(), 3);
        return;
    }
    func_ov003_020d4ce4(param_1, state);
    if (state == 6) {
        *(unsigned int *)(param_1 + 0x174) = 0;
        *(unsigned char *)(param_1 + 0x168) = 3;
        *(unsigned char *)(param_1 + 0x16a) = 0;
        *(unsigned char *)(param_1 + 0x169) = 0;
        func_0202a0c0(func_0200847c(), 2);
        if (*(unsigned int *)(param_1 + 0x154) != 0) {
            (**(code **)(**(int **)(param_1 + 0x154) + 0xd8))(*(void **)(param_1 + 0x154));
            *(unsigned int *)(param_1 + 0x154) = 0;
        }
        func_ov003_020d4f70(param_1);
        func_020506f4(8, 1, -1);
        handle = func_02022dfc(param_1);
        func_02022f34(handle, (unsigned int)&data_ov003_020d5aec,
            (0x0000016fU - 0x6fU)
            | ((unsigned int)*(unsigned char *)(param_1 + 0x16f) << 16)
            | (*(unsigned char *)(param_1 + *(int *)(param_1 + 0x16f + 9) + 0x16f + 0x19) + 1));
    }
}
