class Base02007aa8 {
public:
    Base02007aa8();
    virtual ~Base02007aa8();
};

class Obj02007ab8 : public Base02007aa8 {
public:
    Obj02007ab8();
    virtual ~Obj02007ab8();
    unsigned int f04;
    unsigned int f08;
    unsigned int f0c;
    unsigned short f10;
    unsigned char b0 : 1;
    unsigned char b1 : 1;
    unsigned char b2 : 1;
    unsigned char b3 : 1;
    unsigned char b4 : 1;
    unsigned char b5 : 1;
    unsigned char b6 : 2;
    unsigned char f13;
    unsigned int f14;
    unsigned int f18;
    unsigned int f1c;
    unsigned int f20;
    int f24;
    unsigned int f28;
};

Obj02007ab8::Obj02007ab8() {
    f04 = 0;
    f08 = 0;
    f10 = 0;
    b0 = 0;
    b1 = 1;
    b2 = 0;
    b3 = 0;
    b4 = 0;
    b6 = 0;
    f13 = 0;
    f18 = 0;
    f1c = 0;
    f20 = 0;
    f24 = -1;
    f28 = 0;
}
