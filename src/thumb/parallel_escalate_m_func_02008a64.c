typedef struct {
    int x;
    int y;
} Vec;
typedef struct {
    unsigned int name;
    unsigned short kind;
    unsigned short tag;
    unsigned int owner;
    unsigned int ptr;
    unsigned int pad10;
    unsigned int extra;
    unsigned int pad18;
    Vec a;
    Vec b;
    unsigned int z0;
    unsigned int z1;
    unsigned int z2;
} Message;
extern Vec data_020a2938;
void func_02008a64(Message *message,unsigned int owner,unsigned int name,unsigned int ptr,unsigned short tag,unsigned int extra)
{
    message->name = name;
    message->kind = 2;
    message->tag = tag;
    message->owner = owner;
    message->ptr = ptr;
    message->extra = extra;
    message->a = data_020a2938;
    message->b = data_020a2938;
    message->z0 = 0;
    message->z1 = 0;
    message->z2 = 0;
}
