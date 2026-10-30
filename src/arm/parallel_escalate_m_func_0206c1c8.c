extern int func_02075c98(void); extern void func_02075cac(int); extern int *func_0206b728(int *,int); extern void func_0206b8d4(int *);
int func_0206c1c8(int *p) { int r,t,irq; int *q; irq=func_02075c98(); t=p[5]; t=(t&8)!=0; r=1; if(t) r=0; if(r==0) p[5]=p[5]&~8; func_02075cac(irq); q=func_0206b728(p,1); if(q) func_0206b8d4(q); return r; }

