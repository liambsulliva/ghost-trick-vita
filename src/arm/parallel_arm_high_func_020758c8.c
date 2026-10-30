extern int data_020d126c;
extern void func_02075944();
extern void func_02075a54();

void func_020758c8(unsigned int *param_1)
{
    unsigned int *p;
    unsigned int *q;

    p = (unsigned int *)((unsigned int *)&data_020d126c)[3];
    if (p != 0) {
        do {
            if (p[3] >= param_1[3]) {
                if (p[3] != param_1[3] || ((short *)p)[8] > ((short *)param_1)[8]) {
                    q = (unsigned int *)p[5];
                    param_1[5] = (unsigned int)q;
                    param_1[6] = (unsigned int)p;
                    p[5] = (unsigned int)param_1;
                    if (q != 0) {
                        q[6] = (unsigned int)param_1;
                        return;
                    }
                    ((unsigned int *)&data_020d126c)[3] = (unsigned int)param_1;
                    func_02075a54(param_1);
                    return;
                }
            }
            p = (unsigned int *)p[6];
        } while (p != 0);
    }
    func_02075944(param_1);
}
