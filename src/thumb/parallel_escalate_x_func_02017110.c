extern char data_02084848;
extern char data_020b9df4;
extern int func_02015cc4(unsigned int c);
extern int func_02017214(char *p);
extern unsigned short func_0201c0fc(unsigned int c);
extern int func_0201c130(unsigned int c);
extern unsigned char func_0201cf7c(char *p, unsigned int c, unsigned int prev);
extern int func_0201cff0(char *p, unsigned int c, unsigned int prev);

int func_02017110(unsigned char *param_1, unsigned short *param_2)
{
    unsigned int lim[4];
    unsigned short base;
    int flag;
    int count;
    unsigned short prev;
    unsigned short total;
    unsigned short add;
    unsigned short cur;

    flag = 0;
    count = *(unsigned short *)(param_1 + 0x4a);
    total = 0;
    prev = 0xfffe;
    lim[0] = prev;
    lim[1] = prev;
    lim[1] = lim[1] - 0xfe;
    lim[2] = prev;
    lim[2] = lim[2] - 0xe7;
    lim[3] = prev;
    lim[3] = lim[3] - 0xfe;
    while (total < count) {
        if (func_02015cc4(*param_2) != 0) {
            break;
        }
        cur = *param_2;
        if (cur < lim[1]) {
            if (func_0201c130(cur) != 0) {
                base = func_0201c0fc(*param_2);
                add = base + func_02017214(&data_020b9df4) * 2;
                total = total + add;
            } else {
                add = func_0201cf7c(&data_020b9df4, *param_2, lim[0]);
                if (flag == 0) {
                    total = total + add;
                } else {
                    add = add * 2;
                    total = total + add;
                }
                if (func_0201cff0(&data_020b9df4, cur, prev) != 0) {
                    total = total + func_02017214(&data_020b9df4);
                }
            }
            prev = cur;
        } else if (cur == lim[2]) {
            if (flag == 0) {
                flag = 1;
            } else {
                flag = 0;
            }
        } else {
            param_2 = param_2 + *(unsigned short *)(&data_02084848 + ((cur - lim[3]) & 0xffff) * 2);
        }
        param_2 = param_2 + 1;
    }
    *(unsigned short *)(param_1 + 0x3c) = (unsigned short)((count - total) / 2 - *(short *)(param_1 + 0x42));
    if (*(short *)(param_1 + 0x3c) < 0) {
        *(unsigned short *)(param_1 + 0x3c) = 0;
    }
    return 0;
}
