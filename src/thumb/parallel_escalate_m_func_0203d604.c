extern unsigned char data_020cff60;
extern unsigned int *func_02008444(void);
extern void func_0203cc20(unsigned char *object);
extern void func_0203d65c(unsigned int *object);
extern void func_02075e44(unsigned char *buffer);

void func_0203d604(unsigned char *param_1)
{
    unsigned char buffer[84];

    func_0203cc20(param_1);
    func_0203d65c(func_02008444());
    if (param_1[0x61] != 0) {
        func_02075e44(buffer);
        switch (buffer[0]) {
        default:
            data_020cff60 = 1;
            break;
        case 2:
            data_020cff60 = 2;
            break;
        case 3:
            data_020cff60 = 3;
            break;
        case 4:
            data_020cff60 = 4;
            break;
        case 5:
            data_020cff60 = 5;
            break;
        }
    }
}
