struct State {
    unsigned int before[4];
    int data;
};
extern struct State data_020b18a0;
extern int func_02000d9c();
extern int func_02007a54();

void func_02002b88(unsigned int param_1)

{
    int iVar1;
    int iVar2;
    unsigned int *puVar3;

    iVar1 = ((int)0x020b18a0u);
    iVar2 = *(int *)(data_020b18a0.data + 0x1580);
    puVar3 = (unsigned int *)func_02000d9c(iVar2,param_1,*(int *)(*(int *)(iVar2 + 0x98) + 0x14));
    if (*(int *)(*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) +
                              (*puVar3 & 0xffffff) * 0x10 + 0x14) != 0) {
        func_02007a54();
    }
    *(unsigned int *)
      (*(int *)(*(int *)(data_020b18a0.data + 0x1580) + 0x90) + (*puVar3 & 0xffffff) * 0x10
      + 0x14) = 0;
    return;
}
