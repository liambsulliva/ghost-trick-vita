extern signed char data_0209a8f8;
extern signed char data_0209a934;
extern signed char data_0209a938;
extern signed char data_0209a9d4;
extern signed char data_0209a9e0;
extern int func_0200da04();
extern int func_02014220();

void func_0203bfc8(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)

{
    signed char *pcVar1;

    if (*(signed char *)(param_1 + 0x49) == '\x01') {
        func_0200da04(((signed char *)&data_0209a934),((int)&data_0209a8f8),((signed char *)&data_0209a9d4),((unsigned int)&data_0209a938));
        return;
    }
    if (*(signed char *)(param_1 + 0x49) == '\0') {
        if (*(unsigned char **)(param_1 + 0x44) != (unsigned char *)0x0) {
            pcVar1 = (signed char *)func_02014220(*(unsigned char **)(param_1 + 0x44));
            func_0200da04(pcVar1,((int)&data_0209a8f8),((signed char *)&data_0209a9e0),((unsigned int)&data_0209a938));
            return;
        }
        func_0200da04(((signed char *)&data_0209a934),((int)&data_0209a8f8),((signed char *)&data_0209a9e0),((unsigned int)&data_0209a938));
    }
    return;
}
