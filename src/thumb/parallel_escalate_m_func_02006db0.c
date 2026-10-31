struct State {
    unsigned int before[4];
    int data;
    int ready;
};
extern struct State data_020b18a0;
extern int data_020b1908;
extern void func_02006c0c(void);
extern void func_02006c6c(void);
extern void func_02006c84(void);
extern void func_02003900(void);
extern void func_020056e8(void);
extern void func_0200584c(int list,unsigned int proc,int value);
extern void func_020066a8(void);
extern void func_02006b8c(void);
extern void func_02007924(void);
extern void func_02073ed4(int thread,unsigned int proc,int arg,int stack,unsigned int size,unsigned int priority);
extern void func_02074564(int thread);
extern void func_020752d4(void);
extern void func_02078d80(int list);
extern void func_02078da0(int list);

void func_02006db0(unsigned int param_1)
{
    int iVar1;
    int iVar2;
    int iVar3;

    if (data_020b18a0.ready == 0) {
        iVar3 = data_020b1908;
        data_020b18a0.data = iVar3 + 0x20;
        *(unsigned int *)(iVar3 + 0x20 + 0x000015a4u) = param_1;
        if ((*(unsigned int *)(data_020b18a0.data + 0x000015a4u) & 1) != 0) {
            func_02007924();
        }
        func_020752d4();
        func_02003900();
        func_020056e8();
        func_020066a8();
        func_02006b8c();
        iVar2 = 0x1584;
        iVar1 = (int)&data_020b18a0;
        func_0200584c(*(int *)(iVar1 + 0x10) + iVar2,(unsigned int)func_02006c6c,0);
        func_0200584c(*(int *)(iVar1 + 0x10) + iVar2 + 0x10,(unsigned int)func_02006c84,0);
        func_02078d80(*(int *)(iVar1 + 0x10) + iVar2);
        func_02078da0(*(int *)(iVar1 + 0x10) + iVar2 + 0x10);
        iVar2 = 0x1be4;
        func_02073ed4(*(int *)(iVar1 + 0x10) + 0x1be4,(unsigned int)func_02006c0c,0,*(int *)(iVar1 + 0x10) + 0x2524,0x400,5);
        func_02074564(*(int *)(iVar1 + 0x10) + iVar2 + -0x30);
        *(int *)(iVar1 + 0x14) = 1;
    }
}
