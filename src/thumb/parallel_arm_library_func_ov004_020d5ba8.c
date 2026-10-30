extern void func_ov004_020d59a4(int, int);
extern void func_ov004_020d59d4(int, int);

struct Low3 {
    unsigned char low3 : 3;
};

void func_ov004_020d5ba8(int param_1)
{
    unsigned int bits;

    if (*(unsigned int *)(param_1 + 0x48) == 0) {
        bits = ((struct Low3 *)(param_1 + 0xaaa))->low3;
        if ((bits & 1) != 0) {
            func_ov004_020d59a4(param_1, 8);
        }
        bits = ((struct Low3 *)(param_1 + 0xaaa))->low3;
        if ((bits & 2) != 0) {
            func_ov004_020d59d4(param_1, 0x10);
        }
    } else {
        bits = ((struct Low3 *)(param_1 + 0xaaa))->low3;
        if ((bits & 1) != 0) {
            func_ov004_020d59a4(param_1, 4);
        }
        bits = ((struct Low3 *)(param_1 + 0xaaa))->low3;
        if ((bits & 2) != 0) {
            func_ov004_020d59d4(param_1, 4);
        }
    }
    *(unsigned int *)(param_1 + 0x48) ^= 1;
}
