class Handler02022ca4 {
public:
    virtual void slot0();
    virtual void slot1();
    virtual int slot2(int value, int flag);
};

struct Owner02022ca4 {
    unsigned short pad;
    unsigned short type;
};

extern "C" {
int func_02022ca4(Owner02022ca4 *owner, int value)
{
    int offset;

    switch (owner->type) {
    case 1:
        offset = 0xfa4;
        break;
    case 2:
        offset = 0xfa8;
        break;
    case 3:
        offset = 0xfac;
        break;
    case 4:
        offset = 0xfb0;
        break;
    case 5:
        offset = 0xfb4;
        break;
    case 6:
        offset = 0xfb8;
        break;
    case 7:
        offset = 0xfbc;
        break;
    default:
        goto done;
    }
    value = ((Handler02022ca4 *)((char *)owner + offset))->slot2(value, 0);
done:
    return value;
}
}
