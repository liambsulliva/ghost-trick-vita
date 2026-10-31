extern char data_02084848;
extern char data_020b9df4;
extern int func_02015cc4(unsigned int c);
extern int func_02017214(char *p);
extern unsigned short func_0201c0fc(unsigned int c);
extern int func_0201c130(unsigned int c);
extern unsigned char func_0201cf7c(char *p, unsigned int c, unsigned int prev);
extern int func_0201cff0(char *p, unsigned int c, unsigned int prev);

int func_02017240(unsigned char *param_1, unsigned short *param_2)
{
    unsigned int lim[3];
    unsigned short base;
    int count;
    unsigned short prev;
    unsigned short total;
    unsigned short add;
    unsigned short cur;

    count = *(unsigned short *)(param_1 + 0x4c);
    total = 0;
    prev = 0xfffe;
    lim[0] = prev;
    lim[1] = prev;
    lim[1] = lim[1] - 0xfe;
    lim[2] = prev;
    lim[2] = lim[2] - 0xfe;
    while (total < count) {
        if (func_02015cc4(*param_2) != 0) {
            break;
        }
        cur = *param_2;
        if (cur < lim[1]) {
            if (func_0201c130(cur) != 0) {
                base = func_0201c0fc(*param_2);
                add = base + func_02017214(&data_020b9df4) * 2;
            } else {
                add = func_0201cf7c(&data_020b9df4, *param_2, lim[0]);
            }
            total = total + add;
            if (func_0201cff0(&data_020b9df4, cur, prev) != 0) {
                total = total + func_02017214(&data_020b9df4);
            }
            prev = cur;
        } else {
            param_2 = param_2 + *(unsigned short *)(&data_02084848 + ((cur - lim[2]) & 0xffff) * 2);
        }
        param_2 = param_2 + 1;
    }
    *(unsigned short *)(param_1 + 0x3e) = (unsigned short)((count - total) - *(short *)(param_1 + 0x42));
    if (*(short *)(param_1 + 0x3e) < 0) {
        *(unsigned short *)(param_1 + 0x3e) = 0;
    }
    return 0;
}
