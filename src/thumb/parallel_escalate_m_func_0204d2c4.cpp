class Object0204d2c4
{
public:
    virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3();
    virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7();
    virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15();
    virtual void s16(); virtual void s17(); virtual void s18(); virtual void s19();
    virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27();
    virtual void s28(); virtual void s29(); virtual void s30(); virtual void s31();
    virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35(int argument0);
};

extern "C" {
unsigned int *func_02008444(void);
int func_0200f330(unsigned int *argument0);
void func_020471ec(void *argument0, char *argument1);
void func_02047250(void *argument0, char *argument1);
void func_020083b0(void *argument0, void *argument1);
void func_020083c0(void *argument0, void *argument1, void *argument2);
void func_0201f088(void *argument0, void *argument1, unsigned int argument2);
void func_0201f0a0(void *argument0, void *argument1);
void func_020349b0(void *argument0, void *argument1, int argument2);
void func_02008378(void *argument0, void *argument1, void *argument2);
void func_020632f0(void *argument0, void *argument1, void *argument2, int argument3, int argument4);
extern char data_020a46d4;
extern char data_020ceaa4;

void func_0204d2c4(char *param_1)
{
    unsigned int arr_b[3];
    unsigned int arr_c[3];
    unsigned int arr_d[3];
    int iStack_40[3];
    unsigned int arr_a[3];
    unsigned int arr_g[3];
    unsigned int arr_f[3];
    unsigned int *puVar1;
    int iVar2;

    puVar1 = func_02008444();
    iVar2 = func_0200f330(puVar1);
    if (iVar2 == 0) {
        func_020471ec(arr_a, param_1);
        func_020083b0(param_1 + 0xbc, arr_a);
        func_02047250(arr_b, param_1);
        func_020083c0(arr_c, arr_b, param_1 + 0xbc);
        func_0201f088(arr_d, arr_c, (unsigned int)&data_020a46d4);
        func_0201f0a0(iStack_40, arr_d);
        if (*(unsigned short *)(param_1 + 0xd4) != 0) {
            iStack_40[0] = -iStack_40[0];
            iStack_40[1] = -iStack_40[1];
        }
        func_020349b0(arr_f, iStack_40, 0x28000);
        func_02008378(arr_g, param_1 + 0xbc, arr_f);
        func_020083b0(param_1 + 200, arr_g);
        iVar2 = 0x1a;
        if (0x1a < *(int *)&data_020ceaa4) {
            iVar2 = *(int *)&data_020ceaa4;
        }
        func_020632f0(*(void **)(param_1 + 0xb8), param_1 + 0xbc, param_1 + 200, 600, 0x800);
        ((Object0204d2c4 *)*(int *)(param_1 + 0xb8))->s35((signed char)iVar2);
    }
}
}
