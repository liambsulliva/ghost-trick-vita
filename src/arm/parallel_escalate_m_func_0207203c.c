typedef struct {
    unsigned int h[5];
    unsigned char block[64];
    unsigned int num;
    unsigned int Nl;
    unsigned int Nh;
} SHA1Context;

extern void (*data_020b174c)(SHA1Context *context);
extern void func_0207303c(const void *src, void *dst, unsigned int size);

void func_0207203c(SHA1Context *context, const void *input, unsigned int length)
{
    unsigned int rest;

    while (length > 0) {
        rest = 64 - context->num;
        if (rest > length) {
            rest = length;
        }
        func_0207303c(input, &context->block[context->num], rest);
        input = (const unsigned char *)input + rest;
        length -= rest;
        context->num += rest;
        if (context->num >= 64) {
            data_020b174c(context);
            context->num = 0;
            ++context->Nl;
            if (context->Nl == 0) {
                ++context->Nh;
            }
        }
    }
}
