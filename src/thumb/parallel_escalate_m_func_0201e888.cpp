class Obj {
public:
  virtual void s0();
  virtual void s1();
  virtual unsigned char *s2(unsigned int a, void *b);
  virtual unsigned char *s3(unsigned int a);
  virtual void s4();
  virtual void s5();
  virtual void s6();
  virtual void s7(unsigned char *p, unsigned int v);
  virtual void s8();
  virtual void s9();
  virtual unsigned int s10();
};

extern "C" {
extern char data_02084e04;
Obj *func_0201e664(int *a, int b, unsigned int c);
void *func_02007bd8(void);
Obj *func_02007b70(void *p);
unsigned char *func_020086ac(void *p);
void func_02074860(unsigned char *p, unsigned int v);
unsigned int func_0201e880(unsigned char *p);
void func_02073174(unsigned char *p, unsigned char *q);
}

extern "C" unsigned char *func_0201e888(int *param_1, int param_2, unsigned int param_3, int param_4, Obj *param_5)
{
  Obj *o;
  unsigned char *res;
  unsigned int n;
  unsigned char *buf;

  o = func_0201e664(param_1, param_2, param_3);
  res = 0;
  param_1[1] = 0;
  if (param_5 == 0) {
    param_5 = func_02007b70(func_02007bd8());
  }
  if (o != 0) {
    n = o->s10();
    if (n & 0x80000000) {
      n &= 0xfffff;
      buf = func_020086ac(func_02007bd8());
      o->s7(buf, n);
      func_02074860(buf, n);
      n = func_0201e880(buf);
      if (param_4 != 0) {
        res = param_5->s3(n);
      }
      else {
        res = param_5->s2(n, &data_02084e04);
      }
      if (res == 0) {
        return 0;
      }
      func_02073174(buf, res);
    }
    else {
      if (param_4 != 0) {
        res = param_5->s3(n);
      }
      else {
        res = param_5->s2(n, &data_02084e04);
      }
      if (res == 0) {
        return 0;
      }
      o->s7(res, n);
    }
    if (res != 0) {
      func_02074860(res, n);
      param_1[1] = n;
    }
  }
  if (o != 0) {
    o->s1();
  }
  return res;
}
