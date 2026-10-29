extern unsigned char * func_0202970c();
extern int func_02010b54();
extern int func_02029708();
extern int func_0202c358();
extern int func_02061fc4();
extern int func_02061fd0();
extern int func_02061fe0();
extern int func_02061fec();
extern int func_02062018();
extern int func_02062034();
extern int func_02063220();
extern int func_0206322c();
extern int func_02063268();

void func_02063164(int param_1)

{
    unsigned char mode;
    int iVar1;
    unsigned int uVar2;
    unsigned int z;
    int stride;
    short *table;

    mode = *(unsigned char *)(param_1 + 0xc);
    if ((((mode == 3) && (iVar1 = func_02010b54(param_1), iVar1 != 0)) &&
            (iVar1 = func_0202c358(param_1), iVar1 != 0)) &&
          ((*(unsigned char *)(param_1 + 0x92) != 0 && (*(int *)(param_1 + 0x88) != 0)))) {
        iVar1 = func_02029708(func_0202970c());
        func_02063220(2);
        func_02061fc4();
        uVar2 = 0;
        func_02061fd0(0,0,-(iVar1 - (unsigned int)*(unsigned short *)(param_1 + 0x94)));
        func_0206322c(0,0,0,0,0,0,0,0);
        func_02063268(0,0,3,0x3c,(int)*(signed char *)(param_1 + 0x44),0x800);
        func_02061fe0(2);
        func_02061fec(*(unsigned short *)(param_1 + 0x90));
        z = uVar2;
        stride = 2;
        do {
            table = *(short **)((char *)param_1 + 0x98);
            func_02062018((int)*(short *)((char *)table + uVar2 * 4),(int)*(short *)((char *)table + uVar2 * 4 + stride),z);
            uVar2 = uVar2 + 1;
        } while (uVar2 < 0x22);
        func_02062034();
    }
    return;
}
