extern void func_0207303c(void *first,void *second,unsigned int size);
struct StreamState {
    int before;
    unsigned int position;
    unsigned int length;
    unsigned char *data;
};

unsigned int func_0201f3f8(struct StreamState *object,void *buffer,unsigned int size)
{
    unsigned int position;

    position = object->position;
    if (position + size > object->length) {
        size = object->length - position + 1;
    }
    if (size != 0) {
        func_0207303c(object->data + position,buffer,size);
        object->position += size;
    }
    return size;
}
