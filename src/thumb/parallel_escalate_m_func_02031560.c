struct State020cff60 {
    unsigned char unk0;
    unsigned char unk1;
    signed char wait;
    signed char flag;
    int frame;
};

extern struct State020cff60 data_020cff60;
void *func_0202f25c(void);
void func_0201dd0c(void *object);
void func_02031598(unsigned int flags);

void func_02031560(void)
{
    data_020cff60.frame++;
    data_020cff60.wait--;
    if (data_020cff60.wait < 0 && data_020cff60.flag != 0) {
        data_020cff60.flag = 0;
    }
    func_0201dd0c(func_0202f25c());
    func_02031598(1);
}
