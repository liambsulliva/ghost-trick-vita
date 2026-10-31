struct Allocator {
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int size, char *tag);
    virtual void slot3();
    virtual void slot4(int block);
};

extern "C" {
extern char data_020ca01c[];
extern char data_020ca02c[];
extern char data_020ca03c[];
extern char data_020850e0[];
void *func_02007bd8(void);
Allocator *func_02007b70(void *object);
void func_0207303c(char *src, int dst, int size);

void func_02027c98(char *param_1, int param_2)
{
    int i4;
    unsigned int i;
    int sum;

    i4 = param_2 * 4;
    if (*(int *)(data_020ca02c + i4) != 0) {
        if (func_02007b70(func_02007bd8()) != 0) {
            func_02007b70(func_02007bd8())->slot4(*(int *)(data_020ca02c + i4));
            *(int *)(data_020ca02c + i4) = 0;
        }
    }
    *(char **)(data_020ca03c + i4) = param_1;
    if (param_1 == 0) {
        *(int *)(data_020ca01c + i4) = 0;
        return;
    }
    *(int *)(data_020ca01c + i4) = *(int *)(param_1 + 0x34 + *(int *)(param_1 + 8) + *(int *)(param_1 + 0xc));
    *(int *)(data_020ca02c + i4) = func_02007b70(func_02007bd8())->slot2(*(int *)(data_020ca01c + i4) << 3, data_020850e0);
    func_0207303c(param_1 + 0x38 + *(int *)(param_1 + 8) + *(int *)(param_1 + 0xc), *(int *)(data_020ca02c + i4), *(int *)(data_020ca01c + i4) << 3);
    sum = (int)(param_1 + 0x38 + *(int *)(param_1 + 8) + *(int *)(param_1 + 0xc)) + *(int *)(data_020ca01c + i4) * 8;
    for (i = 0; i < *(unsigned int *)(data_020ca01c + i4); i++) {
        *(int *)(*(int *)(data_020ca02c + i4) + i * 8) += sum;
    }
}
}
