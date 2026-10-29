extern int func_0201572c();
extern int func_ov006_020d414c();
extern int func_ov006_020d5760();
extern int func_ov006_020d7240();

void func_ov006_020d58a8(int param_1,unsigned int param_2)

{
    int iVar1;

    iVar1 = func_0201572c();
    if (iVar1 == 0) {
        func_ov006_020d5760(0);
        *(unsigned char *)(param_1 + 0x24) = 10;
        *(unsigned char *)(param_1 + 0x26) = 0;
        func_ov006_020d414c(param_2);
        if (*(char *)(param_1 + 0x27) != '\x01') {
            func_ov006_020d7240(param_1);
        }
    }
    return;
}
