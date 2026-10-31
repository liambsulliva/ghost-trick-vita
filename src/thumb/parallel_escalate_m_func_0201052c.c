typedef struct {
    unsigned int low : 13;
    unsigned int mode : 2;
    unsigned int high : 17;
} Flags0201052c;
extern int data_020ce704;
extern int func_020105a8();
extern int func_020106a0();
extern int func_02044ee0();
extern void func_02046198();
extern void func_020105b0();
extern void func_02010428();
extern int func_02009900();
extern void func_0202f7e8();

void func_0201052c(int param_1, int param_2, int param_3, int param_4, int param_5)
{
    int iVar1;
    int off;
    int puVar2;
    int puVar3;
    off = 0xe00;
    if (*(int *)(param_1 + off) != 0) {
        iVar1 = func_020105a8(*(int *)(param_1 + off));
        if (param_2 == iVar1) {
            *(int *)(param_1 + off + 0x20) = param_3;
            return;
        }
    }
    if ((param_4 != 1 || ((Flags0201052c *)(param_1 + 0xc))->mode == 0) &&
        (func_020106a0(param_1), param_3 != 0)) {
        puVar2 = func_02044ee0(&data_020ce704);
        if (puVar2 != 0) {
            func_02046198(puVar2, param_2);
            func_020105b0(puVar2, (char)param_4);
            func_02010428(puVar2, param_5);
            puVar3 = func_02009900();
            func_0202f7e8(puVar3, puVar2, 8, 0);
            *(int *)(param_1 + off) = puVar2;
            *(int *)(param_1 + off + 0x20) = param_3;
        }
    }
}
