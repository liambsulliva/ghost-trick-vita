extern unsigned int func_0200b848(int param_1);
extern void func_0200bb10(unsigned int a,unsigned int b);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *a,unsigned int b,unsigned int c,unsigned int d,unsigned int e);
void func_0200bad8(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)
{
    unsigned int mode;
    func_0200bb10(func_0200b848(param_1 + 0x14),param_4 >> 7);
    if (param_2 != 0) {
        mode = *(unsigned char *)(param_1 + 10) == 0 ? 5 : 0x13;
        func_0202f1bc(func_02009c1c(),param_2,param_4,param_3,mode);
    }
}
