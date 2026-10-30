class Object02040e04 {
public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28();
    char pad04[0x24];
    char sprite[0x94];
    int image;
    unsigned char palette;
};

extern "C" {
void func_0200b518(void *sprite, int mode, int x, int y);
void func_0200b598(void *sprite, int image, int flags);
void func_020280a8(void *object, int value);

void func_02040e04(Object02040e04 *object)
{
    func_0200b518(object->sprite, 1, 0x80000000, 0x80000000);
    if (object->image != 0) {
        func_0200b598(object->sprite, object->image, 0xf);
        func_020280a8(object->sprite, object->palette);
        object->v28();
    }
}
}
