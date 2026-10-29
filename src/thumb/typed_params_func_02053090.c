extern unsigned char * func_02009900();
extern int func_0202fb48();
extern int func_02052efc();
extern int func_02059894();

void func_02053090(unsigned int param_1)

{
    unsigned char *puVar1;
    unsigned int uVar2;

    func_02052efc(param_1);
    puVar1 = func_02009900();
    uVar2 = func_0202fb48(puVar1,((unsigned char *)0x020cf19cu),-1);
    if ((uVar2 != 0) && (uVar2 != param_1)) {
        func_02059894(param_1,uVar2);
    }
    return;
}
