extern char data_0209dc34;
extern char data_0209dc70;
extern char data_0209dc7c;
extern int func_020083b0();
extern int func_0200da04();
extern int func_02014220();
extern int func_02060d54();

void func_0206124c(unsigned char *param_1)

{
    char *pcVar1;

    param_1[0xb8] = 0;
    func_020083b0((unsigned int *)(param_1 + 0xbc),
                                (unsigned int *)(*(int *)(param_1 + 0xd0) + ((unsigned char)param_1[0x7f] - 1) * 0xc));
    func_02060d54(param_1,6);
    if (*(unsigned char **)(param_1 + 0xf4) != (unsigned char *)0x0) {
        pcVar1 = (char *)func_02014220(*(unsigned char **)(param_1 + 0xf4));
        func_0200da04(pcVar1,((int)&data_0209dc70),((char *)&data_0209dc7c),((unsigned int)&data_0209dc34));
    }
    if (*(unsigned char **)(param_1 + 0xf8) != (unsigned char *)0x0) {
        pcVar1 = (char *)func_02014220(*(unsigned char **)(param_1 + 0xf8));
        func_0200da04(pcVar1,((int)&data_0209dc70),((char *)&data_0209dc7c),((unsigned int)&data_0209dc34));
    }
    return;
}
