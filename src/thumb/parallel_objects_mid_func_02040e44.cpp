class Object02040e44_0 { public: virtual void slot0(); virtual void slot1(); virtual void slot2(); virtual void slot3(); virtual void slot4(); virtual void slot5(); virtual void slot6(); virtual void slot7(); virtual void slot8(); virtual void slot9(); virtual void slot10(); };
extern "C" {
typedef int code(...);
extern int func_0200b518(...);
extern int func_0200b598(...);
void func_02040e44(int *param_1, int param_2, unsigned int param_3, unsigned int param_4) {
  func_0200b518((int)(param_1 + 10), 1, 0x80000000, 0x80000000);
  if (param_2 != 0) {
    param_1[0x2f] = param_2;
    func_0200b598((int)((char *)param_1 + 0x28), param_2, 0xf);
    ((Object02040e44_0 *)(param_1))->slot10();
  }
  return;
}
}
