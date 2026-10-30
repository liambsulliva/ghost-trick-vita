extern char data_02081f30;
extern char data_020b9df4;
extern int func_02016fc4();

void func_0201aa4c(int param_1)

{
    if (*(unsigned char *)(param_1 + 0x96) <= 1) {
        func_02016fc4(((int)&data_020b9df4),0x90);
        return;
    }
    if (*(unsigned char *)(param_1 + 0x96) < 5) {
        func_02016fc4(((int)&data_020b9df4),(unsigned int)*(unsigned short *)(((int)&data_02081f30) + 0x1c));
    }
    return;
}
