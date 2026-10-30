extern int data_020d126c;
extern void func_02075a54();

void func_02075944(unsigned int *param_1)
{
    unsigned int *old;

    old = (unsigned int *)((unsigned int *)&data_020d126c)[4];
    param_1[5] = (unsigned int)old;
    param_1[6] = 0;
    ((unsigned int *)&data_020d126c)[4] = (unsigned int)param_1;
    if (old != 0) {
        old[6] = (unsigned int)param_1;
        return;
    }
    ((unsigned int *)&data_020d126c)[3] = (unsigned int)param_1;
    func_02075a54(param_1);
}
