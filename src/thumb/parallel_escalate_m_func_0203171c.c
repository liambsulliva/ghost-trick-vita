extern char data_020cff60;
typedef int code();
extern int func_02031684();
extern int func_020312e4();
extern int func_020317b0();
extern int func_020317b4();
extern int func_02008444();
extern int func_020317b8();
extern int func_0202f25c();
extern int data_ov005_020d5958();
extern int func_02031420();
extern int func_02031430();
extern int func_020735f0();
extern int func_0200847c();
extern int func_0202a068();
extern int func_0202a16c();
extern int func_0202a01c();
extern int func_020747e4();
extern int func_02075d14();
extern int func_02075dc0();

void func_0203171c(int param_1)
{
    int iVar2;
    unsigned int uVar4;
    int *puVar3;
    unsigned char *st;

    iVar2 = func_02031684(param_1);
    if (iVar2 != 0) {
        puVar3 = (int *)func_020312e4();
        iVar2 = func_020317b0(puVar3);
        if (iVar2 == 0) {
            puVar3 = (int *)func_020312e4();
            iVar2 = func_020317b4(puVar3);
            if (iVar2 == 0) {
                puVar3 = (int *)func_02008444();
                iVar2 = func_020317b8(puVar3);
                if (iVar2 != 0) {
                    func_0202f25c();
                    data_ov005_020d5958();
                }
                func_02031420(0xfffffff0);
                func_02031430(0xfffffff0);
                func_020735f0(1);
                puVar3 = (int *)func_0200847c();
                func_0202a068(puVar3, 0);
                puVar3 = (int *)func_0200847c();
                func_0202a16c(puVar3, 0);
                puVar3 = (int *)func_0200847c();
                func_0202a01c(puVar3);
                func_020747e4();
                func_02075d14(0x007fd87f);
                st = (unsigned char *)&data_020cff60;
                *(char *)(st + 0x10) = 1;
                uVar4 = *(unsigned char *)(st + 0x13) & ~1;
                *(unsigned char *)(st + 0x13) = (unsigned char)(uVar4 | 1);
                func_02075dc0(*(int *)(st + 0x10), st, uVar4);
            }
        }
    }
}
