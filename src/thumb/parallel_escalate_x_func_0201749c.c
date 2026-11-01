extern unsigned int func_02015d04();
extern int func_020175a4();
extern unsigned int func_02015cc4();
extern const unsigned short data_02084848[];
typedef int code();

unsigned int func_0201749c(unsigned char *param_1, unsigned short *param_2, int param_3)
{
    unsigned int uVar3;
    unsigned int uVar5;
    unsigned int uVar6;
    unsigned int uVar7;
    unsigned int uVar8;
    int iVar1;

    uVar6 = 1;
    if (param_2 == 0) {
        return uVar6;
    }
    if (param_3 != 0) {
        uVar6 = 0;
        while (uVar3 = func_02015d04(*param_2), uVar3 == 0) {
            iVar1 = func_020175a4(param_1, param_2);
            if (iVar1 != 0) {
                uVar6 = (uVar6 + 1) & 0xffff;
            }
            uVar7 = 0xff00;
            while (uVar8 = func_02015cc4(*param_2), uVar8 == 0) {
                if (*param_2 >= uVar7) {
                    if (*param_2 == 0xff17) {
                        param_1[0x5b] = 1;
                    }
                    param_2 = param_2 + data_02084848[(*param_2 - 0xff00) & 0xffff];
                }
                param_2 = param_2 + 1;
            }
            if (*param_2 == 0xff01) {
                if (iVar1 == 0) {
                    *param_2 = 0xff30;
                }
                param_2 = param_2 + 1;
            }
        }
        if (uVar6 == 0) {
            uVar6 = 1;
        }
    } else {
        while (uVar5 = func_02015d04(*param_2), uVar5 == 0) {
            if (*param_2 >= 0xff00) {
                if (*param_2 == 0xff17 && param_1[0x5b] == 0) {
                    param_1[0x5b] = 1;
                }
                if (*param_2 == 0xff01) {
                    uVar6 = (uVar6 + 1) & 0xffff;
                }
                param_2 = param_2 + data_02084848[(*param_2 - 0xff00) & 0xffff];
            }
            param_2 = param_2 + 1;
        }
    }
    return uVar6;
}
