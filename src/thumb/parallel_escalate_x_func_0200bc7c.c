extern void func_0200bcc0(unsigned char *object,int a,int b);
void func_0200bc7c(unsigned char *object)
{
    unsigned int i;
    unsigned char *entry;
    if ((object[0xa83] & 1) != 0) {
        object[0xa80] = object[0xa81];
        object[0xa82] = (unsigned int)object[0xa81] >> 2;
        func_0200bcc0(object,0x2c0,0x400);
        for (i = 0; i < 16; i++) {
            entry = object + i * 8;
            *(unsigned int *)(entry + 0xa00) = 0;
            *(unsigned int *)(entry + 0xa04) = 0;
        }
    }
}
