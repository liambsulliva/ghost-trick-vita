typedef struct SHA1Context {
    unsigned int h[5];
    unsigned char block[64];
    unsigned int pool;
    unsigned int blocks_low;
    unsigned int blocks_high;
} SHA1Context;

typedef void (*BlockFunc)(void *context);

extern BlockFunc data_020b174c;
extern void func_02072fa8(void *dst, unsigned char data, unsigned int size);

void func_02071f1c(SHA1Context *context, unsigned char input, unsigned int length)
{
    while (length > 0) {
        unsigned int rest = 64 - context->pool;
        if (rest > length) {
            rest = length;
        }
        func_02072fa8(&context->block[context->pool], input, rest);
        length -= rest;
        context->pool += rest;
        if (context->pool >= 64) {
            data_020b174c(context);
            context->pool = 0;
            ++context->blocks_low;
            if (!context->blocks_low) {
                ++context->blocks_high;
            }
        }
    }
}
