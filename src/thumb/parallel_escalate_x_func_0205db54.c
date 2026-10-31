extern unsigned char data_020cff60;
extern void func_0207193c();
extern void func_02071dbc();
extern void func_020716e8();
extern void func_0205dc0c();
extern void func_02071d94();
extern void func_02071de4();
extern void func_020719fc();
extern unsigned int *func_02008444();
extern void func_0205dc44();

void func_0205db54(unsigned char *p)
{
    if (p[0x54] == 0) {
        if (p[0x10] != 0) {
            p[0x10] = p[0x10] - 1;
        }
        else {
            *(int *)(p + 0x50) += *(unsigned char *)((char *)&data_020cff60 + 1) + 1;
            if (*(int *)(p + 0x50) == 10) {
                func_0207193c(8);
                func_0205dc0c(3, 0, 0, 0, 3, 0x10, 0);
            }
            else if (*(int *)(p + 0x50) == (*(unsigned char *)((char *)&data_020cff60 + 1) + 1) * 2 + 10) {
                func_02071dbc();
                func_020716e8(0xc);
            }
            if (*(int *)(p + 0x50) >= (*(unsigned char *)((char *)&data_020cff60 + 1) + 1) * 3 + 10 && *(int *)(p + 0x48) >= 0) {
                *(int *)(p + 0x4c) += (*(unsigned char *)((char *)&data_020cff60 + 1) + 1) * 6;
                *(int *)(p + 0x48) -= (*(unsigned char *)((char *)&data_020cff60 + 1) + 1) * 6;
                if (*(int *)(p + 0x48) < 8) {
                    *(int *)(p + 0x48) = -1;
                    func_02071d94();
                    func_02071de4();
                    func_020716e8(4);
                    func_020719fc(8);
                    func_0205dc44(func_02008444());
                }
            }
        }
    }
}
