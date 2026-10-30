struct Bits { unsigned char first : 1; unsigned char second : 1; unsigned char third : 1; unsigned char rest : 5; };
void func_ov006_020d76e0(unsigned char *p)
{
    ((struct Bits *)(p + 0xc2))->first = 1;
}
