struct Obj {
    unsigned char *w0;
    unsigned int pad0[5];
    unsigned int f18;
    unsigned int f1c;
    unsigned int f20;
    char pad1[12];
    signed char f30;
    char pad2[2];
    unsigned char f33;
};

extern void func_02038bd4(struct Obj *param_1, unsigned int param_2, int param_3);
extern void func_02039eb8(unsigned char *param_1, unsigned int param_2, int param_3);

void func_02038ba4(struct Obj *param_1)
{
    if (param_1->f33 != 0) {
        func_02038bd4(param_1, param_1->f20, param_1->f30);
    }
    else {
        func_02038bd4(param_1, param_1->f18, param_1->f30);
    }
    param_1->f1c = param_1->f1c - param_1->f30;
    func_02039eb8(param_1->w0, 1, param_1->f30);
}
