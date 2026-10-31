typedef struct {
    unsigned int h[5];
    unsigned char block[64];
    unsigned int num;
    unsigned int Nl;
    unsigned int Nh;
} SHA1Context;

extern const unsigned char data_020a2528[];
extern const unsigned char data_020a2529[];
extern void func_0207203c(SHA1Context *context, const void *input, unsigned int length);
extern void func_02071f1c(SHA1Context *context, unsigned int value, unsigned int length);
extern void func_0207303c(const void *src, void *dst, unsigned int size);

#define SWAP32(x) ((((x) >> 24) & 0xff) | (((x) >> 8) & 0xff00) | (((x) << 8) & 0xff0000) | (((x) << 24) & 0xff000000))

void func_020720d0(SHA1Context *context, void *digest)
{
    unsigned int footer[2];
    unsigned int lo;
    unsigned int hi;

    lo = (context->Nl << 9) + (context->num << 3);
    footer[1] = SWAP32(lo);
    hi = (context->Nh << 9) + (context->Nl >> 23);
    footer[0] = SWAP32(hi);
    func_0207203c(context, data_020a2528, 1);
    if (64 - context->num < sizeof(footer)) {
        func_0207203c(context, data_020a2529, 64 - context->num);
    }
    func_02071f1c(context, 0, 56 - context->num);
    func_0207203c(context, footer, sizeof(footer));
    context->h[0] = SWAP32(context->h[0]);
    context->h[1] = SWAP32(context->h[1]);
    context->h[2] = SWAP32(context->h[2]);
    context->h[3] = SWAP32(context->h[3]);
    context->h[4] = SWAP32(context->h[4]);
    func_0207303c(context, digest, 20);
}
