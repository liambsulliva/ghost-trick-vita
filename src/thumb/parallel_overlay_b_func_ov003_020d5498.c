extern unsigned char data_020cedc4[];
extern unsigned char data_020cee88[];
extern unsigned char data_020cf898[];
extern unsigned char data_020cfe2c[];
extern unsigned char data_020ce900[];
extern unsigned char *func_02009900(void);
extern unsigned int func_0202fa8c(unsigned int param_1, unsigned char *param_2);
extern unsigned int *func_02008444(void);
extern void *func_02007bd8(void);
extern int func_02007b70(void *object);
extern void *func_0201f33c(void *object, void *allocator, unsigned int size, void *data);
extern void func_020119b8(int unused, int *value);
extern unsigned int *func_0201f38c(unsigned int *param_1);
extern int func_ov003_020d4818();

void func_ov003_020d5498(unsigned char *param_1)
{
    int local[6];

    if (func_0202fa8c((unsigned int)func_02009900(), data_020cedc4) == 0) {
        if (func_0202fa8c((unsigned int)func_02009900(), data_020cee88) == 0) {
            if (func_0202fa8c((unsigned int)func_02009900(), data_020cf898) == 0) {
                if (func_0202fa8c((unsigned int)func_02009900(), data_020cfe2c) == 0) {
                    if (func_0202fa8c((unsigned int)func_02009900(), data_020ce900) == 0) {
                        func_0201f33c(local, (void *)func_02007b70(func_02007bd8()), 0x1ef0, *(unsigned char **)(param_1 + 0x184) + 0x10);
                        func_020119b8((int)func_02008444(), local);
                        *(int *)(param_1 + 0x174) = 0;
                        *(unsigned char *)(param_1 + 0x168) = 10;
                        func_ov003_020d4818(param_1);
                        func_0201f38c((unsigned int *)local);
                    }
                }
            }
        }
    }
}
