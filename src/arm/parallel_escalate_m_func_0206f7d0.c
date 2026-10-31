typedef int BOOL;

#define reg_G2_BG0CNT (*(volatile unsigned short *)0x04000008)
#define reg_G2_BG0OFS (*(volatile unsigned int *)0x04000010)
#define reg_G3X_DISP3DCNT (*(volatile unsigned short *)0x04000060)
#define reg_G3X_CLEAR_COLOR (*(volatile unsigned int *)0x04000350)
#define reg_G3X_CLEAR_DEPTH (*(volatile unsigned short *)0x04000354)
#define reg_G3X_CLRIMAGE_OFFSET (*(volatile unsigned short *)0x04000356)
#define reg_G3X_FOG_COLOR (*(volatile unsigned int *)0x04000358)
#define reg_G3X_FOG_OFFSET (*(volatile unsigned short *)0x0400035c)
#define reg_G3_POLYGON_ATTR (*(volatile unsigned int *)0x040004a4)
#define reg_G3_TEXIMAGE_PARAM (*(volatile unsigned int *)0x040004a8)
#define reg_G3_TEXPLTT_BASE (*(volatile unsigned int *)0x040004ac)
#define reg_G3_END_VTXS (*(volatile unsigned int *)0x04000504)
#define reg_G3X_GXSTAT (*(volatile unsigned int *)0x04000600)

#define REG_G3X_DISP3DCNT_GO_MASK 0x00002000
#define REG_G3X_DISP3DCNT_RO_MASK 0x00001000
#define REG_G3X_DISP3DCNT_AAE_MASK 0x00000010
#define REG_G3X_DISP3DCNT_ATE_MASK 0x00000004
#define REG_G3X_DISP3DCNT_THS_MASK 0x00000002
#define REG_G3X_DISP3DCNT_THS_SHIFT 1
#define REG_G3X_GXSTAT_FI_MASK 0xc0000000
#define REG_G3X_GXSTAT_FI_SHIFT 30
#define REG_G3X_GXSTAT_B_MASK 0x08000000
#define REG_G3X_GXSTAT_SE_MASK 0x00008000
#define REG_G2_BG0CNT_PRIORITY_MASK 0x0003
#define REG_G2_BG0CNT_PRIORITY_SHIFT 0

extern void func_0206f940(void);
extern void func_0206f964(void);
extern void func_0206fb08(void);

void func_0206f7d0(void)
{
    func_0206f940();
    reg_G3_END_VTXS = 0;
    while (reg_G3X_GXSTAT & REG_G3X_GXSTAT_B_MASK) {
    }
    reg_G3X_DISP3DCNT = 0;
    reg_G3X_GXSTAT = 0;
    reg_G2_BG0OFS = 0;

    reg_G3X_DISP3DCNT |= REG_G3X_DISP3DCNT_GO_MASK;
    reg_G3X_DISP3DCNT |= REG_G3X_DISP3DCNT_RO_MASK;
    reg_G3X_DISP3DCNT = (unsigned short)((reg_G3X_DISP3DCNT & ~(REG_G3X_DISP3DCNT_THS_MASK | REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK)) | (0 << REG_G3X_DISP3DCNT_THS_SHIFT));
    reg_G3X_DISP3DCNT = (unsigned short)((reg_G3X_DISP3DCNT & ~(REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK)) | REG_G3X_DISP3DCNT_AAE_MASK);
    reg_G3X_DISP3DCNT &= ~(REG_G3X_DISP3DCNT_ATE_MASK | REG_G3X_DISP3DCNT_RO_MASK | REG_G3X_DISP3DCNT_GO_MASK);

    reg_G3X_GXSTAT |= REG_G3X_GXSTAT_SE_MASK;
    reg_G3X_GXSTAT = (reg_G3X_GXSTAT & ~REG_G3X_GXSTAT_FI_MASK) | (2 << REG_G3X_GXSTAT_FI_SHIFT);
    func_0206f964();
    reg_G3X_CLEAR_COLOR = 0;
    reg_G3X_CLEAR_DEPTH = 0x7fff;
    reg_G3X_CLRIMAGE_OFFSET = 0;
    reg_G3X_FOG_COLOR = 0;
    reg_G3X_FOG_OFFSET = 0;
    reg_G2_BG0CNT = (unsigned short)((reg_G2_BG0CNT & ~REG_G2_BG0CNT_PRIORITY_MASK) | (0 << REG_G2_BG0CNT_PRIORITY_SHIFT));
    func_0206fb08();
    reg_G3_POLYGON_ATTR = 0x001f0080;
    reg_G3_TEXIMAGE_PARAM = 0;
    reg_G3_TEXPLTT_BASE = 0;
}
