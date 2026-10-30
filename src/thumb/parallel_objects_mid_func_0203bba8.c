extern char data_0209a984;
extern char data_0209a998;
extern char data_0209a9ac;
extern char data_0209a9b4;
extern char data_0209a934;
extern char data_0209a938;

void func_0203baa4(void *param_1);
unsigned int *func_02008444(void);
unsigned int func_0203247c(unsigned int *param_1);
void func_0201192c(unsigned int *param_1, unsigned int param_2);
void func_02010384(unsigned int *param_1, int param_2);
unsigned int func_02011320(unsigned int *param_1);
void *func_02011264(void);
char *func_0201123c(void *param_1, int param_2);
void func_02022f34(void *param_1, int param_2, int param_3);
int func_0200fa54(unsigned int *param_1);
void func_0200f958(unsigned int *param_1, unsigned int param_2);
void func_0200f9e8(unsigned int *param_1, unsigned int param_2);
int func_02012518(unsigned int *param_1, unsigned int param_2, unsigned int param_3);
void func_0200da04(unsigned int param_1, unsigned int param_2, unsigned int param_3, unsigned int param_4);
void func_0201135c(unsigned int *param_1);

void func_0203bba8(void *param_1)
{
    unsigned int *puVar2;
    unsigned int *puVar3;
    unsigned int uVar4;
    char *puVar5;
    int iVar6;
    int iZero;
    unsigned int uVar7;
    unsigned int uVar8;

    iZero = 0;
    func_0203baa4(param_1);
    puVar2 = func_02008444();
    puVar3 = func_02008444();
    uVar4 = func_0203247c(puVar3);
    func_0201192c(puVar2, uVar4);
    puVar2 = func_02008444();
    func_02010384(puVar2, iZero);
    puVar2 = func_02008444();
    uVar4 = func_02011320(puVar2);
    if (uVar4 == 2) {
        puVar5 = func_0201123c(func_02011264(), iZero);
        func_02022f34(puVar5, (int)&data_0209a984, iZero);
    }
    else {
        puVar5 = func_0201123c(func_02011264(), iZero);
        func_02022f34(puVar5, (int)&data_0209a998, iZero);
        puVar2 = func_02008444();
        uVar7 = (int)&data_0209a9ac;
        uVar8 = (int)&data_0209a9b4;
        iVar6 = func_02012518(puVar2, uVar7, uVar8);
        if (iVar6 != 0) {
            func_0200da04((int)&data_0209a934, uVar7, uVar8, (int)&data_0209a938);
        }
        else {
            puVar2 = func_02008444();
            iVar6 = func_0200fa54(puVar2);
            if (iVar6 != 0) {
                puVar2 = func_02008444();
                func_0200f9e8(puVar2, 0x3e8000);
            }
            else {
                puVar2 = func_02008444();
                func_0200f958(puVar2, 0x3e8000);
            }
        }
    }
    puVar2 = func_02008444();
    uVar4 = func_02011320(puVar2);
    if (uVar4 == 3) {
        puVar2 = func_02008444();
        func_0201135c(puVar2);
    }
}
