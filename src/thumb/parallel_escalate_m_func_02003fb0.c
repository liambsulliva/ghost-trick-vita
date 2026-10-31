struct State {
    unsigned int before[4];
    int data;
};

extern struct State data_020b18a0;
extern int func_02002ff4();
extern int func_02003030();
extern int func_0200306c();
extern void func_02003e30();
extern void func_02007948();

void func_02003fb0(int param_1, int param_2, int param_3, int type, int arg, int sel)
{
    int mask;
    int i;
    unsigned char *e;

    mask = 0xffff;
    if (param_1 != -1) {
        mask &= func_02002ff4(param_1);
    }
    if (param_2 != -1) {
        mask &= func_02003030(param_2);
    }
    if (param_3 != -1) {
        mask &= func_0200306c(param_3);
    }
    for (i = 0; i < 0x10; i++) {
        e = (unsigned char *)(data_020b18a0.data + 0x15bc) + i * 0x4c;
        switch (type) {
        case 0:
        case 1:
            if (*(int *)(e + 4) != type) {
                break;
            }
        case 2:
            if (((mask >> i) & 1) != 0 && (sel == e[0x12] || sel == 0xff)) {
                func_02003e30(e, arg, 0);
            }
            break;
        case 3:
            break;
        }
    }
    func_02007948();
}
