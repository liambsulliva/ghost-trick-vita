extern int func_02072f08();
extern int func_0207487c();
extern int func_020761d0();
extern char data_020d02a0;

void func_02069df8(void)
{
    int buf;

    buf = 0x020d0240;
    *(int *)&data_020d02a0 = buf;
    func_02072f08(0, buf, 96);
    func_0207487c(buf, 96);
    func_020761d0(11, 0x0206a0ec);
}
