typedef struct {
    unsigned int digestLength;
    unsigned int blockLength;
    void *context;
    void *hashBuf;
    void *init;
    void *update;
    void *getHash;
} HMACFuncs;

extern HMACFuncs data_020a2534;
extern void func_02071fe8();
extern void func_0207203c();
extern void func_020720d0();
extern void func_0207235c(void *digest, const void *bin, unsigned int binLen, const void *key, unsigned int keyLen, HMACFuncs *funcs);

void func_020722c8(void *digest, const void *bin, unsigned int binLen, const void *key, unsigned int keyLen)
{
    unsigned char context[96];
    unsigned char hashBuf[20];
    HMACFuncs funcs = data_020a2534;

    funcs.context = context;
    funcs.hashBuf = hashBuf;
    funcs.init = func_02071fe8;
    funcs.update = func_0207203c;
    funcs.getHash = func_020720d0;
    func_0207235c(digest, bin, binLen, key, keyLen, &funcs);
}
