extern char data_020a7b64;
extern char data_020ce320;
extern int func_020402e8();

unsigned int * func_020402c4(unsigned int *param_1)

{
    char cVar1;

    func_020402e8();
    *param_1 = ((unsigned int)&data_020a7b64);
    cVar1 = *((char *)&data_020ce320);
    *((char *)&data_020ce320) = cVar1 + '\x01';
    *(char *)((int)param_1 + 0x11) = cVar1;
    return param_1;
}
