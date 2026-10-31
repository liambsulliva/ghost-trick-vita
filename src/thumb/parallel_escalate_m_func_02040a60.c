extern signed char data_0209b172;
extern signed char data_0209b182;
extern signed char data_0209b1dc;
extern signed char data_0209b210;
extern signed char data_020ce3b8;
extern int func_02008444();
extern int func_02009900();
extern int func_0200fb4c();
extern int func_02014ddc();
extern int func_02014de4();
extern int func_020283d0();
extern int func_0202f7e8();
extern int func_0203cbe4();
extern int func_0204026c();
extern int func_0204050c();
extern int func_020406d8();
extern int func_02040774();
extern int func_020407f4();
extern int func_02040b70();
extern int func_02040c8c();
extern int func_02078e84();

void func_02040a60(int param_1)

{
    int iVar1;
    unsigned int uVar2;

    func_0204050c(param_1);
    if (*(unsigned char *)(param_1 + 0x2c) == '\0') {
        func_02040774(param_1, (unsigned int)&data_0209b1dc);
    }
    else {
        func_02040774(param_1, (unsigned int)&data_0209b210);
    }
    if (*(unsigned char *)(param_1 + 0x2c) == '\0') {
        func_020407f4(param_1, (unsigned int)&data_0209b172);
    }
    else {
        func_020407f4(param_1, (unsigned int)&data_0209b182);
    }
    iVar1 = func_0204026c(((unsigned int)&data_020ce3b8));
    *(int *)(param_1 + 0x50) = iVar1;
    if (iVar1 != 0) {
        func_020406d8(iVar1,((unsigned int)0x000012d2u),6);
        iVar1 = func_0200fb4c(*(unsigned int *)(param_1 + 0x50));
        func_02014ddc(*(unsigned int *)(param_1 + 0x50),iVar1 + 0x14000);
        iVar1 = func_020283d0(*(unsigned int *)(param_1 + 0x50));
        func_02014de4(*(unsigned int *)(param_1 + 0x50),iVar1 + -0x24000);
        uVar2 = func_02009900();
        func_0202f7e8(uVar2,*(unsigned int *)(param_1 + 0x50),10,0);
    }
    uVar2 = func_02008444();
    uVar2 = func_0203cbe4(uVar2);
    iVar1 = func_02040c8c(param_1,uVar2);
    if (iVar1 != 0) {
        func_02078e84(param_1 + 0x54,uVar2);
        func_02040b70(param_1);
    }
    return;
}
