extern void func_0200b598(void *, int, int);
extern unsigned int func_0200b7c0(void *);

void func_ov006_020d4a88(unsigned char *param_1)
{
    if (*(int *)(param_1 + 0xbc) != -1) {
        func_0200b598(param_1 + 0x28, *(int *)(param_1 + 0xbc), 0xf);
        *(int *)(param_1 + 0xbc) = -1;
    }
    func_0200b7c0(param_1 + 0x28);
}
