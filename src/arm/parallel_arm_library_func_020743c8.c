struct State {
    unsigned int before;
    int count;
};
extern struct State data_020d0f0c;
extern long long func_02075c98();
extern int func_02075cac();

int func_020743c8(void)

{
    int iVar1;
    unsigned int saved;
    int iVar2;

    saved = (unsigned int)func_02075c98();
    iVar2 = data_020d0f0c.count;
    iVar1 = 0;
    if (iVar2 != 0) {
        data_020d0f0c.count = iVar2 + -1;
        iVar1 = iVar2;
    }
    func_02075cac(saved);
    return iVar1;
}
