struct Pair {
    unsigned short v[2];
};
typedef void code(void *object, int value);
extern struct Pair data_0209aae0;
extern char data_020ced58;
extern void *func_0204edb4(void *place);
extern void func_0203cdd8(void *object, unsigned short param_2, unsigned short param_3, unsigned short param_4);
extern void *func_02009900(void);
extern void func_0202f7e8(void *param_1, void *param_2, unsigned int param_3, int param_4);
extern void func_0203cf74(int param_1, int param_2, int param_3);

void func_0203cd64(int param_1, int param_2, int param_3)
{
    struct Pair pos;
    unsigned int i;
    int slot;
    void *object;

    pos = data_0209aae0;
    for (i = 0; i < 2; i++) {
        slot = param_1 + i * 4;
        object = func_0204edb4(&data_020ced58);
        *(void **)(slot + 0x38) = object;
        if (object != 0) {
            func_0203cdd8(object, pos.v[i], pos.v[i] + 5, pos.v[i] + 10);
            (**(code **)(**(int **)(slot + 0x38) + 0x84))(*(void **)(slot + 0x38), 0x6f);
            func_0202f7e8(func_02009900(), *(void **)(slot + 0x38), 1, 0);
        }
    }
    func_0203cf74(param_1, param_2, param_3);
}
