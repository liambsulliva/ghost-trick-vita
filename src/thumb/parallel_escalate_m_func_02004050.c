struct State {
    unsigned int before[4];
    int data;
};
extern struct State data_020b18a0;
extern int func_02002ff4(int value);
extern unsigned int func_02003030(int value);
extern int func_0200306c(int value);
extern void func_02003eb4(int entry,int value);
extern void func_02003e30(int entry,int value,int flag);

void func_02004050(int param_1,int param_2,int param_3,unsigned int param_4,unsigned short param_5,unsigned int param_6)
{
    int mask;
    unsigned int i;
    int entry;
    int value;

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
    value = param_5;
    for (i = 0; i < 0x10; i++) {
        entry = data_020b18a0.data + 0x15bc + i * 0x4c;
        switch (param_4) {
        case 0:
        case 1:
            if (*(unsigned int *)(entry + 4) != param_4) {
                break;
            }
        case 2:
            if (((mask >> i) & 1) == 0) {
                break;
            }
            if (param_6 != *(unsigned char *)(entry + 0x12) && param_6 != 0xff) {
                break;
            }
            if ((*(unsigned int *)(entry + 0x3c) & 4) != 0) {
                func_02003eb4(entry,value);
            }
            else {
                func_02003e30(entry,value,1);
            }
            break;
        case 3:
            break;
        }
    }
}
