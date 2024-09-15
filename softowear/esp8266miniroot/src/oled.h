#ifndef __oledd_H__
#define __oledd_H__
#include <U8g2lib.h>

void INF_wifi_wiat(void);
void INF_wifi_err(void);
void INF_ipdis(void);
void BQ_yb(void);
void BQ_la(void);
void BQ_tx(void);
void BQ_gp(void);
void BQ_nh(void);
void BQ_sj(void);
void BQ_zc(void);
void BQ_yh(void);
void BQ_TT(void);
void OledInit(void);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C& getoledhand(void);

#endif