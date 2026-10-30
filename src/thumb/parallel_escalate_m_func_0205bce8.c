typedef void code();
extern void func_02008398(void *param_1, int param_2, int param_3, int param_4);
extern unsigned char *func_02009900(void);
extern unsigned int *func_0202f624(int param_1);
extern void func_0202f7e8(int param_1, int param_2, unsigned int param_3, int param_4);
extern unsigned int *func_02062c74(unsigned int *param_1);

void func_0205bce8(int param_1)
{
    unsigned int *obj;
    unsigned char *mgr;
    unsigned char buf[12];

    obj = func_0202f624(0xf8);
    if (obj != 0) {
        obj = func_02062c74(obj);
    }
    *(unsigned int **)(param_1 + 0x28) = obj;
    mgr = func_02009900();
    func_0202f7e8((int)mgr, *(int *)(param_1 + 0x28), 1, 0);
    func_02008398(buf, 0x80000, 0x145000, 0);
    (**(code **)(**(int **)(param_1 + 0x28) + 0xb8))(*(int **)(param_1 + 0x28), buf, 0x8000, 0x1f000);
}
