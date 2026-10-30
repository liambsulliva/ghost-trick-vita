extern long long func_0207b694(int, int);
extern void func_ov006_020d7c8c(int, int, int, int, signed char);
extern void func_ov006_020d6f80(int, unsigned int);

void func_ov006_020d7cf8(unsigned char *param_1, int param_2, int param_3)
{
    if (*(int *)(param_1 + 0x20) != 0) {
        func_ov006_020d7c8c((int)param_1, *(int *)(param_1 + 0x420), *(int *)(param_1 + 0x424), (signed char)param_2, (signed char)param_3);
        func_ov006_020d6f80(*(int *)(param_1 + 0x648), (int)func_0207b694(param_2 + 1, 10) + 0xb0);
        func_ov006_020d6f80(*(int *)(param_1 + 0x64c), (int)(func_0207b694(param_2 + 1, 10) >> 32) + 0xb0);
        func_ov006_020d6f80(*(int *)(param_1 + 0x650), (int)func_0207b694(param_3, 10) + 0xb0);
        func_ov006_020d6f80(*(int *)(param_1 + 0x654), (int)(func_0207b694(param_3, 10) >> 32) + 0xb0);
    }
}
