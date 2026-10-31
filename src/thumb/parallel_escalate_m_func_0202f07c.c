struct Touch {
    unsigned short x;
    unsigned short y;
    unsigned short touch;
    unsigned short validity;
};
struct Pen {
    unsigned char cur;
    unsigned char prev;
    unsigned char count;
    unsigned char pad3;
    unsigned short x;
    unsigned short y;
    unsigned short validity;
    unsigned short pada;
    int wordc;
    unsigned short startX;
    unsigned short startY;
    unsigned short dragX;
    unsigned short dragY;
};
extern int func_02077ca8(struct Touch *touch);

void func_0202f07c(struct Pen *pen)
{
    struct Touch touch;
    unsigned char count;

    if (func_02077ca8(&touch) == 0) {
        pen->prev = pen->cur;
        pen->cur = 0;
        pen->validity = touch.validity;
        if (touch.touch != 0) {
            pen->cur |= 1;
        }
        if ((~pen->prev & pen->cur) != 0) {
            pen->cur |= 2;
            if (pen->count != 0) {
                pen->cur |= 4;
                count = 0;
            } else {
                count = 0x10;
            }
            pen->count = count;
        }
        if (pen->count != 0) {
            pen->count--;
        }
        if ((pen->prev & 1) != 0 && (pen->cur & 1) == 0) {
            pen->cur |= 8;
        }
        if (touch.validity == 0) {
            pen->x = touch.x;
            pen->y = touch.y;
            if ((pen->cur & 2) != 0) {
                pen->startX = touch.x;
                pen->startY = touch.y;
                pen->dragY = 0;
                pen->dragX = pen->dragY;
                return;
            }
            if ((pen->prev & 1) != 0 && (pen->cur & 1) != 0) {
                pen->dragX = touch.x;
                pen->dragY = touch.y;
            }
        }
    }
}
