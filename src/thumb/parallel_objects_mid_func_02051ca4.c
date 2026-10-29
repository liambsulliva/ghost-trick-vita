extern signed char data_020ceea4;
extern signed char data_020ceec0;
extern unsigned char * func_02009900();
extern int func_0202f7e8();
extern int func_020507d8();
extern signed char * func_02078e84();

void func_02051ca4(signed char *param_1)

{
    int iVar1;
    unsigned char *puVar2;

    iVar1 = func_020507d8(((int)&data_020ceea4));
    if (iVar1 != 0) {
        func_02078e84(((signed char *)&data_020ceec0),param_1);
        puVar2 = func_02009900();
        func_0202f7e8((int)puVar2,iVar1,0,0);
    }
    return;
}
