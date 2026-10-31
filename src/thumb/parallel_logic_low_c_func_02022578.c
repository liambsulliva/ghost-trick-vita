extern int func_02067eb4();
extern int func_02067ef8();

void func_02022578(unsigned int *param_1)
{
    unsigned int uVar1;

    func_02067eb4((unsigned char *)(param_1 + 4), 0);
    func_02067eb4((unsigned char *)(param_1 + 7), 0);
    func_02067eb4((unsigned char *)(param_1 + 10), 0);
    *(unsigned int *)(param_1[3] + 8) = 0;
    *(unsigned int *)(param_1[3] + 0xc) = (*(unsigned int *)(param_1[3] + 0xc) & 0xff000000u) | (*param_1 & 0x00ffffffu);
    func_02067ef8((int *)(param_1 + 7), param_1[3]);
    for (uVar1 = 1; uVar1 < param_1[2]; uVar1 = uVar1 + 1) {
        func_02067ef8((int *)(param_1 + 10), param_1[3] + uVar1 * 0x10);
    }
}
