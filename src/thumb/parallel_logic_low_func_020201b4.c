struct Pair {
    unsigned int first;
    unsigned int second;
};
void func_020201b4(unsigned int *param_1,unsigned int *param_2)

{
    unsigned int uVar1;

    if (param_1 != param_2) {
        *param_1 = *param_2;
        *(unsigned short *)(param_1 + 1) = *(unsigned short *)(param_2 + 1);
        *(unsigned short *)((int)param_1 + 6) = *(unsigned short *)((int)param_2 + 6);
        param_1[2] = param_2[2];
        param_1[3] = param_2[3];
        param_1[5] = param_2[5];
        *(struct Pair *)(param_1 + 7) = *(struct Pair *)(param_2 + 7);
        *(struct Pair *)(param_1 + 9) = *(struct Pair *)(param_2 + 9);
        param_1[0xb] = param_2[0xb];
        param_1[0xc] = param_2[0xc];
        param_1[0xd] = param_2[0xd];
    }
    return;
}
