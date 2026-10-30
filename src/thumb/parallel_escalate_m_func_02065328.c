extern int func_02065168();
extern int func_02065170();

struct Stream02065328 {
    char pad00[0x38];
    int first[4];
    int data[4];
    int second[4];
    unsigned int size[4];
    char pad78[0x24];
    unsigned char count;
};

int func_02065328(struct Stream02065328 *param_1,int param_2,unsigned char *param_3,unsigned char *param_4)
{
    int bVar1;
    char *entry;
    unsigned int iVar2;
    unsigned int uVar3;

    if (param_2 == 0) {
        return 0;
    }
    if (param_1->count <= *param_3) {
        return 0;
    }
    func_02065168(param_2,param_1->first[*param_3],param_1->second[*param_3]);
    iVar2 = (unsigned int)*param_4 * 0x800;
    entry = (char *)param_1 + (unsigned int)*param_3 * 4;
    uVar3 = *(unsigned int *)(entry + 0x68);
    if (uVar3 > iVar2 + 0x800U) {
        func_02065170(param_2,*(int *)(entry + 0x48) + iVar2,0x800);
        bVar1 = *param_4 + 1;
    }
    else {
        func_02065170(param_2,*(int *)(entry + 0x48) + iVar2,uVar3 - iVar2);
        *param_3 = *param_3 + 1;
        bVar1 = 0;
    }
    *param_4 = bVar1;
    return 1;
}
