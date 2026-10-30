extern int func_02075c98();
extern void func_02075cac(int param_1);
extern struct Clock { int unused; int vcount; int frame; } data_020d126c;

int func_02075c48(int param_1)
{
    int iVar1;

    iVar1 = func_02075c98();
    if (param_1 < data_020d126c.vcount) {
        data_020d126c.frame = data_020d126c.frame + 1;
    }
    data_020d126c.vcount = param_1;
    func_02075cac(iVar1);
    return data_020d126c.frame;
}
