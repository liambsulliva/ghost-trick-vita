typedef void code();

void func_0207d97c(void)
{
    code **p;
    code *f;

    for (p = (code **)0x020a2940; p != 0 && (f = *p) != 0; p++) {
        f();
    }
}
