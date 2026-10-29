struct State {
    unsigned int before[4];
    int data;
};
extern struct State data_020b18a0;
extern int func_02074188();

void func_02006b4c(int param_1)

{
    int iVar1;
    int iVar2;

    iVar2 = ((int)0x00004cbau);
    iVar1 = ((int)0x020b18a0u);
    if (param_1 == -1) {
        *(unsigned char *)(data_020b18a0.data + ((int)0x00004cbau)) =
                  *(unsigned char *)(data_020b18a0.data + ((int)0x00004cbau)) | 2;
        *(unsigned char *)(data_020b18a0.data + iVar2) = *(unsigned char *)(data_020b18a0.data + iVar2) | 4;
        return;
    }
    func_02074188(data_020b18a0.data + ((int)0x00002064u));
    return;
}
