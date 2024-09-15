#include "Arduino.h"
#include "ServoMv.h"
#include "action.h"

//-----------------------------------------------动作函数--------------------------------------------------
// 开机蹦跶一下 正常站立
void MV_zcc(void)
{
  SetAllSiPin(105, 1, 0, 1, 0);
  SetAllSiPin(75, 0, 1, 0, 1);

  SetAllSePin(50);
  delay(500);
  SetAllSePinStep(90, 400);
  SetAllSePinStep(50, 600);
}

// 默认姿势
void MV_dfpos(void)
{
  SetAllSiPinStep(75, 80, 0, 1, 0, 1);
  SetAllSiPinStep(105, 80, 1, 0, 1, 0);
  SetAllSePinStep(50, 300); // 站立
}

void MV_dfposfast(void)
{
  SetAllSiPin(75, 0, 1, 0, 1);
  SetAllSiPin(105, 1, 0, 1, 0);
  SetAllSePin(50); // 快速站立
}

// 哈哈姿势
void MV_haha(void)
{
  MV_dfpos();
  for (uint8_t i = 0; i < 2; i++)
  {
    SetAllSePinStep(160, 800, 0, 1, 0, 1);
    SetAllSePinStep(20, 800, 0, 1, 0, 1);
  }
  MV_dfpos();
}

void MV_Jump(void)
{
  MV_dfpos();
  SetAllSiPinStep(90, 200);
  SetAllSePinStep(90, 800, 1, 1, 1, 1);
  delay(500);
  SetAllSePin(0); // 快速站立
  delay(500);
  SetAllSiPinStep(75, 100, 0, 1, 0, 1);
  SetAllSiPinStep(105, 100, 1, 0, 1, 0);
  SetAllSePinStep(50, 400); // 站立
}

// 前进
void MV_go(void)
{

  MV_dfpos();

  SetAllSePinStep(80, 150, 0, 1, 1, 0); // 抬起2 3 脚
  uint8_t i = 6;
  while (i--)
  {

    SetAllSiPinStep(118, 100, 0, 1, 1, 0); // 2 3 脚前移
    SetAllSePinStep(70, 100, 0, 1, 0, 1);  // 放下2 4 脚展开重心到屁股
    SetAllSePinStep(80, 50, 0, 0, 0, 1);   // 放下4 脚展开重心到屁股
    SetAllSePinStep(60, 100, 0, 0, 1, 0);  // 放下3 脚

    SetAllSePinStep(70, 100, 1, 0, 0, 0); // 抬起1 脚
    SetAllSePinStep(90, 50, 0, 0, 0, 1);  // 抬起4 脚
    SetAllSiPinStep(83, 100, 0, 1, 1, 0); // 2 3 脚后移
    /******************************************************************/
    SetAllSiPinStep(118, 100, 1, 0, 0, 1); // 1 4 脚前移
    SetAllSePinStep(70, 100, 0, 1, 0, 1);  // 放下2 4 脚展开重心到屁股
    SetAllSePinStep(80, 50, 0, 1, 0, 0);   // 放下2 脚展开重心到屁股
    SetAllSePinStep(60, 100, 1, 0, 0, 0);  // 放下1 脚

    SetAllSePinStep(70, 100, 0, 1, 0, 0); // 抬起2 脚
    SetAllSePinStep(90, 50, 0, 0, 1, 0);  // 抬起3 脚
    SetAllSiPinStep(83, 100, 1, 0, 0, 1); // 1 4 脚后移
  }
  MV_dfpos();
}

void MV_back(void)
{
}

// 右移
void MV_yy(void)
{
  MV_dfpos();

  SetAllSePinStep(85, 500, 1, 1, 1, 1); // 1 2 3 4下沉
  uint8_t i = 6;
  while (i--)
  {

    SetAllSePinStep(0, 100, 0, 0, 1, 1);  // 3 4 快
    SetAllSePinStep(0, 1000, 1, 1, 0, 0); // 1 2  慢

    SetAllSePinStep(85, 1000, 0, 0, 1, 1); // 3 4 慢
    SetAllSePinStep(85, 100, 1, 1, 0, 0);  // 1 2  快
  }
  MV_dfpos();
}

// 左转
void MV_zz(void)
{

  MV_dfpos();
  SetAllSePinStep(80, 100, 0, 1, 1, 0); // 抬起2 3 脚
  int i = 6;
  while (i--)
  {
    // 3:105 2:75
    SetAllSiPinStep(52, 40, 0, 1, 0, 0);  // 2 3 逆时针转动
    SetAllSiPinStep(128, 40, 0, 0, 1, 0); // 2 3 逆时针转动

    SetAllSePinStep(50, 60, 0, 1, 1, 0); // 放下 2 3 脚

    SetAllSePinStep(80, 60, 1, 0, 0, 1); // 抬起1 4 脚

    SetAllSiPinStep(98, 40, 0, 1, 0, 0); // 2 3 顺时针转动
    SetAllSiPinStep(82, 40, 0, 0, 1, 0); // 2 3 顺时针转动

    SetAllSiPinStep(82, 40, 1, 0, 0, 0); // 1 4 逆时针转动
    SetAllSiPinStep(98, 40, 0, 0, 0, 1); // 1 4 逆时针转动
    SetAllSePinStep(50, 60, 1, 0, 0, 1); // 放下1 4 脚

    SetAllSePinStep(80, 60, 0, 1, 1, 0); // 抬起2 3 脚

    SetAllSiPinStep(128, 40, 1, 0, 0, 0); // 1 4 顺时针转动
    SetAllSiPinStep(52, 40, 0, 0, 0, 1);  // 1 4 顺时针转动
  }
  MV_dfpos();
}

// 右转
void MV_yz(void)
{

  MV_dfpos();
  SetAllSePinStep(80, 100, 1, 0, 0, 1); // 抬起1 4 脚
  int i = 6;
  while (i--)
  {
    // 1:105 4:75
    SetAllSiPinStep(128, 40, 1, 0, 0, 0); // 1- 4 顺针转动
    SetAllSiPinStep(52, 40, 0, 0, 0, 1);  // 1- 4 顺针转动

    SetAllSePinStep(50, 60, 1, 0, 0, 1); //  放下1 4 脚
    SetAllSePinStep(80, 60, 0, 1, 1, 0); // 抬起 2 3 脚

    SetAllSiPinStep(82, 40, 1, 0, 0, 0); // 1- 4 逆时针转动
    SetAllSiPinStep(98, 40, 0, 0, 0, 1); // 1- 4 逆时针转动

    SetAllSiPinStep(98, 40, 0, 1, 0, 0); // 2- 3 顺时针转动
    SetAllSiPinStep(82, 40, 0, 0, 1, 0); // 2- 3 顺时针转动

    SetAllSePinStep(50, 60, 0, 1, 1, 0); // 放下2 3 脚
    SetAllSePinStep(80, 60, 1, 0, 0, 1); // 抬起1 4 脚

    SetAllSiPinStep(52, 40, 0, 1, 0, 0);  // 2- 3 逆时针转动
    SetAllSiPinStep(128, 40, 0, 0, 1, 0); // 2- 3 逆时针转动
  }
  MV_dfpos();
}

// 来呀
void MV_ly(void)
{
  MV_dfpos();
  SetAllSePinStep(120, 500, 0, 1, 0, 1);
  SetAllSePinStep(70, 200, 0, 0, 0, 1);
  SetAllSiPinStep(160, 200, 0, 0, 1, 0);
  for (uint8_t i = 0; i < 4; i++)
  {
    SetAllSePinStep(140, 300, 0, 0, 1, 0);
    SetAllSePinStep(80, 300, 0, 0, 1, 0);
  }
  SetAllSiPinStep(75, 100, 0, 1, 0, 1);
  SetAllSiPinStep(105, 1000, 0, 0, 1, 0);
  SetAllSePinStep(50, 500); // 站立
}

// 招手
void MV_zs(void)
{
  MV_dfpos();
  SetAllSePinStep(120, 500, 0, 1, 0, 1);
  SetAllSePinStep(70, 200, 0, 0, 0, 1);
  SetAllSiPinStep(100, 200, 0, 0, 1, 0);
  for (uint8_t i = 0; i < 4; i++)
  {
    SetAllSePinStep(140, 300, 0, 0, 1, 0);
    SetAllSePinStep(80, 300, 0, 0, 1, 0);
  }
  SetAllSiPinStep(75, 100, 0, 1, 0, 1);
  SetAllSiPinStep(105, 100, 1, 0, 1, 0);
  SetAllSePinStep(50, 500); // 站立
}

// 左右摇摆
void MV_yba(void)
{
  MV_dfpos();
  SetAllSiPinStep(90, 300, 1, 1, 1, 1);
  uint8_t i = 6;
  while (i--)
  {
    SetAllSePin(10, 0, 0, 1, 1);
    SetAllSePin(90, 1, 1, 0, 0);
    delay(200);
    SetAllSePin(10, 1, 1, 0, 0);
    SetAllSePin(90, 0, 0, 1, 1);
    delay(200);
  }
  SetAllSiPinStep(75, 100, 0, 1, 0, 1);
  SetAllSiPinStep(105, 100, 1, 0, 1, 0);
  SetAllSePinStep(50, 500); // 站立
}

// 俯卧撑
void MV_fwc(void)
{
  MV_dfpos();
  SetAllSiPinStep(40, 300, 0, 1, 0, 1);
  SetAllSePinStep(90, 200, 0, 1, 0, 1);
  SetAllSiPinStep(90, 100, 1, 0, 1, 0);
  uint8_t i = 4;
  while (i--)
  {
    SetAllSePinStep(90, 200, 1, 0, 1, 0);
    SetAllSePinStep(40, 400, 1, 0, 1, 0);
  }
  SetAllSiPinStep(75, 500, 0, 1, 0, 1);
  SetAllSiPinStep(105, 80, 1, 0, 1, 0);
  SetAllSePinStep(50, 500); // 站立
}

// 游泳
void MV_yo(void)
{
  MV_dfpos();
  SetAllSePinStep(100, 300, 1, 1, 1, 1);
  SetAllSiPinStep(35, 300, 0, 1, 0, 1);
  uint8_t i = 10;
  while (i--)
  {
    SetAllSePinStep(140, 80, 0, 1, 1, 0);
    SetAllSePinStep(100, 80, 0, 1, 1, 0);
    SetAllSePinStep(140, 80, 1, 0, 0, 1);
    SetAllSePinStep(100, 80, 1, 0, 0, 1);
  }

  SetAllSiPinStep(75, 500, 0, 1, 0, 1);
  SetAllSiPinStep(105, 80, 1, 0, 1, 0);
  SetAllSePinStep(50, 500); // 站立
}


void MV_D1(void)
{
  MV_zz();
  MV_yz();
  MV_zs();
  MV_go();
  MV_yba();
  MV_Jump();
}

void MV_D2(void)
{
  MV_ly();
  MV_haha();
  MV_yz();
  MV_yz();
  MV_go();
  MV_Jump();
  MV_zz();
  MV_fwc();
  MV_zz();
}

//-----------------------------------------------end----------------------------------------------
