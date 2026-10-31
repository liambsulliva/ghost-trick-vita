extern char data_020a2bbc;
extern char data_020b42d8;
extern char data_020c9e48;
extern char data_020c9e74;
extern int func_02009850();
extern int func_02009900();
extern int func_02009a0c();
extern int func_02009c1c();
extern int func_02009eec();
extern int func_02009fc8();
extern int func_0200a248();
extern int func_0200a780();
extern int func_0202f3a4();
extern int func_0202fad4();
extern int func_0206f588();

void func_0200a260(int param_1,unsigned int param_2,unsigned int param_3,unsigned int param_4)
{
    unsigned int uVar3;

    uVar3 = 0;
    func_0200a248(param_1,0);
    func_0202f3a4(func_02009c1c(),0x10,*(unsigned int *)(param_1 + ((int)0x0000aa44u)));
    do {
        if (func_02009fc8(param_1 + uVar3 * 0x10) == 0) {
            func_0200a780(param_1 + uVar3 * 0x10);
        }
        uVar3 = uVar3 + 1;
    } while (uVar3 < 0x10);
    *(unsigned short *)(((int)&data_020c9e48) + 10) = (unsigned short)0xc013u;
    func_02009850(&data_020a2bbc);
    func_0206f588(&data_020c9e74,&data_020a2bbc,0x80,0x60,(int)*(short *)(param_1 + ((int)0x0000aa34u)),
                  (int)*(short *)(param_1 + ((int)0x0000aa34u) + 2));
    func_0202fad4(func_02009900(),&data_020b42d8);
    func_02009a0c(param_1 + ((int)0x00006e02u),0xc00);
    func_02009eec(param_1);
    return;
}
