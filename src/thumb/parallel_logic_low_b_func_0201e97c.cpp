struct Buffer {
    virtual ~Buffer();
    int before[2];
    int result;
};
extern "C" Buffer *func_0201e664(void *pack,int block,unsigned int index);

extern "C" int func_0201e97c(void *pack,int block,unsigned int index)
{
    Buffer *buffer;
    int result;

    buffer = func_0201e664(pack,block,index);
    if (buffer != 0) {
        result = buffer->result;
        delete buffer;
        return result;
    }
    return 0;
}
