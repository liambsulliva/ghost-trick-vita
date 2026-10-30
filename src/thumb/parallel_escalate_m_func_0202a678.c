extern void func_020070b0(unsigned int id, int mode);
extern void func_0202a65c(int object, int value, int flag);

void func_0202a678(int object, unsigned int id)
{
    unsigned short *rec;
    int i;

    rec = (unsigned short *)(object + 0x1c8);
    if (*(unsigned char *)(object + 0x24) != 2 && id != 0xffff) {
        for (i = 0; i <= 1; i++) {
            if (i >= 1) {
                return;
            }
            if (id == *rec) {
                break;
            }
            rec = rec + 6;
        }
        func_020070b0(*rec, 6);
        func_0202a65c(object, *(int *)(rec + 4), 1);
        func_0202a65c(object, *(int *)(rec + 2), 1);
        *rec = 0xffff;
        *(int *)(rec + 2) = 0;
        *(int *)(rec + 4) = 0;
        *(unsigned short *)(object + 0x2c) = *(unsigned short *)(object + 0x2c) - 1;
    }
}
