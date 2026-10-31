struct Bits130 { unsigned char b0 : 1; unsigned char b1 : 1; unsigned char b2 : 1; unsigned char b3 : 1; unsigned char b4 : 1; unsigned char hi : 3; };
struct Bits0e0 { unsigned char b0 : 1; unsigned char b1 : 1; unsigned char b2 : 1; unsigned char b3 : 1; unsigned char b4 : 1; unsigned char b5 : 1; unsigned char b6 : 1; unsigned char b7 : 1; };

extern unsigned int *func_02008444();
extern unsigned int func_02011320();
extern int func_0203435c();
extern int func_020109c0();
extern int func_0200f330();
extern int func_0200f320();
extern char *func_02010b7c();
extern unsigned int func_02015188();
extern int func_0202cb64();
extern int func_02057718();
extern int func_02010b54();

int func_0204630c(unsigned char *param_1, int param_2, int param_3)
{
    unsigned int *puVar2;

    if (((struct Bits130 *)(param_1 + 0x130))->b0) {
        return 0;
    }
    if (param_1[0x10] != 1) {
        return 0;
    }
    puVar2 = func_02008444();
    if (func_02011320(puVar2) != 3) {
        return 0;
    }
    puVar2 = func_02008444();
    if (func_0203435c(puVar2) != 0) {
        return 0;
    }
    if (func_020109c0(param_1) != 0) {
        return 0;
    }
    if (((struct Bits130 *)(param_1 + 0x130))->hi != 3 || ((struct Bits0e0 *)(param_1 + 0xe0))->b2) {
        puVar2 = func_02008444();
        if (func_0200f330(puVar2) != 0) {
            return 0;
        }
    }
    if (((struct Bits130 *)(param_1 + 0x130))->hi != 2) {
        return 0;
    }
    puVar2 = func_02008444();
    if (func_0200f320(puVar2) != 0) {
        return 0;
    }
    if (func_02015188(func_02010b7c()) == 0) {
        return 0;
    }
    if (func_0202cb64(func_02010b7c()) != 0) {
        return 0;
    }
    if (func_02057718(param_1, param_2, param_3) != 0 && func_02010b54(param_1)) {
        return 1;
    }
    return 0;
}
