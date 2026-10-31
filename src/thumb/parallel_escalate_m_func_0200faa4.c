extern void func_0200f70c(int *param_1);
extern int func_02010194(unsigned char *param_1);
extern void func_02058a80(int *param_1, int param_2);
extern void func_0205958c(int *param_1, unsigned char *param_2, unsigned int param_3);
extern int *func_020083a0(unsigned char *param_1);
extern void func_02008378(int *param_1, int *param_2, int *param_3);
extern void func_020083b0(int *param_1, int *param_2);
extern int func_0200f5c8(unsigned char *param_1);
extern int func_0200f5e8(unsigned char *param_1);
extern void func_0200f61c(unsigned char *param_1, int param_2, int param_3, int param_4);

void func_0200faa4(unsigned char *param_1, unsigned char *param_2, unsigned int param_3, int param_4, int param_5, int param_6)
{
    int pos[3];
    int base[3];
    int sum[3];
    int offset[3];
    int *src;
    int node;
    int *scale;

    if (param_4 == -1 && param_5 == -1) {
        return;
    }
    func_0200f70c(pos);
    if (param_3 == 0xffffffff) {
        node = func_02010194(param_1);
        if (node == 0) {
            return;
        }
        src = base;
        func_02058a80(src, node);
    } else {
        if (param_2 == 0) {
            goto done;
        }
        func_0205958c(offset, param_2, param_3);
        scale = func_020083a0(param_2);
        src = sum;
        func_02008378(src, offset, scale);
    }
    func_020083b0(pos, src);
done:
    if (param_4 == -1) {
        pos[0] = func_0200f5c8(param_1);
    } else {
        pos[0] = pos[0] - param_4 * 0x1000;
    }
    if (param_5 == -1) {
        pos[1] = func_0200f5e8(param_1);
    } else {
        pos[1] = pos[1] - param_5 * 0x1000;
    }
    func_0200f61c(param_1, pos[0], pos[1], param_6);
}
