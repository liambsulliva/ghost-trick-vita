typedef struct HmacFuncs0207235c {
    unsigned int dlength;
    unsigned int blength;
    void *context;
    void *hash_buf;
    void (*HashReset)(void *context);
    void (*HashSetSource)(void *context, const void *data, unsigned int length);
    void (*HashGetDigest)(void *context, void *digest);
} HmacFuncs0207235c;

void func_0207235c(void *mac, const void *message, unsigned int message_length, const void *key, unsigned int key_length, HmacFuncs0207235c *funcs)
{
    unsigned char temp_key[64];
    unsigned char ipad[64];
    unsigned char opad[64];
    unsigned int i;

    if (mac == 0 || message == 0 || message_length == 0 || key == 0 || key_length == 0 || funcs == 0) {
        return;
    }
    if (key_length > funcs->blength) {
        funcs->HashReset(funcs->context);
        funcs->HashSetSource(funcs->context, key, key_length);
        funcs->HashGetDigest(funcs->context, temp_key);
        key = temp_key;
        key_length = funcs->dlength;
    }
    for (i = 0; i < key_length; i++) {
        ipad[i] = (unsigned char)(((const unsigned char *)key)[i] ^ 0x36);
    }
    for (; i < funcs->blength; i++) {
        ipad[i] = 0x00 ^ 0x36;
    }
    funcs->HashReset(funcs->context);
    funcs->HashSetSource(funcs->context, ipad, funcs->blength);
    funcs->HashSetSource(funcs->context, message, message_length);
    funcs->HashGetDigest(funcs->context, funcs->hash_buf);
    for (i = 0; i < key_length; i++) {
        opad[i] = (unsigned char)(((const unsigned char *)key)[i] ^ 0x5c);
    }
    for (; i < funcs->blength; i++) {
        opad[i] = 0x00 ^ 0x5c;
    }
    funcs->HashReset(funcs->context);
    funcs->HashSetSource(funcs->context, opad, funcs->blength);
    funcs->HashSetSource(funcs->context, funcs->hash_buf, funcs->dlength);
    funcs->HashGetDigest(funcs->context, mac);
}
