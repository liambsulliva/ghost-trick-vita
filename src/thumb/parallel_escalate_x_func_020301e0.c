extern unsigned int data_020865f8[];
extern void *func_0200883c(void);
extern void *func_02008824(void *mgr);
extern unsigned char *func_0201e888(void *sys,int a,int id,int b,int c);
extern void *func_02009c1c(void);
extern void func_0202f1bc(void *dma,unsigned char *src,int dst,int size,int mode);
void func_020301e0(int param_1,int param_2,int param_3)
{
    unsigned char *buf;
    unsigned char *end;
    unsigned char *p;
    if (param_1 < 0 || param_1 >= 4 || param_2 < 0 || param_2 >= 8 || param_3 < 0 || param_3 >= 0x10) {
        return;
    }
    buf = func_0201e888(func_02008824(func_0200883c()),1,(unsigned short)data_020865f8[param_2],1,0);
    end = buf + 0xc00;
    for (p = buf; p < end; p++) {
        if ((*p & 0xf) != 0) {
            *p &= 0xf0;
            *p |= (unsigned char)param_3;
        }
        if ((*p & 0xf0) != 0) {
            *p &= 0xf;
            *p |= (unsigned char)(param_3 << 4);
        }
    }
    func_0202f1bc(func_02009c1c(),buf,param_1 * 0xc00,0xc00,9);
    func_0202f1bc(func_02009c1c(),buf,param_1 * 0xc00,0xc00,0x17);
}
