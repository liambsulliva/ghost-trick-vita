extern int func_02060d54();
extern int func_02060f54();

void func_02060f34(unsigned char *param_1)

{
    char cVar1;

    cVar1 = param_1[0x7e];
    func_02060d54(param_1,1);
    if (cVar1 == '\0') {
        func_02060f54((int)param_1,0);
    }
    return;
}
